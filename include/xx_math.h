#pragma once
#include "xx_prims.h"
#include "xx_rnd.h"

namespace xx {

    bool IsIntersect_BoxBoxI(XYi aPos, XYi aSize, XYi bPos, XYi bSize);

    bool IsIntersect_BoxBoxF(XY b1minXY, XY b1maxXY, XY b2minXY, XY b2maxXY);

    bool IsIntersect_BoxPointF(XY b1minXY, XY b1maxXY, XY p);

    float CalcBounce(float x);

    XY GetRndPosDoughnut(Rnd& rnd_, float maxRadius_, float safeRadius_ = 0.f, float radiansFrom_ = -M_PI, float radiansTo_ = M_PI);

    inline void RGBtoHSV(float& fR, float& fG, float fB, float& fH, float& fS, float& fV);

    void HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV);

    RGBA8 GetRandomColor(Rnd& rnd, RGBA8 refColor_);

}
