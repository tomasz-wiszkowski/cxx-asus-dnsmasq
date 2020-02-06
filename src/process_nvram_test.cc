#include "process_nvram.h"

#include <gtest/gtest.h>

#include <string>
#include <string_view>

using namespace std::literals;

namespace asus {
namespace test {

class ProcessNVRamTest : public ::testing::Test {};

TEST_F(ProcessNVRamTest, ReadEmptyList) {
  const auto list = ""sv;
  const auto map = ProcessCustomClientList(list);

  ASSERT_EQ(0, map.size());
}

TEST_F(ProcessNVRamTest, ReadSingleValidElement) {
  const auto list = "<Client>11:22:33:44:55:66>0>0>>>>"sv;
  const auto map = ProcessCustomClientList(list);

  ASSERT_EQ(1, map.size());

  const auto& elem = map.begin()->second;
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

class ExtractClientInfoTest : public ::testing::Test {};

TEST_F(ExtractClientInfoTest, EmptyString) {
  auto variant = ExtractClientInfo("");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidStartMarker, std::get<ParseResult>(variant));
}

TEST_F(ExtractClientInfoTest, BadStartMarker) {
  auto variant = ExtractClientInfo("Hello");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidStartMarker, std::get<ParseResult>(variant));
}

TEST_F(ExtractClientInfoTest, MissingEndMarker) {
  auto variant = ExtractClientInfo("<Hello");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidEndMarker, std::get<ParseResult>(variant));

  variant = ExtractClientInfo("<Hello>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidEndMarker, std::get<ParseResult>(variant));
}

TEST_F(ExtractClientInfoTest, MissingMacField) {
  auto variant = ExtractClientInfo("<Hello>>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::MissingFieldEndMarker, std::get<ParseResult>(variant));
}

TEST_F(ExtractClientInfoTest, MissingClientInfoField) {
  auto variant = ExtractClientInfo("<Hello>1>>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::MissingFieldEndMarker, std::get<ParseResult>(variant));
}

TEST_F(ExtractClientInfoTest, MissingMacAddress) {
  auto variant = ExtractClientInfo("<Hello>>>>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidDefinition, std::get<ParseResult>(variant));

  variant = ExtractClientInfo("<Hello>>2>>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidDefinition, std::get<ParseResult>(variant));
}

TEST_F(ExtractClientInfoTest, InvalidMacAddress) {
  auto variant = ExtractClientInfo("<Hello>MacAddress>>>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidDefinition, std::get<ParseResult>(variant));

  variant = ExtractClientInfo("<Hello>12345>>>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidDefinition, std::get<ParseResult>(variant));

  variant = ExtractClientInfo("<Hello>11:22:33:44:55>>>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidDefinition, std::get<ParseResult>(variant));

  variant = ExtractClientInfo("<Hello>11:22:33:44:55:>>>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidDefinition, std::get<ParseResult>(variant));

  variant = ExtractClientInfo("<Hello>11:22:33:44:55:6>>>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidDefinition, std::get<ParseResult>(variant));

  variant = ExtractClientInfo("<Hello>1:22:33:44:55:66>>>");
  ASSERT_TRUE(std::holds_alternative<ParseResult>(variant));
  ASSERT_EQ(ParseResult::InvalidDefinition, std::get<ParseResult>(variant));
}

TEST_F(ExtractClientInfoTest, ValidMacAddress) {
  auto variant = ExtractClientInfo("<Hello>13:57:9b:df:ec:a8>>>");
  ASSERT_TRUE(std::holds_alternative<HostInfo>(variant));

  const auto& host = std::get<HostInfo>(variant);
  EXPECT_EQ("Hello", host.Name());
  EXPECT_EQ("13:57:9b:df:ec:a8", host.MacAddr());
}

}  // namespace test
}  // namespace asus
