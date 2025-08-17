﻿#pragma once
#include "xx_space_.h"

namespace xx {

	template <typename T>
	struct Grid2dNode {
		int32_t next, prev;		// nodes index
		int32_t bucketsIndex;	// used by. -1 mean not use
		T value;				// pointer?
	};

	template <typename T, typename C>
	struct Grid2dCacheNode {
		int32_t next, prev;		// nodes index
		int32_t bucketsIndex;	// used by. -1 mean not use
		C cache;				// only support pod value
		T value;				// pointer?
	};

	template <typename Derived, typename T, typename C = void>
	struct Grid2dBase {
		using Node = std::conditional_t<std::is_void_v<C>, Grid2dNode<T>, Grid2dCacheNode<T, C>>;

		int32_t numRows{}, numCols{}, numCells{};
		int32_t freeHead{ -1 }, freeCount{}, count{}, capacity{};
		Node* nodes{};
		int32_t* buckets{};	// 2d array space. nodes index

		Grid2dBase() = default;
		Grid2dBase(Grid2dBase const& o) = delete;
		Grid2dBase& operator=(Grid2dBase const& o) = delete;

		XX_INLINE void Init(int32_t numRows_, int32_t numCols_, int32_t capacity_) {
			assert(!nodes && !buckets && numRows_ > 0 && numCols_ > 0 && capacity_ > 0);
			numRows = numRows_;
			numCols = numCols_;
			numCells = numRows_ * numCols_;
			capacity = capacity_;
			freeHead = -1;
			freeCount = count = 0;
			nodes = (Node*)new MyAlignedStorage<Node>[capacity_];
			buckets = new int32_t[numCells];
			memset(buckets, -1, numCells * sizeof(int32_t));
		}

		~Grid2dBase() {
			if (!buckets) return;
			Clear();
			delete[](MyAlignedStorage<Node>*)nodes;
			nodes = {};
			delete[]buckets;
			buckets = {};
		}

		void Clear() {
			assert(buckets);
			if (!count) return;
			for (int32_t i = 0; i < count; ++i) {
				auto& o = nodes[i];
				if (o.bucketsIndex != -1) {
					buckets[o.bucketsIndex] = -1;
					o.bucketsIndex = -1;
					o.value.~T();
				}
			}
			freeHead = -1;
			freeCount = 0;
			count = 0;
		}

		void Reserve(int32_t capacity_) {
			assert(buckets && capacity_ > 0);
			if (capacity_ <= capacity) return;
			auto newNodes = (Node*)new MyAlignedStorage<Node>[capacity_];
			if constexpr (IsPod_v<T>) {
				::memcpy((void*)newNodes, (void*)nodes, count * sizeof(Node));
			}
			else {
				for (int32_t i = 0; i < count; ++i) {
					new (&newNodes[i].value) T((T&&)nodes[i].value);
					nodes[i].value.T::~T();
				}
			}
			delete[](MyAlignedStorage<Node>*)nodes;
			nodes = newNodes;
			capacity = capacity_;
		}

		template<typename V>
		XX_INLINE int32_t Add(int32_t rowNumber_, int32_t columnNumber_, V&& v_) {
			assert(buckets);
			assert(rowNumber_ >= 0 && rowNumber_ < numRows);
			assert(columnNumber_ >= 0 && columnNumber_ < numCols);

			// alloc
			int32_t nodeIndex;
			if (freeCount > 0) {
				nodeIndex = freeHead;
				freeHead = nodes[nodeIndex].next;
				freeCount--;
			}
			else {
				if (count == capacity) {
					Reserve(count ? count * 2 : 16);
				}
				nodeIndex = count;
				count++;
			}
			auto& o = nodes[nodeIndex];

			// calc
			auto bucketsIndex = ToBucketsIndex(rowNumber_, columnNumber_);

			// link
			o.next = buckets[bucketsIndex];
			if (buckets[bucketsIndex] >= 0) {
				nodes[buckets[bucketsIndex]].prev = nodeIndex;
			}
			buckets[bucketsIndex] = nodeIndex;
			o.prev = -1;
			o.bucketsIndex = bucketsIndex;

			// assign
			new (&o.value) T(std::forward<V>(v_));
			return nodeIndex;
		}

		XX_INLINE void Remove(int32_t nodeIndex_) {
			assert(buckets);
			assert(nodeIndex_ >= 0 && nodeIndex_ < count && nodes[nodeIndex_].bucketsIndex != -1);
			auto& o = nodes[nodeIndex_];

			// unlink
			if (o.prev < 0) {
				buckets[o.bucketsIndex] = o.next;
			}
			else {
				nodes[o.prev].next = o.next;
			}
			if (o.next >= 0) {
				nodes[o.next].prev = o.prev;
			}

			// free
			o.next = freeHead;
			o.prev = -1;
			o.bucketsIndex = -1;
			freeHead = nodeIndex_;
			freeCount++;

			// cleanup
			o.value.~T();
		}

		XX_INLINE void Update(int32_t nodeIndex_, int32_t newRowNumber_, int32_t newColumnNumber_) {
			assert(buckets);
			assert(nodeIndex_ >= 0 && nodeIndex_ < count && nodes[nodeIndex_].bucketsIndex != -1);

			// calc
			auto newBucketsIndex = ToBucketsIndex(newRowNumber_, newColumnNumber_);
			auto& o = nodes[nodeIndex_];

			// no change check
			if (o.bucketsIndex == newBucketsIndex) return;

			// unlink
			if (o.prev < 0) {
				buckets[o.bucketsIndex] = o.next;
			}
			else {
				nodes[o.prev].next = o.next;
			}
			if (o.next >= 0) {
				nodes[o.next].prev = o.prev;
			}

			// link
			o.next = buckets[newBucketsIndex];
			if (buckets[newBucketsIndex] >= 0) {
				nodes[buckets[newBucketsIndex]].prev = nodeIndex_;
			}
			buckets[newBucketsIndex] = nodeIndex_;
			o.prev = -1;
			o.bucketsIndex = newBucketsIndex;
		}

		XX_INLINE Node& NodeAt(int32_t nodeIndex_) const {
			assert(buckets);
			assert(nodeIndex_ >= 0 && nodeIndex_ < count);
			assert(nodes[nodeIndex_].bucketsIndex != -1);
			return (Node&)nodes[nodeIndex_];
		}

		XX_INLINE int32_t Count() const {
			return count - freeCount;
		}

		XX_INLINE bool Empty() const {
			return Count() == 0;
		}

		XX_INLINE int32_t ToBucketsIndex(int32_t rowNumber_, int32_t columnNumber_) const {
			assert(rowNumber_ >= 0 && rowNumber_ < numRows);
			assert(columnNumber_ >= 0 && columnNumber_ < numCols);
			return rowNumber_ * numCols + columnNumber_;
		}


		/*******************************************************************************************/
		// derived functions

		// call by other ForeachXxxxxxxx func
		// .Foreach??????????([](decltype(grid)::Node& o, float range)->void {  all  });
		// .Foreach??????????([](decltype(grid)::Node& o, float range)->bool {  return false == break  });
		template <typename F, typename R = std::invoke_result_t<F, Node&, float>>
		XX_INLINE void ForeachCore(int32_t rowNumber_, int32_t columnNumber_, float range_, F& func) {
			if (rowNumber_ < 0 || rowNumber_ >= numRows || columnNumber_ < 0 || columnNumber_ >= numCols) return;
			auto ni = buckets[rowNumber_ * numCols + columnNumber_];
			while (ni != -1) {
				auto& n = nodes[ni];
				auto nex = n.next;
				if constexpr (std::is_void_v<R>) {
					func(n, range_);
				}
				else {
					if (func(n, range_)) return;
				}
				ni = nex;
			}
		}

		// foreach target + neighbors: 9 cells( range always == 0 )
		template <typename F, typename R = std::invoke_result_t<F, Node&, float>>
		void ForeachBy9(int32_t rowNumber_, int32_t columnNumber_, F&& func) {
			/* 4 */ForeachCore(rowNumber_, columnNumber_ - 1, 0, func);
			/* 5 */ForeachCore(rowNumber_, columnNumber_, 0, func);
			/* 6 */ForeachCore(rowNumber_, columnNumber_ + 1, 0, func);
			/* 1 */ForeachCore(rowNumber_ + 1, columnNumber_ - 1, 0, func);
			/* 2 */ForeachCore(rowNumber_ + 1, columnNumber_, 0, func);
			/* 3 */ForeachCore(rowNumber_ + 1, columnNumber_ + 1, 0, func);
			/* 7 */ForeachCore(rowNumber_ - 1, columnNumber_ - 1, 0, func);
			/* 8 */ForeachCore(rowNumber_ - 1, columnNumber_, 0, func);
			/* 9 */ForeachCore(rowNumber_ - 1, columnNumber_ + 1, 0, func);
		}

		// ring diffuse foreach ( usually for update logic or range search )
		template <typename F, typename R = std::invoke_result_t<F, Node&, float>>
		void ForeachByRange(int32_t rowNumber_, int32_t columnNumber_, int32_t searchRange, xx::SpaceGridRingDiffuseData const& rdd, F&& func) {
			auto scale = float(((Derived*)this)->cellSize) / rdd.cellSize;
			for (int32_t i = 1, e = rdd.lens.len; i < e; i++) {
				auto offsets = rdd.lens[i - 1].count;
				auto size = rdd.lens[i].count - rdd.lens[i - 1].count;
				auto range = rdd.lens[i].radius * scale;
				for (int32_t j = 0; j < size; ++j) {
					auto& tmp = rdd.idxs[offsets + j];
					ForeachCore(rowNumber_ + tmp.y, columnNumber_ + tmp.x, range, func);
				}
				if (range > searchRange) break;
			}
			assert(false);	// not enough rdd data?
		}

		// todo: more search

	};


	template<typename T, typename C = void>
	struct Grid2dCircle : xx::Grid2dBase<Grid2dCircle<T, C>, T, C> {
		using Base = xx::Grid2dBase<Grid2dCircle<T, C>, T, C>;
		using Base::Base;
		int32_t cellSize{};
		float _1_cellSize{};
		XYi pixelSize{};

		void Init(int32_t cellSize_, int32_t numRows_, int32_t numCols_, int32_t capacity_ = 0) {
			assert(cellSize_ > 0);
			Base::Init(numRows_, numCols_, capacity_);
			cellSize = cellSize_;
			_1_cellSize = 1.f / cellSize_;
			pixelSize = { cellSize_ * numCols_, cellSize_ * numRows_ };
		}

		template<typename V>
		void Add(int32_t& nodeIndex_, V&& e) {
			assert(nodeIndex_ == -1);
			auto cri = PosToCRIndex(e->pos);
			nodeIndex_ = Base::Add(cri.y, cri.x, std::forward<V>(e));
			auto& o = this->nodes[nodeIndex_];
			if constexpr (!std::is_void_v<C>) {
				o.cache = e;
			}
		}

		void Update(int32_t nodeIndex_, T const& e) {
			assert(Base::nodes[nodeIndex_].value == e);
			auto& o = Base::nodes[nodeIndex_];
			if constexpr (!std::is_void_v<C>) {
				o.cache = e;
			}
			auto cri = PosToCRIndex(e->pos);
			Base::Update(nodeIndex_, cri.y, cri.x);
		}

		void Remove(int32_t nodeIndex_, T const& e) {
			assert(Base::nodes[nodeIndex_].value == e);
			Base::Remove(nodeIndex_);
			nodeIndex_ = -1;
		}

		XX_INLINE XYi PosToCRIndex(XY p) const {
			return p * _1_cellSize;
		}

		XX_INLINE int32_t ToBucketsIndex(XY p) const {
			return Base::ToBucketsIndex(int32_t(p.y * _1_cellSize), int32_t(p.x * _1_cellSize));
		}

		XX_INLINE int32_t NodeIndexAt(XY p) const {
			return Base::buckets[ToBucketsIndex(p)];
		}
	};

	// todo: Grid2dBox ( size < cellsize )

}
