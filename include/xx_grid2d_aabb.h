#pragma once
#include "xx_space_.h"

namespace xx {

	template <typename T>
	struct Grid2dAABB {
		struct Node {
			int32_t next, inResults;
			FromTo<XY> aabb;
			FromTo<XYi> ccrr;
			T value;				// pointer?
		};

		XYi gridSize{};
		XY cellSize{}, _1_cellSize{}, worldSize{};
		int32_t freeHead{ -1 }, freeCount{}, count{}, capacity{};
		Node* nodes{};
		List<List<int32_t>> cells{};	// value: nodes index
		List<int32_t> results;

		Grid2dAABB() = default;
		Grid2dAABB(Grid2dAABB const& o) = delete;
		Grid2dAABB& operator=(Grid2dAABB const& o) = delete;

		XX_INLINE void Init(XYi gridSize_, XY cellSize_, int32_t cellCap_ = 4, int32_t capacity_ = 16) {
			assert(!nodes && capacity_ > 0);
			assert(gridSize_.x > 0 && gridSize_.y > 0);
			assert(cellSize_.x > 0 && cellSize_.y > 0);
			gridSize = gridSize_;
			cellSize = cellSize_;
			_1_cellSize = 1.f / cellSize_;
			worldSize = cellSize_ * gridSize_;
			freeHead = -1;
			freeCount = count = 0;
			capacity = capacity_;
			nodes = (Node*)new MyAlignedStorage<Node>[capacity_];
			cells.Resize(gridSize_.x * gridSize_.y);
			for (auto& c : cells) c.Reserve(cellCap_);
		}

		~Grid2dAABB() {
			if (!nodes) return;
			for (int32_t i = 0; i < count; ++i) {
				auto& o = nodes[i];
				if (o.next == -1) {
					o.value.~T();
				}
			}
			delete[](MyAlignedStorage<Node>*)nodes;
			nodes = {};
		}

		void Reserve(int32_t capacity_) {
			assert(capacity_ > 0);
			if (capacity_ <= capacity) return;
			auto newNodes = (Node*)new MyAlignedStorage<Node>[capacity_];
			if constexpr (IsPod_v<T>) {
				::memcpy((void*)newNodes, (void*)nodes, count * sizeof(Node));
			}
			else {
				for (int32_t i = 0; i < count; ++i) {
					new (&newNodes[i]) Node((Node&&)nodes[i]);
					nodes[i].value.~T();
				}
			}
			delete[](MyAlignedStorage<Node>*)nodes;
			nodes = newNodes;
			capacity = capacity_;
		}

		template<typename V>
		XX_INLINE int32_t Add(FromTo<XY> const& aabb_, V&& v_) {
			assert(aabb_.from.x < aabb_.to.x && aabb_.from.y < aabb_.to.y);
			assert(aabb_.from.x >= 0 && aabb_.from.x < worldSize.x);
			assert(aabb_.from.y >= 0 && aabb_.from.y < worldSize.y);
			assert(aabb_.to.x >= 0 && aabb_.to.x < worldSize.x);
			assert(aabb_.to.y >= 0 && aabb_.to.y < worldSize.y);

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

			// calc covered cells
			FromTo<XYi> ccrr{ (aabb_.from * _1_cellSize).template As<int32_t>()
							, (aabb_.to * _1_cellSize).template As<int32_t>() };

			// link
			for (auto y = ccrr.from.y; y <= ccrr.to.y; y++) {
				for (auto x = ccrr.from.x; x <= ccrr.to.x; x++) {
					auto& c = cells[y * gridSize.x + x];
					assert(c.Find(nodeIndex) == -1);
					c.Emplace(nodeIndex);
				}
			}

			// init
			o.next = -1;
			o.inResults = 0;
			o.aabb = aabb_;
			o.ccrr = ccrr;
			new (&o.value) T(std::forward<V>(v_));
			return nodeIndex;
		}

		XX_INLINE void Remove(int32_t nodeIndex_) {
			assert(nodeIndex_ >= 0 && nodeIndex_ < count && nodes[nodeIndex_].next == -1);
			auto& o = nodes[nodeIndex_];

			// unlink
			for (auto y = o.ccrr.from.y; y <= o.ccrr.to.y; y++) {
				for (auto x = o.ccrr.from.x; x <= o.ccrr.to.x; x++) {
					auto& c = cells[y * gridSize.x + x];
					for (int32_t i = c.len - 1; i >= 0; --i) {
						if (c[i] == nodeIndex_) {
							c.SwapRemoveAt(i);
							goto LabEnd;
						}
					}
					assert(false);
				LabEnd:;
				}
			}

			// free
			o.next = freeHead;
			freeHead = nodeIndex_;
			freeCount++;
			o.value.~T();
		}

		XX_INLINE void Update(int32_t nodeIndex_, FromTo<XY> const& aabb_) {
			assert(nodes);
			assert(nodeIndex_ >= 0 && nodeIndex_ < count && nodes[nodeIndex_].next == -1);
			assert(aabb_.from.x < aabb_.to.x && aabb_.from.y < aabb_.to.y);
			assert(aabb_.from.x >= 0 && aabb_.from.x < worldSize.x);
			assert(aabb_.from.y >= 0 && aabb_.from.y < worldSize.y);
			assert(aabb_.to.x >= 0 && aabb_.to.x < worldSize.x);
			assert(aabb_.to.y >= 0 && aabb_.to.y < worldSize.y);

			// locate
			auto& o = nodes[nodeIndex_];

			// update1
			o.aabb = aabb_;

			FromTo<XYi> ccrr{ (aabb_.from * _1_cellSize).template As<int32_t>()
							, (aabb_.to * _1_cellSize).template As<int32_t>() };

			// no change check
			if (o.ccrr == ccrr) return;

			// unlink
			for (auto y = o.ccrr.from.y; y <= o.ccrr.to.y; y++) {
				for (auto x = o.ccrr.from.x; x <= o.ccrr.to.x; x++) {
					auto& c = cells[y * gridSize.x + x];
					for (int32_t i = c.len - 1; i >= 0; --i) {
						if (c[i] == nodeIndex_) {
							c.SwapRemoveAt(i);
							goto LabEnd;
						}
					}
					assert(false);
				LabEnd:;
				}
			}

			// link
			for (auto y = ccrr.from.y; y <= ccrr.to.y; y++) {
				for (auto x = ccrr.from.x; x <= ccrr.to.x; x++) {
					auto& c = cells[y * gridSize.x + x];
					assert(c.Find(nodeIndex_) == -1);
					c.Emplace(nodeIndex_);
				}
			}

			// update2
			o.ccrr = ccrr;
		}

		XX_INLINE Node& NodeAt(int32_t nodeIndex_) const {
			assert(nodeIndex_ >= 0 && nodeIndex_ < count && nodes[nodeIndex_].next == -1);
			return (Node&)nodes[nodeIndex_];
		}

		XX_INLINE int32_t Count() const {
			return count - freeCount;
		}

		XX_INLINE bool Empty() const {
			return Count() == 0;
		}


		// return true: success.  false: aabb is out of range
		XX_INLINE bool TryLimitAABB(FromTo<XY>& aabb_, float edge_ = 1.f) {
			assert(edge_ > 0 && edge_ < cellSize.x * 0.5f && edge_ < cellSize.y * 0.5f);
			auto ws = worldSize - edge_;
			if (!IsIntersect_BoxBoxF(XY{ edge_ }, ws, aabb_.from, aabb_.to)) return false;
			if (aabb_.from.x < edge_) aabb_.from.x = edge_;
			if (aabb_.from.y < edge_) aabb_.from.y = edge_;
			if (aabb_.to.x > ws.x) aabb_.to.x = ws.x;
			if (aabb_.to.y > ws.y) aabb_.to.y = ws.y;
			return true;
		}

		// fill items to results
		void ForeachPoint(XY const& p_) {
			assert(p_.x >= 0 && p_.x < worldSize.x);
			assert(p_.y >= 0 && p_.y < worldSize.y);
			results.Clear();

			auto cr = (p_ * _1_cellSize).template As<int32_t>();
			auto& c = cells[cr.y * gridSize.x + cr.x];
			for (int32_t i = 0; i < c.len; ++i) {
				auto& n = nodes[c[i]];
				if (IsIntersect_BoxPointF(n.aabb.from, n.aabb.to, p_)) {
					results.Add(c[i]);
				}
			}
		}

		// fill items to results
		void ForeachAABB(FromTo<XY> const& aabb_, int32_t except_ = -1) {
			assert(aabb_.from.x < aabb_.to.x && aabb_.from.y < aabb_.to.y);
			assert(aabb_.from.x >= 0 && aabb_.from.x < worldSize.x);
			assert(aabb_.from.y >= 0 && aabb_.from.y < worldSize.y);
			assert(aabb_.to.x >= 0 && aabb_.to.x < worldSize.x);
			assert(aabb_.to.y >= 0 && aabb_.to.y < worldSize.y);
			results.Clear();

			// calc covered cells
			FromTo<XYi> ccrr{ (aabb_.from * _1_cellSize).template As<int32_t>()
				, (aabb_.to * _1_cellSize).template As<int32_t>() };

			if (ccrr.from.x == ccrr.to.x || ccrr.from.y == ccrr.to.y) {
				// 1-2 row, 1-2 col: do full rect check
				for (auto y = ccrr.from.y; y <= ccrr.to.y; y++) {
					for (auto x = ccrr.from.x; x <= ccrr.to.x; x++) {
						auto& c = cells[y * gridSize.x + x];
						for (int32_t i = 0; i < c.len; ++i) {
							if (c[i] == except_) continue;
							auto& n = nodes[c[i]];
							if (!(n.aabb.to.x < aabb_.from.x || aabb_.to.x < n.aabb.from.x 
								|| n.aabb.to.y < aabb_.from.y || aabb_.to.y < n.aabb.from.y)) {
								results.Emplace(c[i]);
							}
						}
					}
				}
				return;
			}

			// except set flag
			if (except_ != -1) {
				assert(except_ >= 0 && except_ < count && nodes[except_].next == -1);
				nodes[except_].inResults = 1;
			}

			// first row: check AB
			auto y = ccrr.from.y;

			// first cell: check top left AB
			auto x = ccrr.from.x;
			{
				auto& c = cells[y * gridSize.x + x];
				for (int32_t i = 0; i < c.len; ++i) {
					auto& n = nodes[c[i]];
					if (n.inResults) continue;
					if (n.aabb.to.x > aabb_.from.x && n.aabb.to.y > aabb_.from.y) {
						n.inResults = 1;
						results.Emplace(c[i]);
					}
				}
			}

			// middle cells: check top AB
			for (++x; x < ccrr.to.x; x++) {
				auto& c = cells[y * gridSize.x + x];
				for (int32_t i = 0; i < c.len; ++i) {
					auto& n = nodes[c[i]];
					if (n.inResults) continue;
					if (n.aabb.to.y > aabb_.from.y) {
						n.inResults = 1;
						results.Emplace(c[i]);
					}
				}
			}

			// last cell: check top right AB
			if (x == ccrr.to.x) {
				auto& c = cells[y * gridSize.x + x];
				for (int32_t i = 0; i < c.len; ++i) {
					auto& n = nodes[c[i]];
					if (n.inResults) continue;
					if (n.aabb.from.x < aabb_.to.x && n.aabb.to.y > aabb_.from.y) {
						n.inResults = 1;
						results.Emplace(c[i]);
					}
				}
			}

			// middle rows: first & last col check AB
			for (++y; y < ccrr.to.y; y++) {

				// first cell: check left AB
				x = ccrr.from.x;
				auto& c = cells[y * gridSize.x + x];
				for (int32_t i = 0; i < c.len; ++i) {
					auto& n = nodes[c[i]];
					if (n.inResults) continue;
					if (n.aabb.to.x > aabb_.from.x) {
						n.inResults = 1;
						results.Emplace(c[i]);
					}
				}

				// middle cols: no check
				for (; x < ccrr.to.x; x++) {
					auto& c = cells[y * gridSize.x + x];
					for (int32_t i = 0; i < c.len; ++i) {
						auto& n = nodes[c[i]];
						if (n.inResults) continue;
						n.inResults = 1;
						results.Emplace(c[i]);
					}
				}

				// last cell: check right AB
				if (x == ccrr.to.x) {
					auto& c = cells[y * gridSize.x + x];
					for (int32_t i = 0; i < c.len; ++i) {
						auto& n = nodes[c[i]];
						if (n.inResults) continue;
						if (n.aabb.from.x < aabb_.to.x) {
							n.inResults = 1;
							results.Emplace(c[i]);
						}
					}
				}
			}

			// last row: check AB
			if (y == ccrr.to.y) {

				// first cell: check left bottom AB
				x = ccrr.from.x;
				auto& c = cells[y * gridSize.x + x];
				for (int32_t i = 0; i < c.len; ++i) {
					auto& n = nodes[c[i]];
					if (n.inResults) continue;
					if (n.aabb.to.x > aabb_.from.x && n.aabb.from.y < aabb_.to.y) {
						n.inResults = 1;
						results.Emplace(c[i]);
					}
				}

				// middle cells: check bottom AB
				for (++x; x < ccrr.to.x; x++) {
					auto& c = cells[y * gridSize.x + x];
					for (int32_t i = 0; i < c.len; ++i) {
						auto& n = nodes[c[i]];
						if (n.inResults) continue;
						if (n.aabb.from.y < aabb_.to.y) {
							n.inResults = 1;
							results.Emplace(c[i]);
						}
					}
				}

				// last cell: check right bottom AB
				if (x == ccrr.to.x) {
					auto& c = cells[y * gridSize.x + x];
					for (int32_t i = 0; i < c.len; ++i) {
						auto& n = nodes[c[i]];
						if (n.inResults) continue;
						if (n.aabb.from.x < aabb_.to.x && n.aabb.from.y < aabb_.to.y) {
							n.inResults = 1;
							results.Emplace(c[i]);
						}
					}
				}
			}

			// clear flags
			if (except_ != -1) {
				nodes[except_].inResults = 0;
			}
			for (int32_t i = 0; i < results.len; ++i) {
				nodes[results[i]].inResults = 0;
			}
		}

	};

}
