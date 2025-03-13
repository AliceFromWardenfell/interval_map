#include "IntervalMapTest.hpp"

using keyValueTypes = ::testing::Types<
	std::pair<int, int>,
	std::pair<int, char>,
	std::pair<int, float>,
	std::pair<int, std::vector<int>>,
	std::pair<int, std::string>
	>;

TYPED_TEST_CASE(IntervalMapTest, keyValueTypes);

TYPED_TEST(IntervalMapTest, DefaultValue)
{
	using K = typename TypeParam::first_type;
	using V = typename TypeParam::second_type;

	EXPECT_EQ(this->m_iMap[K{}], V{});
}

TEST(IntervalMapTest, BasicInsertion)
{
	DS::IntervalMap<int, std::string> iMap("DefaulValue");

	iMap.insert(2, 8, "TestValue");

	EXPECT_EQ(iMap[1], "DefaulValue");
	EXPECT_EQ(iMap[2], "TestValue");
	EXPECT_EQ(iMap[7], "TestValue");
	EXPECT_EQ(iMap[8], "DefaulValue");
}
