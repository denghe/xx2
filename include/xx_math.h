#pragma once
#include "xx_prims.h"
#include "xx_rnd.h"

namespace xx {

    inline static constexpr float gPI{ (float)M_PI }, gNPI{ -gPI }, g2PI{ gPI * 2 }, gPI_2{ gPI / 2 };
    inline static constexpr float gSQRT2{ 1.414213562373095f }, gSQRT2_1{ 0.7071067811865475f };


    bool IsIntersect_BoxBoxI(XYi aPos, XYi aSize, XYi bPos, XYi bSize);

    bool IsIntersect_BoxBoxF(XY b1minXY, XY b1maxXY, XY b2minXY, XY b2maxXY);

    bool IsIntersect_BoxPointF(XY b1minXY, XY b1maxXY, XY p);

    float CalcBounce(float x);

    XY GetRndPosDoughnut(Rnd& rnd_, float maxRadius_, float safeRadius_ = 0.f, float radiansFrom_ = -M_PI, float radiansTo_ = M_PI);

    inline void RGBtoHSV(float& fR, float& fG, float fB, float& fH, float& fS, float& fV);

    void HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV);

    RGBA8 GetRandomColor(Rnd& rnd, RGBA8 refColor_);

    XY RotatePoint(XY d, float radians);


    float AngleGap(float tar, float cur);

    // calc cur to tar by rate
    // return cur's new value
    // todo: verify
    float AngleLerpByRate(float tar, float cur, float rate);

    /*
        * linear example:

        auto bak = radians;
        radians = xx::RotateControl::LerpAngleByFixed(targetRadians, radians, frameMaxChangeRadian);
        auto rd = bak - radians;
        if (rd >= xx::gPI)
        {
            rd -= xx::g2PI;
        }
        else if (rd < xx::gNPI)
        {
            rd += xx::g2PI;
        }
        auto radiansStep = rd / n;
    */
    // calc cur to tar by fixed raidians
    // return cur's new value
    float AngleLerpByFixed(float tar, float cur, float a);

    // change cur to tar by a( max value )
    // return true: cur == tar
    bool AngleStep(float& cur, float tar, float a);

    // limit a by from ~ to
    // no change: return false
    bool AngleLimit(float& a, float from, float to);
}
