#include "YamlLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "HikingSimException.h"

namespace TorchAndBridge
{

void YamlLoader::LoadYaml(HikingSimulator& hs, const std::string& yamlFile)
{
    std::ifstream ifs(yamlFile);
    if (!ifs.good())
    {
        std::ostringstream oss;
        oss << "Cannot open file " << yamlFile << ".";
        throw BadConfigException(oss.str().c_str());
    }
    ifs.close();
    try
    {
        YAML::Node config = YAML::LoadFile(yamlFile);
        check_node(config, "bridges");
        YAML::Node bridge_list = config["bridges"];
        check_node(config, "hikers");
        YAML::Node hiker_list = config["hikers"];
        std::vector<std::string> hiker_names;
        std::vector<std::string> bridge_names;
        for (YAML::iterator nodeIt = hiker_list.begin(); nodeIt != hiker_list.end(); ++nodeIt)
        {
            check_node(config, nodeIt->as<std::string>());
            hiker_names.push_back(nodeIt->as<std::string>());
        }
        for (YAML::iterator nodeIt = bridge_list.begin(); nodeIt != bridge_list.end(); ++nodeIt)
        {
            check_node(config, nodeIt->as<std::string>());
            bridge_names.push_back(nodeIt->as<std::string>());
        }
        StringMap hikerToBridge;
        for (auto bridgeNameIt = bridge_names.begin(); bridgeNameIt != bridge_names.end(); bridgeNameIt++)
        {
            YAML::Node bridge_info = config[*bridgeNameIt];
            check_node(bridge_info, "length");
            check_node(bridge_info, "hikers");
            hs.AddBridge(*bridgeNameIt, bridge_info["length"].as<double>());
            YAML::Node hiker_list = bridge_info["hikers"];
            for (YAML::iterator hikerIt = hiker_list.begin(); hikerIt != hiker_list.end(); ++hikerIt)
            {
                hikerToBridge.insert(std::make_pair(hikerIt->as<std::string>(), *bridgeNameIt));
            }
        }
        for (auto hikerNameIt = hiker_names.begin(); hikerNameIt != hiker_names.end(); hikerNameIt++)
        {
            YAML::Node hiker_info = config[*hikerNameIt];
            check_node(hiker_info, "speed");
            // skip if hiker is not associated with a bridge
            if (hikerToBridge.find(*hikerNameIt) == hikerToBridge.end())
                continue;
            hs.AddHiker(*hikerNameIt, hiker_info["speed"].as<double>(), hikerToBridge.find(*hikerNameIt)->second);
        }
    }
    catch (const YAML::Exception& ye)
    {
        throw BadConfigException(ye.what());
    }
    catch (const std::exception& e)
    {
        throw BadConfigException(e.what());
    }
}

void YamlLoader::check_node(YAML::Node node, const std::string& key)
{
    if (!node[key])
    {
        std::ostringstream oss;
        oss << "Missing YAML key: " << key << ".";
        throw BadConfigException(oss.str().c_str());
    }
}

}
