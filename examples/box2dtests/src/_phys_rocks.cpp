#include "pch.h"
#include "_phys_rocks.h"

XX_INLINE void ::_phys_rocks::MakePolygon(b2BodyId const& id_, b2ShapeDef& def_, float scale_, XY const* data_, size_t len_) {
	auto buf = (XY*)alloca(len_ * sizeof(XY));
	for (int32_t i = 0; i < len_; ++i) {
		buf[i] = data_[i] * scale_;
	}
	auto hull = b2ComputeHull((b2Vec2*)buf, len_);
	auto polygon = b2MakePolygon(&hull, 1);
	b2CreatePolygonShape(id_, &def_, &polygon);
};

XX_INLINE void ::_phys_rocks::MakeCircle(b2BodyId const& id_, b2ShapeDef& def_, float scale_, float x_, float y_, float r_) {
	auto circle = b2Circle{ .center = {x_ * scale_, y_ * scale_ }, .radius = r_ * scale_ };
	b2CreateCircleShape(id_, &def_, &circle);
};


void ::_phys_rocks::rock1_1::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys_rocks::rock1_1::fixture1::density;
			def_->material.friction = ::_phys_rocks::rock1_1::fixture1::friction;
			def_->material.restitution = ::_phys_rocks::rock1_1::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_1::fixture1::polygons1, _countof(::_phys_rocks::rock1_1::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_1::fixture1::polygons2, _countof(::_phys_rocks::rock1_1::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_1::fixture1::polygons3, _countof(::_phys_rocks::rock1_1::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_1::fixture1::polygons4, _countof(::_phys_rocks::rock1_1::fixture1::polygons4));
	};
}
void ::_phys_rocks::rock1_2::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys_rocks::rock1_2::fixture1::density;
			def_->material.friction = ::_phys_rocks::rock1_2::fixture1::friction;
			def_->material.restitution = ::_phys_rocks::rock1_2::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_2::fixture1::polygons1, _countof(::_phys_rocks::rock1_2::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_2::fixture1::polygons2, _countof(::_phys_rocks::rock1_2::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_2::fixture1::polygons3, _countof(::_phys_rocks::rock1_2::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_2::fixture1::polygons4, _countof(::_phys_rocks::rock1_2::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_2::fixture1::polygons5, _countof(::_phys_rocks::rock1_2::fixture1::polygons5));
	};
}
void ::_phys_rocks::rock1_3::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys_rocks::rock1_3::fixture1::density;
			def_->material.friction = ::_phys_rocks::rock1_3::fixture1::friction;
			def_->material.restitution = ::_phys_rocks::rock1_3::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_3::fixture1::polygons1, _countof(::_phys_rocks::rock1_3::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_3::fixture1::polygons2, _countof(::_phys_rocks::rock1_3::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_3::fixture1::polygons3, _countof(::_phys_rocks::rock1_3::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_3::fixture1::polygons4, _countof(::_phys_rocks::rock1_3::fixture1::polygons4));
	};
}
void ::_phys_rocks::rock1_4::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys_rocks::rock1_4::fixture1::density;
			def_->material.friction = ::_phys_rocks::rock1_4::fixture1::friction;
			def_->material.restitution = ::_phys_rocks::rock1_4::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_4::fixture1::polygons1, _countof(::_phys_rocks::rock1_4::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_4::fixture1::polygons2, _countof(::_phys_rocks::rock1_4::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_4::fixture1::polygons3, _countof(::_phys_rocks::rock1_4::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_4::fixture1::polygons4, _countof(::_phys_rocks::rock1_4::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_4::fixture1::polygons5, _countof(::_phys_rocks::rock1_4::fixture1::polygons5));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_4::fixture1::polygons6, _countof(::_phys_rocks::rock1_4::fixture1::polygons6));
	};
}
void ::_phys_rocks::rock1_5::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys_rocks::rock1_5::fixture1::density;
			def_->material.friction = ::_phys_rocks::rock1_5::fixture1::friction;
			def_->material.restitution = ::_phys_rocks::rock1_5::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_5::fixture1::polygons1, _countof(::_phys_rocks::rock1_5::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_5::fixture1::polygons2, _countof(::_phys_rocks::rock1_5::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_5::fixture1::polygons3, _countof(::_phys_rocks::rock1_5::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_5::fixture1::polygons4, _countof(::_phys_rocks::rock1_5::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_5::fixture1::polygons5, _countof(::_phys_rocks::rock1_5::fixture1::polygons5));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_5::fixture1::polygons6, _countof(::_phys_rocks::rock1_5::fixture1::polygons6));
	};
}
void ::_phys_rocks::rock1_6::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys_rocks::rock1_6::fixture1::density;
			def_->material.friction = ::_phys_rocks::rock1_6::fixture1::friction;
			def_->material.restitution = ::_phys_rocks::rock1_6::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_6::fixture1::polygons1, _countof(::_phys_rocks::rock1_6::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_6::fixture1::polygons2, _countof(::_phys_rocks::rock1_6::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_6::fixture1::polygons3, _countof(::_phys_rocks::rock1_6::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::rock1_6::fixture1::polygons4, _countof(::_phys_rocks::rock1_6::fixture1::polygons4));
	};
}
void ::_phys_rocks::bag_1::Init(b2BodyId const& id_, float scale_, b2ShapeDef* def_, char const* fixturesMask_) {
	auto def = b2DefaultShapeDef();
	bool isDefOverride{};
	if (!def_) def_ = &def;
	else isDefOverride = true;
	// fixture1
	if (!fixturesMask_ || fixturesMask_[1 - 1]) {
		if (!isDefOverride) {
			def_->density = ::_phys_rocks::bag_1::fixture1::density;
			def_->material.friction = ::_phys_rocks::bag_1::fixture1::friction;
			def_->material.restitution = ::_phys_rocks::bag_1::fixture1::restitution;
		}
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::bag_1::fixture1::polygons1, _countof(::_phys_rocks::bag_1::fixture1::polygons1));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::bag_1::fixture1::polygons2, _countof(::_phys_rocks::bag_1::fixture1::polygons2));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::bag_1::fixture1::polygons3, _countof(::_phys_rocks::bag_1::fixture1::polygons3));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::bag_1::fixture1::polygons4, _countof(::_phys_rocks::bag_1::fixture1::polygons4));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::bag_1::fixture1::polygons5, _countof(::_phys_rocks::bag_1::fixture1::polygons5));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::bag_1::fixture1::polygons6, _countof(::_phys_rocks::bag_1::fixture1::polygons6));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::bag_1::fixture1::polygons7, _countof(::_phys_rocks::bag_1::fixture1::polygons7));
		MakePolygon(id_, *def_, scale_, ::_phys_rocks::bag_1::fixture1::polygons8, _countof(::_phys_rocks::bag_1::fixture1::polygons8));
	};
}