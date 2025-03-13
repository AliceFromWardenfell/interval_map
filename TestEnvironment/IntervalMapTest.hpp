#pragma once

#include <utility>

// STL for test cases
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "IntervalMap.hpp"

template <typename T>
class IntervalMapTest : public ::testing::Test
{
	public:

		IntervalMapTest() = default;

	protected:

		using K = typename T::first_type;
		using V = typename T::second_type;

	protected:

		void SetUp() override { m_iMap = DS::IntervalMap<K, V>(V{}); }

	protected:

		DS::IntervalMap<K, V> m_iMap;
};
