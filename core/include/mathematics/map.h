#pragma once

#include <engine/custom_allocator.h>
#include <vector> // TODO: replace this with neko's own container.
#include <xxhash.hpp>
#include <algorithm>

namespace neko
{

	template<typename Key, typename Value, const size_t Size>
	class FixedMap // 24 bytes
	{
	public:
		using Hash = xxh::hash_t<64>;
		using Pair = std::pair<Hash, Value>;

		FixedMap() {
			pairs_.resize(Size); // Initializes the pairs to 0.
		}

		~FixedMap() = default;

		[[nodiscard]] size_t Capacity() const {
			return pairs_.capacity();
		}

		Value& operator[](const Key key) {
			const Hash hash = xxh::xxhash<64>(&key, sizeof(Key), 42, xxh::endianness::littleEndian);
			auto it = std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair& p) { return p.first == hash; });

			neko_assert(it != pairs_.end(),
				"neko::FixedMap<Key,Value>::operator[](const Key): Key passed to operator not found.");
			return it->second;
		}

		bool Contains(const Key key) const {
			const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
			return std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair p) { return p.first == hash; }) != pairs_.end();
		}

		void Append(const Key key, const Value value) {
			neko_assert(!Contains(key),
				"neko::FixedMap<Key,Value>::Append(const Key, const Value): Map already contains Key passed.");
			auto it = std::find_if(pairs_.begin(), pairs_.end(), [](Pair& p) { return p.first == 0; });
			neko_assert(it != pairs_.end(),
				"neko::FixedMap<Key,Value>::Append(const Key, const Value): No more free slots in map.")
				it->first = xxh::xxhash<64>(&key, sizeof(Key), 42, xxh::endianness::littleEndian);
			it->second = value;
		}

		typename std::vector<Pair>::iterator begin() {
			return pairs_.begin();
		}

		typename std::vector<Pair>::iterator end() {
			return pairs_.end();
		}

		typename std::vector<Pair>::const_iterator cbegin() const {
			return pairs_.cbegin();
		}

		typename std::vector<Pair>::const_iterator cend() const {
			return pairs_.cend();
		}

	private:
		std::vector<Pair> pairs_; // 24 bytes // TODO: Replace this with neko's container when it's ready.
		// Allocator& allocator_;
	};

	template<typename Key, typename Value, const size_t Size>
	class SmallFixedMap
	{
	public:
		using Hash = xxh::hash_t<64>;
		using Pair = std::pair<Hash, Value>;

		SmallFixedMap(){
			pairs_.size(Size);
		}

		~SmallFixedMap() = default;

		[[nodiscard]] size_t Capacity() const{
			return pairs_.size();
		}

		Value& operator[](Key key){
			const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
			auto it = std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair& p) { return p.first == hash;  });

			neko_assert(it != pairs_.end(), "neko::SmallFixedMap<Key,Value>::operator[](const Key): Key passed to operator not found.");
			return it->second;
		}

		bool Contains(Key key) const{
			const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
			return std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair p) { return p.first == hash; }) != pairs_.end();
		}

		void Append(Key key, Value value){
			neko_assert(!Contains(key), "neko::SmallFixedMap<Key, Value>::Append(const Key, const Value): Map already contains Key passed.");
			auto it = std::find_if(pairs_.begin(), pairs_.end(), [](Pair& p) { return p.first == 0;  });
			neko_assert(it != pairs_.end(), "neko::Map<Key,Value>::Append(const Key, const Value): No more free slots in map.");
			it->first = xxh::xxhash<64>(&key, sizeof(Key));
			it->second = value;
		}

		typename std::array<Pair, Size>::iterator begin() {
			return pairs_.begin();
		}

		typename std::array<Pair, Size>::iterator end() {
			return pairs_.end();
		}

		typename std::array<Pair, Size>::const_iterator cbegin() const {
			return pairs_.cbegin();
		}

		typename std::array<Pair, Size>::const_iterator cend() const {
			return pairs_.cend();
		}
		
	private:
		std::array<Pair, 0> pairs_;
	};

	class DynamicMap
	{

	};

	class SmallDynamicMap
	{

	};

}// !neko