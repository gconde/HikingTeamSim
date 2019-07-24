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
#include <vector>
#include <algorithm>

typedef std::map<std::string, std::string> StringMap;
typedef std::map<std::string, double> DoubleMap;

class SIMULATOR_API HikingSimulator
{
public:
    struct Hiker
    {
        Hiker() : speed(0.f) {}

        std::string name; // yaml name
        double speed; // ft/min
    };

    struct Bridge
    {
        Bridge() : fastest_hiker_index(0) {}

        // put fastest person first
        void FastestFirst() {
            if (hikers.empty())
                return;
            std::swap(hikers[0], hikers[fastest_hiker_index]);
            fastest_hiker_index = 0;
        }

        std::string name; // yaml name
        double length; // feet
        std::vector<std::string> hikers;
        int fastest_hiker_index;
    };

    void AddBridge(std::string name, double length);
    void AddHiker(std::string name, double speed, const std::string& bridge);
    int FastestHikerSpeed(const Bridge& bridge) const;
    int HikerSpeed(const std::string& hiker) const;
    double GetTiming(DoubleMap& timings);

private:
    std::map<std::string, Hiker>::const_iterator FastestHiker(const Bridge& bridge) const;

    std::map<std::string, Hiker> hikers_;
    std::vector<Bridge> bridges_;
    std::map<std::string, int> bridgeNameToIndex_;
};

#endif // __SIMULATOR_H__
