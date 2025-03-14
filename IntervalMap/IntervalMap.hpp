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

			//using iterator = typename std::map<K, V>::iterator;

			void insert(const K& left, const K& right, const V& value)
			{
				//std::pair<iterator, iterator> result{m_map.end(), m_map.end()};
				if (left >= right) return;

				auto itLeft = m_map.lower_bound(left);
				auto itRight = m_map.lower_bound(right);

				const V& prevRightValue = std::prev(itRight)->second;

				if (itLeft == m_map.begin() || std::prev(itLeft)->second != value)
				{
					auto [it, isInserted] = m_map.insert_or_assign(left, value); // fix one test
				}

				if (prevRightValue != value)
				{
					auto [it, isInserted] = m_map.insert_or_assign(right, prevRightValue);
				}

				//if (itLeft->first == left) ++itLeft;
				//if (itRight->first == left) ++itLeft;
				m_map.erase(m_map.upper_bound(left), m_map.lower_bound(right)); // get rid of upper bounds?

				//return result;
			}

			void printAsIntervals() const
			{
				auto itLast = --m_map.end();
				for (auto it = m_map.begin(); it != itLast; ++it)
				{
					auto next = std::next(it);
					std::cout << "[" << it->first << ", " << next->first << ") -> " << it->second << std::endl;
				}
				std::cout << "[" << itLast->first << ", " << std::numeric_limits<K>::max() << ") -> " << itLast->second << std::endl;
			}

			void printAsLine() const
			{
				for (const auto& [key, value] : m_map)
				{
					std::cout << key << " " << value << " ";
				}
				std::cout << std::numeric_limits<K>::max() << std::endl;
			}

			const V& get(const K& key) const { return (--m_map.upper_bound(key))->second; } // is it ok to use --?
			const V& operator[](const K& key) const { return (--m_map.upper_bound(key))->second; }

		private:

			std::map<K, V> m_map;
	};
}
