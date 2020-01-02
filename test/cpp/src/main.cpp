#include <iostream>

#include "Simulator.h"
#include "YamlLoader.h"

#include "gtest/gtest.h"

std::string yamlFile1;
std::string yamlFile2;

using namespace TorchAndBridge;

int main(int argc, char *argv[]) {
    if (argc < 2)
    {
        std::cout << "Usage:\n" << *argv << " yaml_file1.yaml yaml_file2.yaml\n" << std::endl;
        return -1;
    }
    yamlFile1 = argv[1];
    yamlFile2 = argv[2];
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(HikerSimTest, YamlFile1)
{
    DoubleMap times;
    HikingSimulator hs;
    YamlLoader::LoadYaml(hs, yamlFile1);
    auto total = hs.GetTiming(times);
    EXPECT_TRUE(total > 0.f);
    EXPECT_TRUE(times.size() > 0);
    std::cout << "Bridge\t\tTime(min)\n";
    std::cout << "------------------------------\n";
    for (auto bridgeIt = times.begin(); bridgeIt != times.end(); bridgeIt++)
    {
        std::cout << bridgeIt->first << "\t\t" << bridgeIt->second << "\n";
    }
    std::cout << std::endl;
}

// Generally the expected values should be read from a config file.
// Putting in expected values directly to save time.
TEST(HikerSimTest, YamlFile2)
{
    DoubleMap times;
    HikingSimulator hs;
    YamlLoader::LoadYaml(hs, yamlFile2);
    auto total = hs.GetTiming(times);
    EXPECT_NEAR(24.5f, total, .01f);
    auto bridgeIt = times.find("wood_bridge");
    ASSERT_TRUE(bridgeIt != times.end());
    EXPECT_NEAR(10.f, bridgeIt->second , .01f);
    bridgeIt = times.find("metal_bridge");
    ASSERT_TRUE(bridgeIt != times.end());
    EXPECT_NEAR(14.5f, bridgeIt->second, .01f);
}
