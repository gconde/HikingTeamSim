#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#ifdef _WIN32
  #ifdef SIMULATOR_EXPORTS
    #define SIMULATOR_API __declspec(dllexport)
  #else
    #define SIMULATOR_API __declspec(dllimport)
  #endif
#else
  #define SIMULATOR_API
#endif

#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <memory>

typedef std::map<std::string, std::string> StringMap;
typedef std::map<std::string, double> DoubleMap;

namespace TorchAndBridge
{

class SIMULATOR_API HikingSimulator
{
public:
    struct Hiker
    {
        Hiker() : speed_(0.f) {}
        Hiker(std::string name, double speed) : name_(name), speed_(speed) {}

        std::string name_; // yaml name
        double speed_; // ft/min
    };

    struct HikerSortFunctor
    {
        bool operator()(std::shared_ptr<HikingSimulator::Hiker>& h1, std::shared_ptr<HikingSimulator::Hiker>& h2)
        {
            return h1->speed_ > h2->speed_;
        }
    };

    struct Bridge
    {
        Bridge() : length_(0.f) {}

        void SortHikers() { std::sort(hikers_.begin(), hikers_.end(), HikerSortFunctor()); }

        void AddHiker(std::shared_ptr<Hiker> h)
        {
            if (hiker_names_.find(h->name_) == hiker_names_.end())
            {
                hikers_.push_back(h);
                hiker_names_.insert(h->name_);
            }
        }

        void AddHikers(Bridge& bridge)
        {
            for (auto hikerIt = bridge.hikers_.begin(); hikerIt != bridge.hikers_.end(); hikerIt++)
            {
                AddHiker(*hikerIt);
            }
        }

        std::string name_; // yaml name
        double length_; // feet
        std::vector<std::shared_ptr<Hiker>> hikers_;
        std::set<std::string> hiker_names_;
    };

    void AddBridge(std::string name, double length);
    void AddHiker(std::string name, double speed, const std::string& bridge);
    double GetTiming(DoubleMap& timings);

private:
    std::vector<Bridge> bridges_;
    std::map<std::string, int> bridgeNameToIndex_;
};

} // TorchAndBridge

#endif // __SIMULATOR_H__
