#pragma once
#include "xx_includes.h"
#include "xx_list.h"
#include "xx_prims.h"

namespace xx {
	
    struct Grid2dAABB {
        struct Node {
            int32_t next, inResults;
            FromTo<XY> aabb;
            FromTo<XYi> ccrr;
            void* ud;
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

        void Init(XYi gridSize_, XY cellSize_, int32_t cellCap_ = 4, int32_t capacity_ = 16);
        ~Grid2dAABB();
        void Reserve(int32_t capacity_);
        int32_t Add(FromTo<XY> const& aabb_, void* ud_);
        void Remove(int32_t nodeIndex_);
        void Update(int32_t nodeIndex_, FromTo<XY> const& aabb_);
        Node& NodeAt(int32_t nodeIndex_) const;
        int32_t Count() const;
        bool Empty() const;

        // return true: success.  false: aabb is out of range
        bool TryLimitAABB(FromTo<XY>& aabb_, float edge_ = 1.f);

        // search by point. fill items to results
        void ForeachPoint(XY const& p_);

        // search by AABB. fill items to results
        void ForeachAABB(FromTo<XY> const& aabb_, int32_t except_ = -1);
    };

}
