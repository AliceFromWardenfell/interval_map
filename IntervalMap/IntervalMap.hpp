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

			template<typename V_forward>
			IntervalMap(V_forward&& val)
			:
				m_valBegin(std::forward<V_forward>(val))
			{}

		public:

			template<typename V_forward>
			void insert(const K& keyBegin, const K& keyEnd, V_forward&& val)
			{
				if (!(keyBegin < keyEnd)) return;

				auto itBegin = m_map.lower_bound(keyBegin);
				auto itEnd = m_map.upper_bound(keyEnd);

				// Saving right overlaping value before map modifying (don't like it btw...)
				V prevEndVal = (itEnd != m_map.end())
					? std::prev(itEnd)->second
					: m_valBegin;

				const bool isSameValAsPrevEnd = (val == prevEndVal);
				const bool isSameValAsPrevBegin = (m_map.empty() || itBegin == m_map.begin())
					? val == m_valBegin
					: val == std::prev(itBegin)->second;

				// Left overlap handling
				if (!isSameValAsPrevBegin)
				{
					itBegin = std::next(m_map.insert_or_assign(itBegin, keyBegin, std::forward<V_forward>(val)));
				}

				// Right overlap handling
				if (!isSameValAsPrevEnd)
				{
					itEnd = m_map.insert_or_assign(itEnd, keyEnd, std::move(prevEndVal));
				}

				// Internal overlaps handling
				if (itBegin != m_map.end() &&
					(itEnd == m_map.end() || m_map.key_comp()(itBegin->first, itEnd->first)))
				{
					m_map.erase(itBegin, itEnd);
				}
			}

			void printAsIntervals() const
			{
				auto itLast = std::prev(m_map.end());
				for (auto it = m_map.begin(); it != itLast; ++it)
				{
					auto next = std::next(it);
					std::cout << "[" << it->first << ", " << next->first << ") -> " << it->second << std::endl;
				}
				std::cout << "[" << itLast->first << ", " << std::numeric_limits<K>::max() << ") -> " << itLast->second << std::endl;
			}

			void printAsLine() const
			{
				for (const auto& [key, val] : m_map)
				{
					std::cout << key << " " << val << " ";
				}
				std::cout << std::numeric_limits<K>::max() << std::endl;
			}

			V const& operator[](K const& key) const
			{
				auto it = m_map.upper_bound(key);
				if (it == m_map.begin())
				{
					return m_valBegin;
				}
				else
				{
					return (--it)->second;
				}
			}

			using MapType = std::map<K, V>;

			const MapType& getMap() const
			{
				return m_map;
			}

		private:

			V m_valBegin;

		public:

			std::map<K, V> m_map;
	};
}
