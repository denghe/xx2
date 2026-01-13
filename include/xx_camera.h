#pragma once
#include "xx_prims.h"

namespace xx {

    struct Camera {
        float logicScale{ 1 }, baseScale{ 1 };
        float scale{ 1 }, _1_scale{ 1 };
        XY original{};	// logic center pos
        XY offset{};

        void Init(float baseScale_, float logicScale_, XY original_ = {});
        void SetOriginal(XY original_);
        void SetBaseScale(float baseScale_);
        void SetLogicScale(float logicScale_);
        XY ToGLPos(XY logicPos_) const;
        XY ToLogicPos(XY glPos_) const;
    };

}
