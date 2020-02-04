#include "process_nvram.h"

#include <gtest/gtest.h>

#include <string>
#include <string_view>

using namespace std::literals;

namespace asus {
namespace test {

class ProcessNVRamTest : public ::testing::Test {
 public:
  void SetUp() {}
  void TearDown() {}

 protected:
};

TEST_F(ProcessNVRamTest, ReadEmptyList) {
  const auto list = ""sv;
  const auto map = ProcessCustomClientList(list);

  ASSERT_EQ(0, map.size());
}

TEST_F(ProcessNVRamTest, ReadSingleValidElement) {
  const auto list = "<Client>11:22:33:44:55:66>0>0>>>>"sv;
  const auto map = ProcessCustomClientList(list);

  ASSERT_EQ(1, map.size());

  const auto &elem = map.begin()->second;
  EXPECT_EQ("Client", elem.Name());
  EXPECT_EQ("11:22:33:44:55:66", elem.MacAddr());
}

TEST_F(ProcessNVRamTest, ReadMultipleValidElement) {
  const auto list =
      "<Client>11:22:33:44:55:66>0>0>>>>"
      "<Other Client>12:34:56:78:9a:bc>0>0>>>>"
      "<Yet Another One>01:23:45:65:43:21>0>6>>>>"sv;
  const auto map = ProcessCustomClientList(list);

  ASSERT_EQ(3, map.size());

  auto iter = map.find(0x112233445566);
  ASSERT_NE(map.end(), iter);
  EXPECT_EQ("Client", iter->second.Name());

  iter = map.find(0x123456789abc);
  ASSERT_NE(map.end(), iter);
  EXPECT_EQ("Other Client", iter->second.Name());

  iter = map.find(0x012345654321);
  ASSERT_NE(map.end(), iter);
  EXPECT_EQ("Yet Another One", iter->second.Name());
}

TEST_F(ProcessNVRamTest, InvalidInput) {
  const auto list = "Test"sv;
  EXPECT_EQ(0, ProcessCustomClientList("Test").size());
  EXPECT_EQ(0, ProcessCustomClientList("<Test").size());
  EXPECT_EQ(0, ProcessCustomClientList("<Test>>").size());
  EXPECT_EQ(0, ProcessCustomClientList("<Test>>>>").size());
  EXPECT_EQ(0, ProcessCustomClientList("<Test>>>>>>").size());
  EXPECT_EQ(0, ProcessCustomClientList("<>>>>>>").size());
}

}  // namespace test
}  // namespace asus
