#pragma once
#include "xx_space_.h"

namespace xx {

	struct Grid2dAABBNodeInfo {
		int32_t nodesIndex, indexAtInfos, prev, next;
	};

	template <typename T>
	struct Grid2dAABBNode {
		int32_t next, inResults;
		FromTo<XY> aabb;
		FromTo<XYi> ccrr;
		List<Grid2dAABBNodeInfo> infos;
		T value;				// pointer?
	};

	template <typename T>
	struct Grid2dAABB {
		using Node = Grid2dAABBNode<T>;

		XYi gridSize{};
		XY cellSize{}, _1_cellSize{}, worldSize{};
		//int32_t numCells{};
		int32_t freeHead{ -1 }, freeCount{}, count{}, capacity{};
		Node* nodes{};
		int32_t* cells{};	// 2d array space. nodes index
		List<int32_t> results;

		Grid2dAABB() = default;
		Grid2dAABB(Grid2dAABB const& o) = delete;
		Grid2dAABB& operator=(Grid2dAABB const& o) = delete;

		XX_INLINE void Init(XYi gridSize_, float cellSize_, int32_t capacity_) {
			assert(!nodes && !cells && capacity_ > 0);
			assert(gridSize.x > 0 && gridSize.y > 0);
			assert(cellSize_.x > 0 && cellSize_.y > 0);
			gridSize = gridSize_;
			cellSize = cellSize_;
			_1_cellSize = 1.f / cellSize_;
			worldSize = cellSize * gridSize;
			freeHead = -1;
			freeCount = count = 0;
			capacity = capacity_;
			nodes = (Node*)new MyAlignedStorage<Node>[capacity_];
			auto numCells = gridSize_.x * gridSize_.y;
			cells = new int32_t[numCells];
			memset(cells, -1, numCells * sizeof(int32_t));
		}

		~Grid2dAABB() {
			if (!cells) return;
			Clear(false);
			delete[](MyAlignedStorage<Node>*)nodes;
			nodes = {};
			delete[]cells;
			cells = {};
		}

		void Clear(bool doMemset = true) {
			assert(cells);
			if (!count) return;
			for (int32_t i = 0; i < count; ++i) {
				auto& o = nodes[i];
				if (o.next == -1) {
					std::destroy_at(&o);
				}
			}
			if (doMemset) {
				memset(cells, -1, numCells * sizeof(int32_t));
			}
			freeHead = -1;
			freeCount = 0;
			count = 0;
		}

		void Reserve(int32_t capacity_) {
			assert(cells && capacity_ > 0);
			if (capacity_ <= capacity) return;
			auto newNodes = (Node*)new MyAlignedStorage<Node>[capacity_];
			if constexpr (IsPod_v<T>) {
				::memcpy((void*)newNodes, (void*)nodes, count * sizeof(Node));
			}
			else {
				for (int32_t i = 0; i < count; ++i) {
					new (&newNodes[i]) Node((Node&&)nodes[i]);
					std::destroy_at(&nodes[i]);
				}
			}
			delete[](MyAlignedStorage<Node>*)nodes;
			nodes = newNodes;
			capacity = capacity_;
		}

		template<typename V>
		XX_INLINE int32_t Add(FromTo<XY> const& aabb, V&& v_) {
			assert(cells);
			assert(aabb.from.x < aabb.to.x && aabb.from.y < aabb.to.y);
			assert(aabb.from.x >= 0 && aabb.from.x < worldSize.x);
			assert(aabb.from.y >= 0 && aabb.from.y < worldSize.y);
			assert(aabb.to.x >= 0 && aabb.to.x < worldSize.x);
			assert(aabb.to.y >= 0 && aabb.to.y < worldSize.y);

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
			std::construct_at(&o);
			o.value = std::forward<V>(v_);

			// calc covered cells
			o.biFrom = (aabb.from * _1_cellSize).As<int32_t>();
			o.biTo = (aabb.to * _1_cellSize).As<int32_t>();
			auto numMaxCoveredCells = (biTo.x - biFrom.x + 2) * (biTo.y - biFrom.y + 2);

			// link
			auto& infos = o.infos;
			infos.Reserve(numMaxCoveredCells);
			for (auto y = o.biFrom.y; y <= o.biTo.y; y++) {
				for (auto x = o.biFrom.x; x <= o.biTo.x; x++) {
					auto bi = y * gridSize.x + x;
					//auto ci = &infos.emplace_back(ItemCellInfo{ c, bi, nullptr, cells[bi] });
					//if (cells[bi] != -1) {
					//	cells[bi]->prev = ci;
					//}
					//cells[bi] = ci;
				}
			}

			return nodeIndex;
		}

		XX_INLINE void Remove(int32_t nodeIndex_) {
			assert(cells);
			assert(nodeIndex_ >= 0 && nodeIndex_ < count && nodes[nodeIndex_].cellsIndex != -1);
			auto& o = nodes[nodeIndex_];

			//// unlink
			//if (o.prev < 0) {
			//	cells[o.cellsIndex] = o.next;
			//}
			//else {
			//	nodes[o.prev].next = o.next;
			//}
			//if (o.next >= 0) {
			//	nodes[o.next].prev = o.prev;
			//}

			// free
			o.next = freeHead;
			o.prev = -1;
			o.cellsIndex = -1;
			freeHead = nodeIndex_;
			freeCount++;

			// cleanup
			std::destroy_at(&o);
		}

		XX_INLINE void Update(int32_t nodeIndex_, int32_t newRowNumber_, int32_t newColumnNumber_) {
			assert(cells);
			assert(nodeIndex_ >= 0 && nodeIndex_ < count && nodes[nodeIndex_].cellsIndex != -1);

			//// calc
			//auto newBucketsIndex = ToBucketsIndex(newRowNumber_, newColumnNumber_);
			//auto& o = nodes[nodeIndex_];

			//// no change check
			//if (o.cellsIndex == newBucketsIndex) return;

			//// unlink
			//if (o.prev < 0) {
			//	cells[o.cellsIndex] = o.next;
			//}
			//else {
			//	nodes[o.prev].next = o.next;
			//}
			//if (o.next >= 0) {
			//	nodes[o.next].prev = o.prev;
			//}

			//// link
			//o.next = cells[newBucketsIndex];
			//if (cells[newBucketsIndex] >= 0) {
			//	nodes[cells[newBucketsIndex]].prev = nodeIndex_;
			//}
			//cells[newBucketsIndex] = nodeIndex_;
			//o.prev = -1;
			//o.cellsIndex = newBucketsIndex;
		}

		XX_INLINE Node& NodeAt(int32_t nodeIndex_) const {
			assert(cells);
			assert(nodeIndex_ >= 0 && nodeIndex_ < count);
			assert(nodes[nodeIndex_].cellsIndex != -1);
			return (Node&)nodes[nodeIndex_];
		}

		XX_INLINE int32_t Count() const {
			return count - freeCount;
		}

		XX_INLINE bool Empty() const {
			return Count() == 0;
		}

	};


}
