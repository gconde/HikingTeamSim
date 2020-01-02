#ifndef __YAMLLOADER_H__
#define __YAMLLOADER_H__

#ifdef _WIN32
#ifdef SIMULATOR_EXPORTS
#define SIMULATOR_API __declspec(dllexport)
#else
#define SIMULATOR_API __declspec(dllimport)
#endif
#else
#define SIMULATOR_API
#endif

#include "Simulator.h"
#include "yaml-cpp/yaml.h"

namespace TorchAndBridge
{

class SIMULATOR_API YamlLoader
{
public:
    static void LoadYaml(HikingSimulator& hs, const std::string& yamlFile);
private:
    static void check_node(YAML::Node node, const std::string& key);
};

} //TorchAndBridge
#endif //__YAMLLOADER_H__
