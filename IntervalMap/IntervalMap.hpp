#pragma once

#include <map>

//debug includes
#include <iostream>

namespace DS
{
	template <typename K, typename V>
	class IntervalMap // add commented docs on what is expected from K and V
	{
		public:

			IntervalMap() //implement 5 rule
			:
				IntervalMap(V{}) // No silent recovery. If K or V fails, we fail too
			{}

			IntervalMap(const V& defaultValue)
			:
				m_map{{std::numeric_limits<K>::lowest(), defaultValue}}
			{}

		public:

			void insert(const K& left, const K& right, const V& value)
			{
				std::cout << "Hello from insert" << std::endl;
			}

			const V& get(const K& key) const { return (--m_map.upper_bound(key))->second; }
			const V& operator[](const K& key) const { return (--m_map.upper_bound(key))->second; }

		private:

			std::map<K, V> m_map;
	};
}
