#pragma once
#include <chrono>
#include <random>
#include <cstdint>

namespace Comphi {

	constexpr const uint64_t PHI = 0x9e3779b97f4a7c15;

	class Random
	{
	public:
		static uint64_t generateUniqueID() {
			static std::mt19937_64 m_engine = std::mt19937_64(std::random_device()());
			static uint64_t m_counter = 0;
			
			// Get current timestamp since epoch in milliseconds
			auto now = std::chrono::system_clock::now();
			auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
			auto epoch_ms = now_ms.time_since_epoch();
			uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(epoch_ms).count();

			// Get a random number
			uint64_t random_num = m_engine();

			// Combine timestamp and random number to create a unique ID
			uint64_t unique_id = (timestamp << 32) | random_num;

			// Increment counter to ensure uniqueness even if generated in the same millisecond
			m_counter++;
			unique_id |= m_counter;

			return unique_id;
		}

		template<typename... Ts>
		static uint64_t hash_combine(const Ts&... values) {
			uint64_t hash = 0;
			(hash_combine(hash, values), ...);
			return hash;
		}

		template<typename T>
		static void hash_combine(uint64_t& seed, const T& value) {
			seed ^= std::hash<T>()(value) + PHI + (seed << 6) + (seed >> 2);
		}

	};
}