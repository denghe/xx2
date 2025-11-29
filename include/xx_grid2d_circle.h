#pragma once
#include "xx_grid2d.h"

namespace xx {

	template<typename T, typename C = void>
	struct Grid2dCircle : Grid2dBase<Grid2dCircle<T, C>, T, C> {
		using Base = Grid2dBase<Grid2dCircle<T, C>, T, C>;
		using Base::Base;
		int32_t cellSize{};
		float _1_cellSize{};
		XYi pixelSize{};	// world size

		void Init(int32_t cellSize_, int32_t numRows_, int32_t numCols_, int32_t capacity_ = 100) {
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
			nodeIndex_ = Base::Alloc(cri.y, cri.x);
			if constexpr (!std::is_void_v<C>) {
				this->nodes[nodeIndex_].cache = e;
			}
			Base::Add(nodeIndex_, std::forward<V>(e));
		}

		template<typename V>
		void Add(V&& e) {
			int32_t tmp{-1};
			Add(tmp, std::forward<V>(e));
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

		void Remove(int32_t& nodeIndex_, T const& e) {
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
