#pragma once

#include <map>

namespace DS
{
	template <typename K, typename V>
	class IntervalMap
	{
		public:

			IntervalMap() = default;

			IntervalMap(V const& value)
			{
				m_map.insert(m_map.end(), { std::numeric_limits<K>::lowest(), value });
			}

		public:

			V const& operator[](K const& key) const { return (--m_map.upper_bound(key))->second; }

		private:

			std::map<K, V> m_map;
	};
}
