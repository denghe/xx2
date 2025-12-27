#include "pch.h"
#include "_phys.h"

XX_INLINE void ::_phys::MakePolygon(b2BodyId const& id_, b2ShapeDef& def_, float scale_, XY const* data_, size_t len_) {
	auto buf = (XY*)alloca(len_ * sizeof(XY));
	for (int32_t i = 0; i < len_; ++i) {
		buf[i] = data_[i] * scale_;
	}
	auto hull = b2ComputeHull((b2Vec2*)buf, len_);
	auto polygon = b2MakePolygon(&hull, 1);
	b2CreatePolygonShape(id_, &def_, &polygon);
};

XX_INLINE void ::_phys::MakeCircle(b2BodyId const& id_, b2ShapeDef& def_, float scale_, float x_, float y_, float r_) {
	auto circle = b2Circle{ .center = {x_ * scale_, y_ * scale_ }, .radius = r_ * scale_ };
	b2CreateCircleShape(id_, &def_, &circle);
};


void ::_phys::_10::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_10::fixture1::density;
			def_->material.friction = ::_phys::_10::fixture1::friction;
			def_->material.restitution = ::_phys::_10::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_10::fixture1::polygons1, _countof(::_phys::_10::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_10::fixture1::polygons2, _countof(::_phys::_10::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_10::fixture1::polygons3, _countof(::_phys::_10::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_10::fixture1::polygons4, _countof(::_phys::_10::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_10::fixture1::polygons5, _countof(::_phys::_10::fixture1::polygons5));
	};
}
void ::_phys::_110::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_110::fixture1::density;
			def_->material.friction = ::_phys::_110::fixture1::friction;
			def_->material.restitution = ::_phys::_110::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_110::fixture1::polygons1, _countof(::_phys::_110::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_110::fixture1::polygons2, _countof(::_phys::_110::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_110::fixture1::polygons3, _countof(::_phys::_110::fixture1::polygons3));
	};
}
void ::_phys::_123::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_123::fixture1::density;
			def_->material.friction = ::_phys::_123::fixture1::friction;
			def_->material.restitution = ::_phys::_123::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_123::fixture1::polygons1, _countof(::_phys::_123::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_123::fixture1::polygons2, _countof(::_phys::_123::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_123::fixture1::polygons3, _countof(::_phys::_123::fixture1::polygons3));
	};
}
void ::_phys::_128::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_128::fixture1::density;
			def_->material.friction = ::_phys::_128::fixture1::friction;
			def_->material.restitution = ::_phys::_128::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_128::fixture1::polygons1, _countof(::_phys::_128::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_128::fixture1::polygons2, _countof(::_phys::_128::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_128::fixture1::polygons3, _countof(::_phys::_128::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_128::fixture1::polygons4, _countof(::_phys::_128::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_128::fixture1::polygons5, _countof(::_phys::_128::fixture1::polygons5));
	};
}
void ::_phys::_138::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_138::fixture1::density;
			def_->material.friction = ::_phys::_138::fixture1::friction;
			def_->material.restitution = ::_phys::_138::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_138::fixture1::polygons1, _countof(::_phys::_138::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_138::fixture1::polygons2, _countof(::_phys::_138::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_138::fixture1::polygons3, _countof(::_phys::_138::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_138::fixture1::polygons4, _countof(::_phys::_138::fixture1::polygons4));
	};
}
void ::_phys::_14::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_14::fixture1::density;
			def_->material.friction = ::_phys::_14::fixture1::friction;
			def_->material.restitution = ::_phys::_14::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_14::fixture1::polygons1, _countof(::_phys::_14::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_14::fixture1::polygons2, _countof(::_phys::_14::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_14::fixture1::polygons3, _countof(::_phys::_14::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_14::fixture1::polygons4, _countof(::_phys::_14::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_14::fixture1::polygons5, _countof(::_phys::_14::fixture1::polygons5));
	};
}
void ::_phys::_147::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_147::fixture1::density;
			def_->material.friction = ::_phys::_147::fixture1::friction;
			def_->material.restitution = ::_phys::_147::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_147::fixture1::polygons1, _countof(::_phys::_147::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_147::fixture1::polygons2, _countof(::_phys::_147::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_147::fixture1::polygons3, _countof(::_phys::_147::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_147::fixture1::polygons4, _countof(::_phys::_147::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_147::fixture1::polygons5, _countof(::_phys::_147::fixture1::polygons5));
	};
}
void ::_phys::_168::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_168::fixture1::density;
			def_->material.friction = ::_phys::_168::fixture1::friction;
			def_->material.restitution = ::_phys::_168::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_168::fixture1::polygons1, _countof(::_phys::_168::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_168::fixture1::polygons2, _countof(::_phys::_168::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_168::fixture1::polygons3, _countof(::_phys::_168::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_168::fixture1::polygons4, _countof(::_phys::_168::fixture1::polygons4));
	};
}
void ::_phys::_192::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_192::fixture1::density;
			def_->material.friction = ::_phys::_192::fixture1::friction;
			def_->material.restitution = ::_phys::_192::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_192::fixture1::polygons1, _countof(::_phys::_192::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_192::fixture1::polygons2, _countof(::_phys::_192::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_192::fixture1::polygons3, _countof(::_phys::_192::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_192::fixture1::polygons4, _countof(::_phys::_192::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_192::fixture1::polygons5, _countof(::_phys::_192::fixture1::polygons5));
		MakePolygon(id_, *def_, scale_, ::_phys::_192::fixture1::polygons6, _countof(::_phys::_192::fixture1::polygons6));
	};
}
void ::_phys::_206::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_206::fixture1::density;
			def_->material.friction = ::_phys::_206::fixture1::friction;
			def_->material.restitution = ::_phys::_206::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_206::fixture1::polygons1, _countof(::_phys::_206::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_206::fixture1::polygons2, _countof(::_phys::_206::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_206::fixture1::polygons3, _countof(::_phys::_206::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_206::fixture1::polygons4, _countof(::_phys::_206::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_206::fixture1::polygons5, _countof(::_phys::_206::fixture1::polygons5));
	};
}
void ::_phys::_243::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_243::fixture1::density;
			def_->material.friction = ::_phys::_243::fixture1::friction;
			def_->material.restitution = ::_phys::_243::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_243::fixture1::polygons1, _countof(::_phys::_243::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_243::fixture1::polygons2, _countof(::_phys::_243::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_243::fixture1::polygons3, _countof(::_phys::_243::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_243::fixture1::polygons4, _countof(::_phys::_243::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_243::fixture1::polygons5, _countof(::_phys::_243::fixture1::polygons5));
		MakePolygon(id_, *def_, scale_, ::_phys::_243::fixture1::polygons6, _countof(::_phys::_243::fixture1::polygons6));
	};
}
void ::_phys::_244::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_244::fixture1::density;
			def_->material.friction = ::_phys::_244::fixture1::friction;
			def_->material.restitution = ::_phys::_244::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_244::fixture1::polygons1, _countof(::_phys::_244::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_244::fixture1::polygons2, _countof(::_phys::_244::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_244::fixture1::polygons3, _countof(::_phys::_244::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_244::fixture1::polygons4, _countof(::_phys::_244::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_244::fixture1::polygons5, _countof(::_phys::_244::fixture1::polygons5));
		MakePolygon(id_, *def_, scale_, ::_phys::_244::fixture1::polygons6, _countof(::_phys::_244::fixture1::polygons6));
	};
}
void ::_phys::_283::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_283::fixture1::density;
			def_->material.friction = ::_phys::_283::fixture1::friction;
			def_->material.restitution = ::_phys::_283::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_283::fixture1::polygons1, _countof(::_phys::_283::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_283::fixture1::polygons2, _countof(::_phys::_283::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_283::fixture1::polygons3, _countof(::_phys::_283::fixture1::polygons3));
	};
}
void ::_phys::_285::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_285::fixture1::density;
			def_->material.friction = ::_phys::_285::fixture1::friction;
			def_->material.restitution = ::_phys::_285::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_285::fixture1::polygons1, _countof(::_phys::_285::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_285::fixture1::polygons2, _countof(::_phys::_285::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_285::fixture1::polygons3, _countof(::_phys::_285::fixture1::polygons3));
	};
}
void ::_phys::_288::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_288::fixture1::density;
			def_->material.friction = ::_phys::_288::fixture1::friction;
			def_->material.restitution = ::_phys::_288::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_288::fixture1::polygons1, _countof(::_phys::_288::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_288::fixture1::polygons2, _countof(::_phys::_288::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_288::fixture1::polygons3, _countof(::_phys::_288::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_288::fixture1::polygons4, _countof(::_phys::_288::fixture1::polygons4));
	};
}
void ::_phys::_296::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_296::fixture1::density;
			def_->material.friction = ::_phys::_296::fixture1::friction;
			def_->material.restitution = ::_phys::_296::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_296::fixture1::polygons1, _countof(::_phys::_296::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_296::fixture1::polygons2, _countof(::_phys::_296::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_296::fixture1::polygons3, _countof(::_phys::_296::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_296::fixture1::polygons4, _countof(::_phys::_296::fixture1::polygons4));
	};
}
void ::_phys::_303::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_303::fixture1::density;
			def_->material.friction = ::_phys::_303::fixture1::friction;
			def_->material.restitution = ::_phys::_303::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_303::fixture1::polygons1, _countof(::_phys::_303::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_303::fixture1::polygons2, _countof(::_phys::_303::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_303::fixture1::polygons3, _countof(::_phys::_303::fixture1::polygons3));
	};
}
void ::_phys::_309::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_309::fixture1::density;
			def_->material.friction = ::_phys::_309::fixture1::friction;
			def_->material.restitution = ::_phys::_309::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_309::fixture1::polygons1, _countof(::_phys::_309::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_309::fixture1::polygons2, _countof(::_phys::_309::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_309::fixture1::polygons3, _countof(::_phys::_309::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_309::fixture1::polygons4, _countof(::_phys::_309::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_309::fixture1::polygons5, _countof(::_phys::_309::fixture1::polygons5));
	};
}
void ::_phys::_402::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_402::fixture1::density;
			def_->material.friction = ::_phys::_402::fixture1::friction;
			def_->material.restitution = ::_phys::_402::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_402::fixture1::polygons1, _countof(::_phys::_402::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_402::fixture1::polygons2, _countof(::_phys::_402::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_402::fixture1::polygons3, _countof(::_phys::_402::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_402::fixture1::polygons4, _countof(::_phys::_402::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_402::fixture1::polygons5, _countof(::_phys::_402::fixture1::polygons5));
	};
}
void ::_phys::_407::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_407::fixture1::density;
			def_->material.friction = ::_phys::_407::fixture1::friction;
			def_->material.restitution = ::_phys::_407::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_407::fixture1::polygons1, _countof(::_phys::_407::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_407::fixture1::polygons2, _countof(::_phys::_407::fixture1::polygons2));
	};
}
void ::_phys::_408::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_408::fixture1::density;
			def_->material.friction = ::_phys::_408::fixture1::friction;
			def_->material.restitution = ::_phys::_408::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_408::fixture1::polygons1, _countof(::_phys::_408::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_408::fixture1::polygons2, _countof(::_phys::_408::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_408::fixture1::polygons3, _countof(::_phys::_408::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_408::fixture1::polygons4, _countof(::_phys::_408::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_408::fixture1::polygons5, _countof(::_phys::_408::fixture1::polygons5));
	};
}
void ::_phys::_415::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_415::fixture1::density;
			def_->material.friction = ::_phys::_415::fixture1::friction;
			def_->material.restitution = ::_phys::_415::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_415::fixture1::polygons1, _countof(::_phys::_415::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_415::fixture1::polygons2, _countof(::_phys::_415::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_415::fixture1::polygons3, _countof(::_phys::_415::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_415::fixture1::polygons4, _countof(::_phys::_415::fixture1::polygons4));
	};
}
void ::_phys::_421::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_421::fixture1::density;
			def_->material.friction = ::_phys::_421::fixture1::friction;
			def_->material.restitution = ::_phys::_421::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_421::fixture1::polygons1, _countof(::_phys::_421::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_421::fixture1::polygons2, _countof(::_phys::_421::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_421::fixture1::polygons3, _countof(::_phys::_421::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_421::fixture1::polygons4, _countof(::_phys::_421::fixture1::polygons4));
	};
}
void ::_phys::_422::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_422::fixture1::density;
			def_->material.friction = ::_phys::_422::fixture1::friction;
			def_->material.restitution = ::_phys::_422::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_422::fixture1::polygons1, _countof(::_phys::_422::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_422::fixture1::polygons2, _countof(::_phys::_422::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_422::fixture1::polygons3, _countof(::_phys::_422::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_422::fixture1::polygons4, _countof(::_phys::_422::fixture1::polygons4));
	};
}
void ::_phys::_443::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_443::fixture1::density;
			def_->material.friction = ::_phys::_443::fixture1::friction;
			def_->material.restitution = ::_phys::_443::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_443::fixture1::polygons1, _countof(::_phys::_443::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_443::fixture1::polygons2, _countof(::_phys::_443::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_443::fixture1::polygons3, _countof(::_phys::_443::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_443::fixture1::polygons4, _countof(::_phys::_443::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_443::fixture1::polygons5, _countof(::_phys::_443::fixture1::polygons5));
		MakePolygon(id_, *def_, scale_, ::_phys::_443::fixture1::polygons6, _countof(::_phys::_443::fixture1::polygons6));
	};
}
void ::_phys::_449::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_449::fixture1::density;
			def_->material.friction = ::_phys::_449::fixture1::friction;
			def_->material.restitution = ::_phys::_449::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_449::fixture1::polygons1, _countof(::_phys::_449::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_449::fixture1::polygons2, _countof(::_phys::_449::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_449::fixture1::polygons3, _countof(::_phys::_449::fixture1::polygons3));
	};
}
void ::_phys::_458::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_458::fixture1::density;
			def_->material.friction = ::_phys::_458::fixture1::friction;
			def_->material.restitution = ::_phys::_458::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_458::fixture1::polygons1, _countof(::_phys::_458::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_458::fixture1::polygons2, _countof(::_phys::_458::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_458::fixture1::polygons3, _countof(::_phys::_458::fixture1::polygons3));
	};
}
void ::_phys::_461::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_461::fixture1::density;
			def_->material.friction = ::_phys::_461::fixture1::friction;
			def_->material.restitution = ::_phys::_461::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_461::fixture1::polygons1, _countof(::_phys::_461::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_461::fixture1::polygons2, _countof(::_phys::_461::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_461::fixture1::polygons3, _countof(::_phys::_461::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_461::fixture1::polygons4, _countof(::_phys::_461::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_461::fixture1::polygons5, _countof(::_phys::_461::fixture1::polygons5));
	};
}
void ::_phys::_462::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_462::fixture1::density;
			def_->material.friction = ::_phys::_462::fixture1::friction;
			def_->material.restitution = ::_phys::_462::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_462::fixture1::polygons1, _countof(::_phys::_462::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_462::fixture1::polygons2, _countof(::_phys::_462::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_462::fixture1::polygons3, _countof(::_phys::_462::fixture1::polygons3));
	};
}
void ::_phys::_463::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_463::fixture1::density;
			def_->material.friction = ::_phys::_463::fixture1::friction;
			def_->material.restitution = ::_phys::_463::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_463::fixture1::polygons1, _countof(::_phys::_463::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_463::fixture1::polygons2, _countof(::_phys::_463::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_463::fixture1::polygons3, _countof(::_phys::_463::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_463::fixture1::polygons4, _countof(::_phys::_463::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_463::fixture1::polygons5, _countof(::_phys::_463::fixture1::polygons5));
		MakePolygon(id_, *def_, scale_, ::_phys::_463::fixture1::polygons6, _countof(::_phys::_463::fixture1::polygons6));
	};
}
void ::_phys::_464::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_464::fixture1::density;
			def_->material.friction = ::_phys::_464::fixture1::friction;
			def_->material.restitution = ::_phys::_464::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_464::fixture1::polygons1, _countof(::_phys::_464::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_464::fixture1::polygons2, _countof(::_phys::_464::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_464::fixture1::polygons3, _countof(::_phys::_464::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_464::fixture1::polygons4, _countof(::_phys::_464::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_464::fixture1::polygons5, _countof(::_phys::_464::fixture1::polygons5));
	};
}
void ::_phys::_467::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_467::fixture1::density;
			def_->material.friction = ::_phys::_467::fixture1::friction;
			def_->material.restitution = ::_phys::_467::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_467::fixture1::polygons1, _countof(::_phys::_467::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_467::fixture1::polygons2, _countof(::_phys::_467::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_467::fixture1::polygons3, _countof(::_phys::_467::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_467::fixture1::polygons4, _countof(::_phys::_467::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_467::fixture1::polygons5, _countof(::_phys::_467::fixture1::polygons5));
	};
}
void ::_phys::_470::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_470::fixture1::density;
			def_->material.friction = ::_phys::_470::fixture1::friction;
			def_->material.restitution = ::_phys::_470::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_470::fixture1::polygons1, _countof(::_phys::_470::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_470::fixture1::polygons2, _countof(::_phys::_470::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_470::fixture1::polygons3, _countof(::_phys::_470::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_470::fixture1::polygons4, _countof(::_phys::_470::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_470::fixture1::polygons5, _countof(::_phys::_470::fixture1::polygons5));
		MakePolygon(id_, *def_, scale_, ::_phys::_470::fixture1::polygons6, _countof(::_phys::_470::fixture1::polygons6));
	};
}
void ::_phys::_472::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_472::fixture1::density;
			def_->material.friction = ::_phys::_472::fixture1::friction;
			def_->material.restitution = ::_phys::_472::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_472::fixture1::polygons1, _countof(::_phys::_472::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_472::fixture1::polygons2, _countof(::_phys::_472::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_472::fixture1::polygons3, _countof(::_phys::_472::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_472::fixture1::polygons4, _countof(::_phys::_472::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_472::fixture1::polygons5, _countof(::_phys::_472::fixture1::polygons5));
		MakePolygon(id_, *def_, scale_, ::_phys::_472::fixture1::polygons6, _countof(::_phys::_472::fixture1::polygons6));
		MakePolygon(id_, *def_, scale_, ::_phys::_472::fixture1::polygons7, _countof(::_phys::_472::fixture1::polygons7));
	};
}
void ::_phys::_51::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_51::fixture1::density;
			def_->material.friction = ::_phys::_51::fixture1::friction;
			def_->material.restitution = ::_phys::_51::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_51::fixture1::polygons1, _countof(::_phys::_51::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_51::fixture1::polygons2, _countof(::_phys::_51::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_51::fixture1::polygons3, _countof(::_phys::_51::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_51::fixture1::polygons4, _countof(::_phys::_51::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_51::fixture1::polygons5, _countof(::_phys::_51::fixture1::polygons5));
	};
}
void ::_phys::_75::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_75::fixture1::density;
			def_->material.friction = ::_phys::_75::fixture1::friction;
			def_->material.restitution = ::_phys::_75::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_75::fixture1::polygons1, _countof(::_phys::_75::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_75::fixture1::polygons2, _countof(::_phys::_75::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_75::fixture1::polygons3, _countof(::_phys::_75::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_75::fixture1::polygons4, _countof(::_phys::_75::fixture1::polygons4));
	};
}
void ::_phys::_98::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_98::fixture1::density;
			def_->material.friction = ::_phys::_98::fixture1::friction;
			def_->material.restitution = ::_phys::_98::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_98::fixture1::polygons1, _countof(::_phys::_98::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_98::fixture1::polygons2, _countof(::_phys::_98::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_98::fixture1::polygons3, _countof(::_phys::_98::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_98::fixture1::polygons4, _countof(::_phys::_98::fixture1::polygons4));
	};
}
void ::_phys::_26::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_26::fixture1::density;
			def_->material.friction = ::_phys::_26::fixture1::friction;
			def_->material.restitution = ::_phys::_26::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_26::fixture1::polygons1, _countof(::_phys::_26::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_26::fixture1::polygons2, _countof(::_phys::_26::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_26::fixture1::polygons3, _countof(::_phys::_26::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_26::fixture1::polygons4, _countof(::_phys::_26::fixture1::polygons4));
	};
}
void ::_phys::_18::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_18::fixture1::density;
			def_->material.friction = ::_phys::_18::fixture1::friction;
			def_->material.restitution = ::_phys::_18::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_18::fixture1::polygons1, _countof(::_phys::_18::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_18::fixture1::polygons2, _countof(::_phys::_18::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_18::fixture1::polygons3, _countof(::_phys::_18::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_18::fixture1::polygons4, _countof(::_phys::_18::fixture1::polygons4));
	};
}
void ::_phys::_17::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys::_17::fixture1::density;
			def_->material.friction = ::_phys::_17::fixture1::friction;
			def_->material.restitution = ::_phys::_17::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys::_17::fixture1::polygons1, _countof(::_phys::_17::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys::_17::fixture1::polygons2, _countof(::_phys::_17::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys::_17::fixture1::polygons3, _countof(::_phys::_17::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys::_17::fixture1::polygons4, _countof(::_phys::_17::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys::_17::fixture1::polygons5, _countof(::_phys::_17::fixture1::polygons5));
	};
}