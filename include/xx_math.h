#pragma once
#include "xx_gamebase_font.h"
#include "xx_node.h"

namespace xx {

    // for integer
    template<typename T = XYi>
    XX_INLINE bool IsIntersect_BoxBoxI(T aPos, T aSize, T bPos, T bSize) {
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

    // for float
    template<typename XY1 = XY, typename XY2 = XY>
    XX_INLINE bool IsIntersect_BoxBoxF(XY1 const& b1minXY, XY1 const& b1maxXY, XY2 const& b2minXY, XY2 const& b2maxXY) {
        return !(b1maxXY.x < b2minXY.x || b2maxXY.x < b1minXY.x || b1maxXY.y < b2minXY.y || b2maxXY.y < b1minXY.y);
    }

    // for float
    template<typename XY1 = XY, typename XY2 = XY>
    XX_INLINE bool IsIntersect_BoxPointF(XY1 const& b1minXY, XY1 const& b1maxXY, XY2 const& p) {
        return !(b1maxXY.x < p.x || p.x < b1minXY.x || b1maxXY.y < p.y || p.y < b1minXY.y);
    }
}
