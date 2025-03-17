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

// Test for adjacent intervals: intervals that touch but do not overlap.
TEST(IntervalMapTest, AdjacentIntervals)
{
	DS::IntervalMap<int, std::string> iMap("Default");
	iMap.insert(10, 20, "A");
	iMap.insert(20, 30, "B");

	EXPECT_EQ(iMap[9], "Default");   // Before first interval
	EXPECT_EQ(iMap[10], "A");          // Start of first interval
	EXPECT_EQ(iMap[19], "A");          // End of first interval
	EXPECT_EQ(iMap[20], "B");          // Start of second interval
	EXPECT_EQ(iMap[29], "B");          // End of second interval
	EXPECT_EQ(iMap[30], "Default");    // After second interval
}

// Test inserting an interval that covers nearly the entire range of int values.
// Note: The interval is [min, max) so the key equal to max should return default.
TEST(IntervalMapTest, FullRangeInterval)
{
	DS::IntervalMap<int, std::string> iMap("Default");
	iMap.insert(std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), "Full");

	EXPECT_EQ(iMap[std::numeric_limits<int>::min()], "Full");
	EXPECT_EQ(iMap[0], "Full");
	EXPECT_EQ(iMap[std::numeric_limits<int>::max() - 1], "Full");
	EXPECT_EQ(iMap[std::numeric_limits<int>::max()], "Default");
}

// Test overlapping intervals with the same starting boundary.
TEST(IntervalMapTest, OverlappingSameStart)
{
	DS::IntervalMap<int, std::string> iMap("Default");
	iMap.insert(10, 20, "A");
	iMap.insert(10, 15, "B");

	EXPECT_EQ(iMap[9], "Default");
	EXPECT_EQ(iMap[10], "B");  // New interval starts at 10
	EXPECT_EQ(iMap[14], "B");  // Still within the overwritten part
	EXPECT_EQ(iMap[15], "A");  // Remainder of the first interval
	EXPECT_EQ(iMap[19], "A");
	EXPECT_EQ(iMap[20], "Default");
}

// Test multiple overlapping intervals inserted sequentially.
TEST(IntervalMapTest, MultipleOverlappingIntervals)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 30, "A");
	iMap.insert(20, 40, "B");
	iMap.insert(25, 35, "C");
	// Expected segments:
	// [10,20) : "A"
	// [20,25) : "B"
	// [25,35) : "C"
	// [35,40) : "B"
	// Else: "Default"
	EXPECT_EQ(iMap[9], "Default");
	EXPECT_EQ(iMap[10], "A");
	EXPECT_EQ(iMap[19], "A");
	EXPECT_EQ(iMap[20], "B");
	EXPECT_EQ(iMap[24], "B");
	EXPECT_EQ(iMap[25], "C");
	EXPECT_EQ(iMap[34], "C");
	EXPECT_EQ(iMap[35], "B");
	EXPECT_EQ(iMap[39], "B");
	EXPECT_EQ(iMap[40], "Default");
}

// Test multiple insertions where a subsequent insertion reassigns part of an interval back to the default value.
TEST(IntervalMapTest, MultipleInsertionsAndReversions)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 20, "A");
	iMap.insert(15, 25, "B");
	// Clear a middle segment by reassigning the default value.
	iMap.insert(12, 18, "Default");

	// Expected segments:
	// [10,12) : "A"
	// [12,18) : "Default" (cleared region)
	// [18,25) : "B"
	EXPECT_EQ(iMap[9], "Default");
	EXPECT_EQ(iMap[10], "A");
	EXPECT_EQ(iMap[11], "A");
	EXPECT_EQ(iMap[12], "Default");
	EXPECT_EQ(iMap[14], "Default");
	EXPECT_EQ(iMap[15], "Default");
	EXPECT_EQ(iMap[17], "Default");
	EXPECT_EQ(iMap[18], "B");
	EXPECT_EQ(iMap[24], "B");
	EXPECT_EQ(iMap[25], "Default");
}

// Test that an IntervalMap with no insertions returns the default value for any key.
TEST(IntervalMapTest, EmptyMapInitialState)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Without any insertions, every key should yield the default value.
	EXPECT_EQ(iMap[-100], "Default");
	EXPECT_EQ(iMap[0], "Default");
	EXPECT_EQ(iMap[100], "Default");
}

// Test inserting into an empty internal map.
// After the first insertion, the correct segments should be set.
TEST(IntervalMapTest, InsertIntoEmptyMap)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// m_map is initially empty; inserting an interval should update it.
	iMap.insert(50, 100, "Custom");

	// Keys outside the inserted interval should still return "Default".
	EXPECT_EQ(iMap[49], "Default");
	// Keys within the inserted range should return "Custom".
	EXPECT_EQ(iMap[50], "Custom");
	EXPECT_EQ(iMap[75], "Custom");
	EXPECT_EQ(iMap[99], "Custom");
	// The key at the end of the interval should revert to "Default".
	EXPECT_EQ(iMap[100], "Default");
}

// Test that inserting an interval with the same value as the default into an empty map has no effect.
TEST(IntervalMapTest, InsertDefaultValueIntoEmptyMap)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Attempt to insert an interval with the default value.
	iMap.insert(50, 100, "Default");

	// All keys should still yield the default value.
	EXPECT_EQ(iMap[49], "Default");
	EXPECT_EQ(iMap[50], "Default");
	EXPECT_EQ(iMap[75], "Default");
	EXPECT_EQ(iMap[99], "Default");
	EXPECT_EQ(iMap[100], "Default");
}

// Test inserting into an empty map
TEST(IntervalMapTest, InsertIntoEmptyMap2)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert the first interval into an empty map
	iMap.insert(5, 15, "First");

	EXPECT_EQ(iMap[4], "Default");  // Before the interval
	EXPECT_EQ(iMap[5], "First");    // Start of the interval
	EXPECT_EQ(iMap[10], "First");   // Inside the interval
	EXPECT_EQ(iMap[14], "First");   // End of the interval
	EXPECT_EQ(iMap[15], "Default"); // After the interval
}

// Test inserting a single interval that covers the entire range of int values into an empty map
TEST(IntervalMapTest, InsertFullRangeIntoEmptyMap)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert an interval that covers the entire range of int values
	iMap.insert(std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), "Full");

	EXPECT_EQ(iMap[std::numeric_limits<int>::min()], "Full");       // Start of the interval
	EXPECT_EQ(iMap[0], "Full");                                    // Inside the interval
	EXPECT_EQ(iMap[std::numeric_limits<int>::max() - 1], "Full");  // End of the interval
	EXPECT_EQ(iMap[std::numeric_limits<int>::max()], "Default");   // After the interval
}

// Test inserting an interval with the same start and end into an empty map
TEST(IntervalMapTest, InsertEmptyIntervalIntoEmptyMap)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert an empty interval (start == end)
	iMap.insert(10, 10, "Empty");

	EXPECT_EQ(iMap[9], "Default");  // Before the interval
	EXPECT_EQ(iMap[10], "Default"); // At the interval (should have no effect)
	EXPECT_EQ(iMap[11], "Default"); // After the interval
}

// Test inserting an interval that starts and ends at the same boundary into an empty map
TEST(IntervalMapTest, InsertSinglePointIntervalIntoEmptyMap)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert an interval that starts and ends at the same boundary
	iMap.insert(10, 11, "SinglePoint");

	EXPECT_EQ(iMap[9], "Default");       // Before the interval
	EXPECT_EQ(iMap[10], "SinglePoint");  // Start of the interval
	EXPECT_EQ(iMap[11], "Default");      // After the interval
}

// Test inserting an interval that starts at the minimum int value into an empty map
TEST(IntervalMapTest, InsertMinValueIntervalIntoEmptyMap)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert an interval that starts at the minimum int value
	iMap.insert(std::numeric_limits<int>::min(), 0, "MinToZero");

	EXPECT_EQ(iMap[std::numeric_limits<int>::min()], "MinToZero");  // Start of the interval
	EXPECT_EQ(iMap[-1], "MinToZero");                               // Inside the interval
	EXPECT_EQ(iMap[0], "Default");                                  // After the interval
}

// Test inserting an interval that ends at the maximum int value into an empty map
TEST(IntervalMapTest, InsertMaxValueIntervalIntoEmptyMap)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert an interval that ends at the maximum int value
	iMap.insert(0, std::numeric_limits<int>::max(), "ZeroToMax");

	EXPECT_EQ(iMap[-1], "Default");                                  // Before the interval
	EXPECT_EQ(iMap[0], "ZeroToMax");                                 // Start of the interval
	EXPECT_EQ(iMap[std::numeric_limits<int>::max() - 1], "ZeroToMax"); // Inside the interval
	EXPECT_EQ(iMap[std::numeric_limits<int>::max()], "Default");     // After the interval
}

// Test inserting an interval that starts and ends at the same boundary (minimum int value) into an empty map
TEST(IntervalMapTest, InsertMinValueSinglePointIntervalIntoEmptyMap)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert an interval that starts and ends at the minimum int value
	iMap.insert(std::numeric_limits<int>::min(), std::numeric_limits<int>::min() + 1, "MinPoint");

	EXPECT_EQ(iMap[std::numeric_limits<int>::min() - 1], "Default"); // Before the interval
	EXPECT_EQ(iMap[std::numeric_limits<int>::min()], "MinPoint");    // Start of the interval
	EXPECT_EQ(iMap[std::numeric_limits<int>::min() + 1], "Default"); // After the interval
}

// Test inserting an interval that starts and ends at the same boundary (maximum int value) into an empty map
TEST(IntervalMapTest, InsertMaxValueSinglePointIntervalIntoEmptyMap)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert an interval that starts and ends at the maximum int value
	iMap.insert(std::numeric_limits<int>::max() - 1, std::numeric_limits<int>::max(), "MaxPoint");

	EXPECT_EQ(iMap[std::numeric_limits<int>::max() - 2], "Default"); // Before the interval
	EXPECT_EQ(iMap[std::numeric_limits<int>::max() - 1], "MaxPoint"); // Start of the interval
	EXPECT_EQ(iMap[std::numeric_limits<int>::max()], "Default");     // After the interval
}

// Test that a newly constructed IntervalMap is in canonical form.
TEST(IntervalMapTest, CanonicalFormInitial)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	const auto& internalMap = iMap.getMap();
	// For canonical form, no two consecutive map entries should have the same value.
	for (auto it = internalMap.begin(); it != internalMap.end(); ++it) {
		auto next = std::next(it);
		if (next != internalMap.end()) {
			EXPECT_NE(it->second, next->second);
		}
	}
}

// Test that after a single insertion into an empty map the canonical form is preserved.
TEST(IntervalMapTest, CanonicalFormAfterSingleInsertion)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert an interval that modifies a segment.
	iMap.insert(50, 100, "Custom");

	const auto& internalMap = iMap.getMap();
	// Check that adjacent entries do not have the same value.
	for (auto it = internalMap.begin(); it != internalMap.end(); ++it) {
		auto next = std::next(it);
		if (next != internalMap.end()) {
			EXPECT_NE(it->second, next->second);
		}
	}
}

// Test that after multiple overlapping insertions the canonical form is maintained.
TEST(IntervalMapTest, CanonicalFormAfterMultipleOverlaps)
{
	DS::IntervalMap<int, std::string> iMap("A");

	// Create several overlapping intervals that could lead to redundant boundaries if not merged.
	iMap.printAsLine();
	iMap.insert(10, 30, "B");
	iMap.printAsLine();
	iMap.insert(20, 40, "C");
	iMap.printAsLine();
	iMap.insert(25, 35, "D");
	iMap.printAsLine();
	iMap.insert(15, 45, "A"); // Reinserting the default over a segment
	iMap.printAsLine();

	const auto& internalMap = iMap.getMap();
	// Validate the canonical property: adjacent entries must have different values.
	for (auto it = internalMap.begin(); it != internalMap.end(); ++it) {
		auto next = std::next(it);
		if (next != internalMap.end()) {
			EXPECT_NE(it->second, next->second);
		}
	}
}

// Test if the map is in canonical form after a single insertion
TEST(IntervalMapTest, CanonicalFormAfterSingleInsertion2)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 20, "A");

	// Check if the map has only one interval
	EXPECT_EQ(iMap.m_map.size(), 2); // Only two boundaries: 10 and 20
	EXPECT_EQ(iMap[9], "Default");
	EXPECT_EQ(iMap[10], "A");
	EXPECT_EQ(iMap[19], "A");
	EXPECT_EQ(iMap[20], "Default");
}

// Test if the map is in canonical form after inserting adjacent intervals with the same value
TEST(IntervalMapTest, CanonicalFormAfterAdjacentInsertionsSameValue)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 20, "A");
	iMap.insert(20, 30, "A"); // Adjacent interval with the same value

	// Check if the intervals are merged into one
	EXPECT_EQ(iMap.m_map.size(), 2); // Only two boundaries: 10 and 30
	EXPECT_EQ(iMap[9], "Default");
	EXPECT_EQ(iMap[10], "A");
	EXPECT_EQ(iMap[29], "A");
	EXPECT_EQ(iMap[30], "Default");
}

// Test if the map is in canonical form after inserting overlapping intervals with the same value
TEST(IntervalMapTest, CanonicalFormAfterOverlappingInsertionsSameValue)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 20, "A");
	iMap.insert(15, 25, "A"); // Overlapping interval with the same value

	// Check if the intervals are merged into one
	EXPECT_EQ(iMap.m_map.size(), 2); // Only two boundaries: 10 and 25
	EXPECT_EQ(iMap[9], "Default");
	EXPECT_EQ(iMap[10], "A");
	EXPECT_EQ(iMap[24], "A");
	EXPECT_EQ(iMap[25], "Default");
}

// Test if the map is in canonical form after inserting intervals that revert to the default value
TEST(IntervalMapTest, CanonicalFormAfterRevertingToDefault)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 20, "A");
	iMap.insert(15, 25, "Default"); // Revert part of the interval to default

	// Check if the map is in canonical form
	EXPECT_EQ(iMap.m_map.size(), 2); // Only two boundaries: 10 and 15
	EXPECT_EQ(iMap[9], "Default");
	EXPECT_EQ(iMap[10], "A");
	EXPECT_EQ(iMap[14], "A");
	EXPECT_EQ(iMap[15], "Default");
	EXPECT_EQ(iMap[25], "Default");
}

// Test if the map is in canonical form after inserting an interval that spans multiple existing intervals
TEST(IntervalMapTest, CanonicalFormAfterSpanningInsertion)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 20, "A");
	iMap.insert(30, 40, "B");
	iMap.insert(5, 35, "C"); // Spanning interval

	// Check if the map is in canonical form
	EXPECT_EQ(iMap.m_map.size(), 3); // Boundaries: 5, 35, 40
	EXPECT_EQ(iMap[4], "Default");
	EXPECT_EQ(iMap[5], "C");
	EXPECT_EQ(iMap[34], "C");
	EXPECT_EQ(iMap[35], "B");
	EXPECT_EQ(iMap[39], "B");
	EXPECT_EQ(iMap[40], "Default");
}

// Test if the map is in canonical form after inserting an interval that covers the entire range
TEST(IntervalMapTest, CanonicalFormAfterFullRangeInsertion)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), "Full");

	// Check if the map is in canonical form
	EXPECT_EQ(iMap.m_map.size(), 2); // Only two boundaries: min and max
	EXPECT_EQ(iMap[std::numeric_limits<int>::min()], "Full");
	EXPECT_EQ(iMap[0], "Full");
	EXPECT_EQ(iMap[std::numeric_limits<int>::max() - 1], "Full");
	EXPECT_EQ(iMap[std::numeric_limits<int>::max()], "Default");
}

// Test if the map is in canonical form after inserting an empty interval
TEST(IntervalMapTest, CanonicalFormAfterEmptyIntervalInsertion)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 10, "A"); // Empty interval

	// Check if the map is in canonical form
	//EXPECT_EQ(iMap.m_map.size(), 1); // Only the default value exists
	EXPECT_EQ(iMap[9], "Default");
	EXPECT_EQ(iMap[10], "Default");
	EXPECT_EQ(iMap[11], "Default");
}

// Test if the map is in canonical form after inserting multiple intervals and reverting some to default
TEST(IntervalMapTest, CanonicalFormAfterMultipleInsertionsAndRevertions)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	iMap.insert(10, 20, "A");
	iMap.insert(15, 25, "B");
	iMap.insert(12, 18, "Default"); // Revert part of the interval to default

	// Check if the map is in canonical form
	EXPECT_EQ(iMap.m_map.size(), 4); // Boundaries: 10, 12, 18, 25
	EXPECT_EQ(iMap[9], "Default");
	EXPECT_EQ(iMap[10], "A");
	EXPECT_EQ(iMap[11], "A");
	EXPECT_EQ(iMap[12], "Default");
	EXPECT_EQ(iMap[17], "Default");
	EXPECT_EQ(iMap[18], "B");
	EXPECT_EQ(iMap[24], "B");
	EXPECT_EQ(iMap[25], "Default");
}

// -----------------------------------------------------------------------------
// Test 1: Right Boundary Lookup Issues
// This test verifies that when inserting an interval whose keyEnd is less than
// the first boundary in m_map, no invalid iterator is dereferenced.
// For example, after an interval insertion starting at 5, inserting one with keyEnd < 5
// should work correctly.
TEST(IntervalMapTest, RightBoundaryLookup)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert an interval so that m_map gets an entry (at key 5)
	iMap.insert(5, 10, "A");

	// Now insert an interval with keyEnd less than the first boundary in m_map.
	// keyEnd = 0 will cause upper_bound(0) to return m_map.begin() (i.e. iterator to key 5).
	// The test checks that no crash occurs and the values are correct.
	iMap.insert(-10, 0, "B");

	// Expected outcomes:
	// Keys less than -10 remain "Default"
	// Keys in [-10, 0) become "B"
	// Keys [0, 5) are still "Default" (since the new interval ended at 0)
	// Keys from 5 onward reflect the earlier insertion
	EXPECT_EQ(iMap[-11], "Default");
	EXPECT_EQ(iMap[-10], "B");
	EXPECT_EQ(iMap[-1], "B");
	EXPECT_EQ(iMap[0], "Default");
	EXPECT_EQ(iMap[4], "Default");
	EXPECT_EQ(iMap[5], "A");
	EXPECT_EQ(iMap[9], "A");
	EXPECT_EQ(iMap[10], "Default");
}

// Test 2: Canonical Representation (No Redundant Boundaries)
// This test checks that the internal m_map remains canonical after assignments.
// Specifically, inserting an interval with a value that is already in effect should not
// add unnecessary boundaries.
TEST(IntervalMapTest, CanonicalRepresentation)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Initially, m_map should be empty
	EXPECT_TRUE(iMap.m_map.empty());

	// Insert an interval that changes the value
	iMap.insert(10, 20, "Custom");
	// For a canonical representation, we expect boundaries at 10 and 20 only.
	EXPECT_EQ(iMap.m_map.size(), 2);

	// Inserting the same interval with the same value should not add new boundaries.
	iMap.insert(10, 20, "Custom");
	EXPECT_EQ(iMap.m_map.size(), 2);
}

// Test 3: Iterator Hint Consistency Efficiency
// Although we cannot directly verify the use of iterator hints,
// this test performs several insertions (in non-sorted order) and checks that
// the overall interval mapping is as expected.
TEST(IntervalMapTest, IteratorHintsConsistency)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert intervals in an order that challenges internal iterator hints
	iMap.insert(30, 40, "X");
	iMap.insert(10, 20, "Y");
	iMap.insert(20, 30, "Z");

	// Expected intervals:
	// (,10): "Default"
	// [10,20): "Y"
	// [20,30): "Z"
	// [30,40): "X"
	// [40,): "Default"
	EXPECT_EQ(iMap[5], "Default");
	EXPECT_EQ(iMap[10], "Y");
	EXPECT_EQ(iMap[19], "Y");
	EXPECT_EQ(iMap[20], "Z");
	EXPECT_EQ(iMap[29], "Z");
	EXPECT_EQ(iMap[30], "X");
	EXPECT_EQ(iMap[39], "X");
	EXPECT_EQ(iMap[40], "Default");
}

// -----------------------------------------------------------------------------
// Test 4: Off-By-One and Boundary Touching Cases
// This test verifies that intervals touching at their boundaries are handled
// correctly without redundant boundaries. It also checks that an insertion that
// does not change the value is correctly ignored.
TEST(IntervalMapTest, OffByOneBoundaries)
{
	DS::IntervalMap<int, std::string> iMap("Default");

	// Insert two adjacent intervals.
	iMap.insert(10, 20, "A");
	iMap.insert(20, 30, "B");

	// Expected:
	// (,10): "Default"
	// [10,20): "A"
	// [20,30): "B"
	// [30,): "Default"
	EXPECT_EQ(iMap[9], "Default");
	EXPECT_EQ(iMap[10], "A");
	EXPECT_EQ(iMap[19], "A");
	EXPECT_EQ(iMap[20], "B");
	EXPECT_EQ(iMap[29], "B");
	EXPECT_EQ(iMap[30], "Default");

	// Now test inserting an interval that touches an existing boundary with the same value.
	// For instance, insert an interval [5,15) with "Default" which is the value in effect before key 10.
	// This should not create a new boundary if handled canonically.
	iMap.printAsLine();
	iMap.insert(5, 15, "Default");
	iMap.printAsLine();
	// Check that the value from 5 to 10 remains "Default" and from 10 onward is still "A"
	// (since the new interval did not change the value in its region).
	EXPECT_EQ(iMap[7], "Default");
	EXPECT_EQ(iMap[10], "Default");
	EXPECT_EQ(iMap[14], "Default");
	EXPECT_EQ(iMap[15], "A");
}
