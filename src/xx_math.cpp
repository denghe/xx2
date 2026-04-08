#include <xx_math.h>

namespace xx {

    /**************************************************************************************************/
    // xx_math.h
    /**************************************************************************************************/

    bool IsIntersect_BoxBoxI(XYi aPos, XYi aSize, XYi bPos, XYi bSize) {
        if (bPos.x >= aPos.x) {
            if (bPos.y >= aPos.y) {
                if (bPos.x < aPos.x + aSize.x) return bPos.y < aPos.y + aSize.y;
            }
            else /* bPos.y < aPos.y */ {
                if (bPos.x < aPos.x + aSize.x) return bPos.y + bSize.y > aPos.y;
            }
        }
        else /* bPos.x < aPos.x */ {
            if (bPos.y >= aPos.y) {
                if (bPos.x + bSize.x > aPos.x) return bPos.y < aPos.y + aSize.y;
            }
            else /* bPos.y < aPos.y */ {
                if (bPos.x + bSize.x > aPos.x) return bPos.y + bSize.y > aPos.y;
            }
        }
        return false;
    }

	bool IsIntersect_BoxBoxF(XY b1minXY, XY b1maxXY, XY b2minXY, XY b2maxXY) {
		return !(b1maxXY.x < b2minXY.x || b2maxXY.x < b1minXY.x
			|| b1maxXY.y < b2minXY.y || b2maxXY.y < b1minXY.y);
	}

	bool IsIntersect_BoxPointF(XY b1minXY, XY b1maxXY, XY p) {
		return !(b1maxXY.x < p.x || p.x < b1minXY.x || b1maxXY.y < p.y || p.y < b1minXY.y);
	}

	float CalcBounce(float x) {
		return 1.f - std::expf(-5.f * x) * std::cosf(6.f * (float)M_PI * x);
	}

	XY GetRndPosDoughnut(Rnd& rnd_, float maxRadius_, float safeRadius_, float radiansFrom_, float radiansTo_) {
		auto len = maxRadius_ - safeRadius_;
		auto len_radius = len / maxRadius_;
		auto safeRadius_radius = safeRadius_ / maxRadius_;
		auto radius = std::sqrtf(rnd_.Next<float>(0, len_radius) + safeRadius_radius) * maxRadius_;
		auto radians = rnd_.Next<float>(radiansFrom_, radiansTo_);
		return { std::cosf(radians) * radius, std::sinf(radians) * radius };
	}



    // https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72

    // Copyright (c) 2014, Jan Winkler <winkler@cs.uni-bremen.de>
    // All rights reserved.
    //
    // Redistribution and use in source and binary forms, with or without
    // modification, are permitted provided that the following conditions are met:
    //
    //     * Redistributions of source code must retain the above copyright
    //       notice, this list of conditions and the following disclaimer.
    //     * Redistributions in binary form must reproduce the above copyright
    //       notice, this list of conditions and the following disclaimer in the
    //       documentation and/or other materials provided with the distribution.
    //     * Neither the name of Universität Bremen nor the names of its
    //       contributors may be used to endorse or promote products derived from
    //       this software without specific prior written permission.
    //
    // THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    // AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    // IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    // ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
    // LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    // CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    // SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    // INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    // CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    // ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    // POSSIBILITY OF SUCH DAMAGE.

    /* Author: Jan Winkler */

    /*! \brief Convert RGB to HSV color space

      Converts a given set of RGB values `r', `g', `b' into HSV
      coordinates. The input RGB values are in the range [0, 1], and the
      output HSV values are in the ranges h = [0, 360], and s, v = [0,
      1], respectively.

      \param fR Red component, used as input, range: [0, 1]
      \param fG Green component, used as input, range: [0, 1]
      \param fB Blue component, used as input, range: [0, 1]
      \param fH Hue component, used as output, range: [0, 360]
      \param fS Hue component, used as output, range: [0, 1]
      \param fV Hue component, used as output, range: [0, 1]

    */
    void RGBtoHSV(float& fR, float& fG, float fB, float& fH, float& fS, float& fV) {
        float fCMax = std::max(std::max(fR, fG), fB);
        float fCMin = std::min(std::min(fR, fG), fB);
        float fDelta = fCMax - fCMin;

        if (fDelta > 0) {
            if (fCMax == fR) {
                fH = 60 * (fmod(((fG - fB) / fDelta), 6));
            }
            else if (fCMax == fG) {
                fH = 60 * (((fB - fR) / fDelta) + 2);
            }
            else if (fCMax == fB) {
                fH = 60 * (((fR - fG) / fDelta) + 4);
            }

            if (fCMax > 0) {
                fS = fDelta / fCMax;
            }
            else {
                fS = 0;
            }

            fV = fCMax;
        }
        else {
            fH = 0;
            fS = 0;
            fV = fCMax;
        }

        if (fH < 0) {
            fH = 360 + fH;
        }
    }



    /*! \brief Convert HSV to RGB color space

      Converts a given set of HSV values `h', `s', `v' into RGB
      coordinates. The output RGB values are in the range [0, 1], and
      the input HSV values are in the ranges h = [0, 360], and s, v =
      [0, 1], respectively.

      \param fR Red component, used as output, range: [0, 1]
      \param fG Green component, used as output, range: [0, 1]
      \param fB Blue component, used as output, range: [0, 1]
      \param fH Hue component, used as input, range: [0, 360]
      \param fS Hue component, used as input, range: [0, 1]
      \param fV Hue component, used as input, range: [0, 1]

    */
    void HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV) {
        float fC = fV * fS; // Chroma
        float fHPrime = fmod(fH / 60.0, 6);
        float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
        float fM = fV - fC;

        if (0 <= fHPrime && fHPrime < 1) {
            fR = fC;
            fG = fX;
            fB = 0;
        }
        else if (1 <= fHPrime && fHPrime < 2) {
            fR = fX;
            fG = fC;
            fB = 0;
        }
        else if (2 <= fHPrime && fHPrime < 3) {
            fR = 0;
            fG = fC;
            fB = fX;
        }
        else if (3 <= fHPrime && fHPrime < 4) {
            fR = 0;
            fG = fX;
            fB = fC;
        }
        else if (4 <= fHPrime && fHPrime < 5) {
            fR = fX;
            fG = 0;
            fB = fC;
        }
        else if (5 <= fHPrime && fHPrime < 6) {
            fR = fC;
            fG = 0;
            fB = fX;
        }
        else {
            fR = 0;
            fG = 0;
            fB = 0;
        }

        fR += fM;
        fG += fM;
        fB += fM;
    }

    RGBA8 GetRandomColor(Rnd& rnd, RGBA8 refColor_) {
        float r, g, b, h, s, v;
        r = refColor_.r / 255.f;
        g = refColor_.g / 255.f;
        b = refColor_.b / 255.f;
        xx::RGBtoHSV(r, g, b, h, s, v);
        auto newH = rnd.Next(0.f, 360.f);
        xx::HSVtoRGB(r, g, b, newH, s, v);
        return { (uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), refColor_.a };
    }

    XY RotatePoint(XY d, float radians) {
        auto c = std::cos(radians);
        auto s = std::sin(radians);
        return { d.x * c - d.y * s, d.x * s + d.y * c };
    }



    float AngleGap(float tar, float cur) {
        auto gap = cur - tar;
        if (gap > gPI) {
            gap -= g2PI;
        }
        if (gap < gNPI) {
            gap += g2PI;
        }
        return gap;
    }

    float AngleLerpByRate(float tar, float cur, float rate) {
        auto gap = AngleGap(tar, cur);
        return cur - gap * rate;
    }

    float AngleLerpByFixed(float tar, float cur, float a) {
        auto gap = AngleGap(tar, cur);
        if (gap < 0) {
            if (gap >= -a) return tar;
            else if (auto r = cur + a; r < gPI) return r;
            else return r - g2PI;
        }
        else {
            if (gap <= a) return tar;
            else if (auto r = cur - a; r > gPI) return r;
            else return r + g2PI;
        }
    }

    bool AngleStep(float& cur, float tar, float a) {
        return (cur = AngleLerpByFixed(tar, cur, a)) == tar;
    }

    bool AngleLimit(float& a, float from, float to) {
        // -PI ~~~~~~~~~~~~~~~~~~~~~~~~~ a ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PI
        assert(a >= gNPI && a <= gPI);
        // from ~~~~~~~~~~~~~~ a ~~~~~~~~~~~~~~~~ to
        if (a >= from && a <= to) return false;
        // from ~~~~~~~~~~~~~~~ -PI ~~~~~~~~~~~~~~~~~~ to ~~~~~~~~~~~~~~~ PI
        if (from < gNPI) {
            // ~~~~~~ from ~~~~~~~ -PI ~~~~~~ to ~~~~ a ~~~~ 0 ~~~~~~~~~~~~~ PI
            if (a < 0) {
                if (a - to < from + g2PI - a) {
                    a = to;
                }
                else {
                    a = from + g2PI;
                }
                // ~~~~~ d ~~~~~~ from ~~~~~~~ -PI ~~~~~~~~ to ~~~~~~~~ 0 ~~~~~~~ a ~~~~ PI
            }
            else {
                auto d = a - g2PI;
                if (d >= from && d <= to) return false;
                else {
                    if (from - d < a - to) {
                        a = from + g2PI;
                    }
                    else {
                        a = to;
                    }
                }
            }
            // -PI ~~~~~~~~~~~~~~~ from ~~~~~~~~~~~~~~~~~~ PI ~~~~~~~~~~~~~~~ to
        }
        else if (to > gPI) {
            // -PI ~~~~~~~~~~~~~~~ 0 ~~~~~ a ~~~~~ from ~~~~~~ PI ~~~~~~~ to
            if (a > 0) {
                if (from - a < a - (to - g2PI)) {
                    a = from;
                }
                else {
                    a = to - g2PI;
                }
                // -PI ~~~~~~~ a ~~~~~~~~ 0 ~~~~~~~ from ~~~~~~ PI ~~~~~~~ to ~~~~~ d ~~~~~
            }
            else {
                auto d = a + g2PI;
                if (d >= from && d <= to) return false;
                else {
                    if (from - a < d - to) {
                        a = from;
                    }
                    else {
                        a = to - g2PI;
                    }
                }
            }
        }
        else {
            // -PI ~~~~~ a ~~~~ from ~~~~~~~~~~~~~~~~~~ to ~~~~~~~~~~~ PI
            if (a < from) {
                if (to <= 0 || from - a < a - (to - g2PI)) {
                    a = from;
                }
                else {
                    a = to;
                }
                // -PI ~~~~~~~~~ from ~~~~~~~~~~~~~~~~~~ to ~~~~~ a ~~~~~~ PI
            }
            else {
                if (from > 0 || a - to < from + g2PI - a) {
                    a = to;
                }
                else {
                    a = from;
                }
            }
        }
        return true;
    }
}
