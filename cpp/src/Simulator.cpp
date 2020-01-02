#include "Simulator.h"

#include <sstream>
#include <algorithm>

#include "HikingSimException.h"

namespace TorchAndBridge
{

void HikingSimulator::AddBridge(std::string name, double length)
{
    Bridge bridge;
    bridge.name_ = name;
    bridge.length_ = length;
    bridges_.push_back(bridge);
    if (bridgeNameToIndex_.find(name) != bridgeNameToIndex_.end())
    {
        std::ostringstream oss;
        oss << "Duplicate bridge added: " << name;
        throw BadParameterException(oss.str().c_str());
    }
    bridgeNameToIndex_.insert(std::make_pair(name, bridges_.size() - 1));
}

void HikingSimulator::AddHiker(std::string name, double speed, const std::string& bridge)
{
    if (bridgeNameToIndex_.find(bridge) == bridgeNameToIndex_.end())
    {
        std::ostringstream oss;
        oss << "Unknown bridge specified: " << bridge;
        throw BadParameterException(oss.str().c_str());
    }
    int index = bridgeNameToIndex_.find(bridge)->second;
    bridges_.at(index).AddHiker(std::make_shared<Hiker>(name, speed));
}

double HikingSimulator::GetTiming(DoubleMap& timings)
{
    if (bridges_.empty())
    {
        throw BadConfigException("No bridges defined.");
    }
    double total_time = 0.f;
    auto prev_it = bridges_.end();
    for (auto bridgeIt = bridges_.begin(); bridgeIt != bridges_.end(); bridgeIt++)
    {
        if (prev_it != bridges_.end())
        {
            bridgeIt->AddHikers(*prev_it);
        }
        prev_it = bridgeIt;
        bridgeIt->SortHikers();
        std::vector<std::shared_ptr<Hiker>>& hiker_group = bridgeIt->hikers_;
        double segment_time = 0.f;
        if (hiker_group.size() > 0 && hiker_group.size() < 3)
        {
            segment_time = bridgeIt->length_ / hiker_group.back()->speed_;
        }
        else if(hiker_group.size() > 0)
        {
            for (int i = 1; i < hiker_group.size() - 2; ++i)
            {
                segment_time += bridgeIt->length_ / hiker_group[i]->speed_;
            }
            segment_time += bridgeIt->length_ / hiker_group.back()->speed_ +
                bridgeIt->length_ / hiker_group[1]->speed_ * 2 +
                bridgeIt->length_ / hiker_group[0]->speed_ * (hiker_group.size() - 3);
        }
        timings.insert(std::make_pair(bridgeIt->name_, segment_time));
        total_time += segment_time;
    }
    return total_time;
}

} //TorchAndBridge
