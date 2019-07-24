#include "Simulator.h"

#include <sstream>
#include<algorithm>
#include<iterator> 

#include "HikingSimException.h"

void HikingSimulator::AddBridge(std::string name, double length)
{
    Bridge bridge;
    bridge.name = name;
    bridge.length = length;
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
    Hiker hiker;
    hiker.name = name;
    hiker.speed = speed;
    hikers_.insert(std::make_pair(name, hiker));
    if (bridgeNameToIndex_.find(bridge) == bridgeNameToIndex_.end())
    {
        std::ostringstream oss;
        oss << "Unknown bridge specified: " << bridge;
        throw BadParameterException(oss.str().c_str());
    }
    int index = bridgeNameToIndex_.find(bridge)->second;
    bridges_.at(index).hikers.push_back(name);
    int fastest_index = bridges_[index].fastest_hiker_index;
    std::string fast_name = bridges_[index].hikers.at(fastest_index);
    double fastest_speed = speed;
    if (hikers_.find(fast_name) != hikers_.end())
        fastest_speed = hikers_.find(fast_name)->second.speed;
    if (speed > fastest_speed)
    {
        bridges_[index].fastest_hiker_index = bridges_[index].hikers.size() - 1;
    }
}

std::map<std::string, HikingSimulator::Hiker>::const_iterator HikingSimulator::FastestHiker(const HikingSimulator::Bridge& bridge) const
{
    if (bridge.hikers.empty())
        return hikers_.end();
    return hikers_.find(bridge.hikers[bridge.fastest_hiker_index]);
}

int HikingSimulator::FastestHikerSpeed(const HikingSimulator::Bridge& bridge) const
{
    if (bridge.hikers.empty() || FastestHiker(bridge) == hikers_.end())
        return 0;
    return FastestHiker(bridge)->second.speed;
}

int HikingSimulator::HikerSpeed(const std::string& hiker) const
{
    if (hikers_.find(hiker) == hikers_.end())
        return 0;
    return hikers_.find(hiker)->second.speed;
}

double HikingSimulator::GetTiming(DoubleMap& timings)
{
    // for each bridge in bridges
    //  use math to figure out time
    //  store time in the map, keep a running sum
    //  need to keep track of fastest hiker
    std::vector<std::string> hiker_group;
    int fastest_index = 0;
    if (bridges_.size())
    {   
        bridges_.front().FastestFirst();
    }
    else
    {
        throw BadConfigException("No bridges defined.");
    }
    if (hikers_.empty())
    {
        throw BadConfigException("No hikers defined.");
    }
    double total_time = 0.f;
    for (auto bridgeIt = bridges_.begin(); bridgeIt != bridges_.end(); bridgeIt++)
    {
        // keep a moving group
        // move the fastest to beginning for ease
        bridgeIt->FastestFirst();
        int swap_index = hiker_group.size();
        std::copy(bridgeIt->hikers.begin(), bridgeIt->hikers.end(), std::back_inserter(hiker_group));
        if (!bridgeIt->hikers.empty() && FastestHikerSpeed(*bridgeIt) > HikerSpeed(hiker_group[0]))
        {
            std::swap(hiker_group[0], hiker_group[swap_index]);
        }
        // calculate this segment
        double segment_time = 0.f;
        double fast_speed = HikerSpeed(hiker_group[0]);
        if (hiker_group.size() == 1) // a lone hiker
        {
            segment_time = bridgeIt->length / fast_speed;
        }
        else
        {
            // time for fastest hiker to walk slower people
            for (int i = 1; i < hiker_group.size(); ++i)
            {
                segment_time += bridgeIt->length / HikerSpeed(hiker_group[i]);
            }
            // time for fastest hiker to walk back
            segment_time += bridgeIt->length * (hiker_group.size() - 2) / fast_speed;
        }
        timings.insert(std::make_pair(bridgeIt->name, segment_time));
        total_time += segment_time;
    }
    return total_time;
}
