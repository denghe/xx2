#include "xx_affine.h"

namespace xx {

    void AffineTransform::PosScaleRadiansAnchorSize(XY const& pos, XY const& scale, float radians, XY const& anchorSize) {
        float c_ = 1, s_ = 0;
        if (radians) {
            c_ = std::cos(-radians);
            s_ = std::sin(-radians);
        }
        a = c_ * scale.x;
        b = s_ * scale.x;
        c = -s_ * scale.y;
        d = c_ * scale.y;
        tx = pos.x + c_ * scale.x * -anchorSize.x - s_ * scale.y * -anchorSize.y;
        ty = pos.y + s_ * scale.x * -anchorSize.x + c_ * scale.y * -anchorSize.y;
    }

    void AffineTransform::PosScaleRadians(XY const& pos, XY const& scale, float radians) {
        float c_ = 1, s_ = 0;
        if (radians) {
            c_ = std::cos(-radians);
            s_ = std::sin(-radians);
        }
        a = c_ * scale.x;
        b = s_ * scale.x;
        c = -s_ * scale.y;
        d = c_ * scale.y;
        tx = pos.x;
        ty = pos.y;
    }

    void AffineTransform::PosScaleAnchorSize(XY const& pos, XY const& scale, XY const& anchorSize) {
        a = scale.x;
        b = 0;
        c = 0;
        d = scale.y;
        tx = pos.x + scale.x * -anchorSize.x;
        ty = pos.y + scale.y * -anchorSize.y;
    }

    void AffineTransform::PosScale(XY const& pos, XY const& scale) {
        a = scale.x;
        b = 0;
        c = 0;
        d = scale.y;
        tx = pos.x;
        ty = pos.y;
    }

    void AffineTransform::Pos(XY const& pos) {
        a = 1;
        b = 0;
        c = 0;
        d = 1;
        tx = pos.x;
        ty = pos.y;
    }

    void AffineTransform::Identity() {
        a = 1;
        b = 0;
        c = 0;
        d = 1;
        tx = 0;
        ty = 0;
    }

    XY AffineTransform::operator()(XY const& point) const {
        return { (float)((double)a * point.x + (double)c * point.y + tx), (float)((double)b * point.x + (double)d * point.y + ty) };
    }

    XY AffineTransform::NoRadiansApply(XY const& point) const {
        return { (float)((double)a * point.x + tx), (float)((double)d * point.y + ty) };
    }

    AffineTransform AffineTransform::MakeConcat(AffineTransform const& t2) {
        auto& t1 = *this;
        return { t1.a * t2.a + t1.b * t2.c, t1.a * t2.b + t1.b * t2.d, t1.c * t2.a + t1.d * t2.c, t1.c * t2.b + t1.d * t2.d,
            t1.tx * t2.a + t1.ty * t2.c + t2.tx, t1.tx * t2.b + t1.ty * t2.d + t2.ty };
    }

    AffineTransform AffineTransform::MakeInvert() {
        auto& t = *this;
        auto determinant = 1 / (t.a * t.d - t.b * t.c);
        return { determinant * t.d, -determinant * t.b, -determinant * t.c, determinant * t.a,
            determinant * (t.c * t.ty - t.d * t.tx), determinant * (t.b * t.tx - t.a * t.ty) };
    }

    AffineTransform AffineTransform::MakePosScaleRadiansAnchorSize(XY const& pos, XY const& scale, float radians, XY const& anchorSize) {
        AffineTransform at;
        at.PosScaleRadiansAnchorSize(pos, scale, radians, anchorSize);
        return at;
    }

    AffineTransform AffineTransform::MakePosScaleAnchorSize(XY const& pos, XY const& scale, XY const& anchorSize) {
        AffineTransform at;
        at.PosScaleAnchorSize(pos, scale, anchorSize);
        return at;
    }

    AffineTransform AffineTransform::MakePosScaleRadians(XY const& pos, XY const& scale, float radians) {
        AffineTransform at;
        at.PosScaleRadians(pos, scale, radians);
        return at;
    }

    AffineTransform AffineTransform::MakePosScale(XY const& pos, XY const& scale) {
        return { scale.x, 0, 0, scale.y, pos.x, pos.y };
    }

    AffineTransform AffineTransform::MakePos(XY const& pos) {
        return { 1.0, 0.0, 0.0, 1.0, pos.x, pos.y };
    }

    AffineTransform AffineTransform::MakeIdentity() {
        return { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    }


    /*******************************************************************************************************************************************/
    /*******************************************************************************************************************************************/

	
	void SimpleAffineTransform::PosScaleAnchorSize(XY const& pos, XY const& scale, XY const& anchorSize) {
		a = scale.x;
		d = scale.y;
		tx = pos.x - scale.x * anchorSize.x;
		ty = pos.y - scale.y * anchorSize.y;
	}

	void SimpleAffineTransform::Identity() {
		a = 1;
		d = 1;
		tx = 0;
		ty = 0;
	}

	XY const& SimpleAffineTransform::Offset() const {
		return (XY&)tx;
	}

	XY const& SimpleAffineTransform::Scale() const {
		return (XY&)a;
	}

	XY SimpleAffineTransform::operator()(XY const& point) const {
		return { (float)((double)a * point.x + tx), (float)((double)d * point.y + ty) };
	}

	SimpleAffineTransform SimpleAffineTransform::MakeConcat(SimpleAffineTransform const& t2) const {
		auto& t1 = *this;
		return { t1.a * t2.a, t1.d * t2.d, t1.tx * t2.a + t2.tx, t1.ty * t2.d + t2.ty };
	}

	SimpleAffineTransform SimpleAffineTransform::MakeInvert() const {
		auto& t = *this;
		auto determinant = 1 / (t.a * t.d);
		return { determinant * t.d, determinant * t.a, determinant * (-t.d * t.tx), determinant * (-t.a * t.ty) };
	}

	SimpleAffineTransform SimpleAffineTransform::MakeIdentity() {
		return { 1.0, 1.0, 0.0, 0.0 };
	}

	SimpleAffineTransform SimpleAffineTransform::MakePosScaleAnchorSize(XY const& pos, XY const& scale, XY const& anchorSize) {
		SimpleAffineTransform t;
		t.PosScaleAnchorSize(pos, scale, anchorSize);
		return t;
	}

}
