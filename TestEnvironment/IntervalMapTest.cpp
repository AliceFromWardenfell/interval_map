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
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(2, 8, "Custom");

	EXPECT_EQ(iMap[1], "Default");
	EXPECT_EQ(iMap[2], "Custom");
	EXPECT_EQ(iMap[7], "Custom");
	EXPECT_EQ(iMap[8], "Default");
}

TEST(IntervalMapTest, ExactOverlapInsertion)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(-100, 100, "Custom");
	iMap.insert(-100, 100, "Overlapped");

	EXPECT_EQ(iMap[-101], "Default");
	EXPECT_EQ(iMap[-100], "Overlapped");
	EXPECT_EQ(iMap[99], "Overlapped");
	EXPECT_EQ(iMap[100], "Default");

	iMap.printAsIntervals();
}

// Test overlapping intervals
TEST(IntervalMapTest, OverlappingIntervals)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 20, "B");
	iMap.insert(15, 25, "C");
	EXPECT_EQ(iMap[9], "Default");  // Before first interval
	EXPECT_EQ(iMap[10], "B"); // From first interval
	EXPECT_EQ(iMap[14], "B"); // Still in first interval
	EXPECT_EQ(iMap[15], "C"); // Overwritten by second interval
	EXPECT_EQ(iMap[24], "C"); // End of second interval
	EXPECT_EQ(iMap[25], "Default"); // After second interval
}

// Test inserting a nested interval
TEST(IntervalMapTest, NestedIntervals)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 30, "B");
	iMap.insert(15, 25, "C");
	EXPECT_EQ(iMap[9], "Default");   // Before any interval
	EXPECT_EQ(iMap[10], "B");  // Start of outer interval
	EXPECT_EQ(iMap[14], "B");  // Before nested interval
	EXPECT_EQ(iMap[15], "C");  // Nested interval starts
	EXPECT_EQ(iMap[24], "C");  // Inside nested interval
	EXPECT_EQ(iMap[25], "B");  // Nested interval ends, outer continues
	EXPECT_EQ(iMap[30], "Default");  // Outer interval ends
}

// Test inserting an interval that spans across an existing one
TEST(IntervalMapTest, SpanningInterval)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 20, "B");
	iMap.insert(5, 25, "C");
	EXPECT_EQ(iMap[4], "Default");  // Before new interval
	EXPECT_EQ(iMap[5], "C");  // New interval starts
	EXPECT_EQ(iMap[9], "C");  // Overwrites previous "Default" values
	EXPECT_EQ(iMap[10], "C"); // Overwrites previous "B" values
	EXPECT_EQ(iMap[24], "C"); // Stretches beyond previous range
	EXPECT_EQ(iMap[25], "Default"); // After interval
}

// Test edge case: inserting an empty interval (start == end)
TEST(IntervalMapTest, EmptyInterval)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 10, "B");
	EXPECT_EQ(iMap[9], "Default");  // Should remain default
	EXPECT_EQ(iMap[10], "Default"); // No effect
	EXPECT_EQ(iMap[11], "Default"); // No effect
}

// Test assigning multiple disjoint intervals
TEST(IntervalMapTest, MultipleDisjointIntervals)
{
	DS::IntervalMap<int, std::string> iMap("A");

	iMap.insert(5, 10, "B");
	iMap.insert(15, 20, "C");
	EXPECT_EQ(iMap[4], "A");  // Before first interval
	EXPECT_EQ(iMap[5], "B");  // Start of first interval
	EXPECT_EQ(iMap[9], "B");  // End of first interval
	EXPECT_EQ(iMap[10], "A"); // Gap between intervals
	EXPECT_EQ(iMap[15], "C"); // Start of second interval
	EXPECT_EQ(iMap[19], "C"); // End of second interval
	EXPECT_EQ(iMap[20], "A"); // After second interval
}

// Test clearing an interval by reassigning default value
TEST(IntervalMapTest, ClearInterval)
{
	DS::IntervalMap<int, std::string> iMap("A");

	iMap.insert(10, 20, "B");
	iMap.insert(10, 20, "A");
	EXPECT_EQ(iMap[9], "A");  // Before cleared interval
	EXPECT_EQ(iMap[10], "A"); // Cleared interval should return to default
	EXPECT_EQ(iMap[19], "A"); // Still default
	EXPECT_EQ(iMap[20], "A"); // Outside original interval

	iMap.printAsIntervals();
}

// Test assigning interval with same value as default (should have no effect)
TEST(IntervalMapTest, AssignDefaultValue)
{
	DS::IntervalMap<int, std::string> iMap("A");

	iMap.insert(10, 20, "A");
	EXPECT_EQ(iMap[9], "A");
	EXPECT_EQ(iMap[10], "A"); // Should be unchanged
	EXPECT_EQ(iMap[20], "A");
}
