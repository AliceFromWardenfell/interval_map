#include "IntervalMapTest.hpp"

using mapTypes = ::testing::Types<
	std::pair<int, std::string>,
	std::pair<char, std::string>
	>;

TYPED_TEST_CASE(IntervalMapTest, mapTypes);

TYPED_TEST(IntervalMapTest, DefaultValue)
{
	using K = typename TypeParam::first_type;
	using V = typename TypeParam::second_type;

	EXPECT_EQ(this->m_iMap[K{}], V{});
}
