#include "pch.h"
#include "_phys.h"

void ::_phys::_10::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_10::fixture1::density;
		def.material.friction = ::_phys::_10::fixture1::friction;
		def.material.restitution = ::_phys::_10::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons1, _countof(::_phys::_10::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons2, _countof(::_phys::_10::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons3, _countof(::_phys::_10::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons4, _countof(::_phys::_10::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons5, _countof(::_phys::_10::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons6, _countof(::_phys::_10::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons7, _countof(::_phys::_10::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons8, _countof(::_phys::_10::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons9, _countof(::_phys::_10::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons10, _countof(::_phys::_10::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture1::polygons11, _countof(::_phys::_10::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
	// fixture2
	{
		def.density = ::_phys::_10::fixture2::density;
		def.material.friction = ::_phys::_10::fixture2::friction;
		def.material.restitution = ::_phys::_10::fixture2::restitution;
		{
			auto circle = b2Circle{ .center = { ::_phys::_10::fixture2::circleRXY[1], ::_phys::_10::fixture2::circleRXY[2] }, .radius = ::_phys::_10::fixture2::circleRXY[0] };
			b2CreateCircleShape(id_, &def, &circle);
		}
	}
	// fixture3
	{
		def.density = ::_phys::_10::fixture3::density;
		def.material.friction = ::_phys::_10::fixture3::friction;
		def.material.restitution = ::_phys::_10::fixture3::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_10::fixture3::polygons1, _countof(::_phys::_10::fixture3::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_110::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_110::fixture1::density;
		def.material.friction = ::_phys::_110::fixture1::friction;
		def.material.restitution = ::_phys::_110::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_110::fixture1::polygons1, _countof(::_phys::_110::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_110::fixture1::polygons2, _countof(::_phys::_110::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_110::fixture1::polygons3, _countof(::_phys::_110::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_110::fixture1::polygons4, _countof(::_phys::_110::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_110::fixture1::polygons5, _countof(::_phys::_110::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_110::fixture1::polygons6, _countof(::_phys::_110::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_110::fixture1::polygons7, _countof(::_phys::_110::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_123::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_123::fixture1::density;
		def.material.friction = ::_phys::_123::fixture1::friction;
		def.material.restitution = ::_phys::_123::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_123::fixture1::polygons1, _countof(::_phys::_123::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_123::fixture1::polygons2, _countof(::_phys::_123::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_123::fixture1::polygons3, _countof(::_phys::_123::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_123::fixture1::polygons4, _countof(::_phys::_123::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_123::fixture1::polygons5, _countof(::_phys::_123::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_123::fixture1::polygons6, _countof(::_phys::_123::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_123::fixture1::polygons7, _countof(::_phys::_123::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_128::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_128::fixture1::density;
		def.material.friction = ::_phys::_128::fixture1::friction;
		def.material.restitution = ::_phys::_128::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons1, _countof(::_phys::_128::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons2, _countof(::_phys::_128::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons3, _countof(::_phys::_128::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons4, _countof(::_phys::_128::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons5, _countof(::_phys::_128::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons6, _countof(::_phys::_128::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons7, _countof(::_phys::_128::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons8, _countof(::_phys::_128::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons9, _countof(::_phys::_128::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons10, _countof(::_phys::_128::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons11, _countof(::_phys::_128::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons12, _countof(::_phys::_128::fixture1::polygons12));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons13, _countof(::_phys::_128::fixture1::polygons13));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons14, _countof(::_phys::_128::fixture1::polygons14));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons15, _countof(::_phys::_128::fixture1::polygons15));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons16, _countof(::_phys::_128::fixture1::polygons16));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_128::fixture1::polygons17, _countof(::_phys::_128::fixture1::polygons17));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_138::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_138::fixture1::density;
		def.material.friction = ::_phys::_138::fixture1::friction;
		def.material.restitution = ::_phys::_138::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_138::fixture1::polygons1, _countof(::_phys::_138::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_138::fixture1::polygons2, _countof(::_phys::_138::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_138::fixture1::polygons3, _countof(::_phys::_138::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_138::fixture1::polygons4, _countof(::_phys::_138::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_138::fixture1::polygons5, _countof(::_phys::_138::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_138::fixture1::polygons6, _countof(::_phys::_138::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_138::fixture1::polygons7, _countof(::_phys::_138::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_14::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_14::fixture1::density;
		def.material.friction = ::_phys::_14::fixture1::friction;
		def.material.restitution = ::_phys::_14::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_14::fixture1::polygons1, _countof(::_phys::_14::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_14::fixture1::polygons2, _countof(::_phys::_14::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_14::fixture1::polygons3, _countof(::_phys::_14::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_14::fixture1::polygons4, _countof(::_phys::_14::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_14::fixture1::polygons5, _countof(::_phys::_14::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_14::fixture1::polygons6, _countof(::_phys::_14::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_14::fixture1::polygons7, _countof(::_phys::_14::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_14::fixture1::polygons8, _countof(::_phys::_14::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_14::fixture1::polygons9, _countof(::_phys::_14::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_14::fixture1::polygons10, _countof(::_phys::_14::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_147::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_147::fixture1::density;
		def.material.friction = ::_phys::_147::fixture1::friction;
		def.material.restitution = ::_phys::_147::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons1, _countof(::_phys::_147::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons2, _countof(::_phys::_147::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons3, _countof(::_phys::_147::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons4, _countof(::_phys::_147::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons5, _countof(::_phys::_147::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons6, _countof(::_phys::_147::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons7, _countof(::_phys::_147::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons8, _countof(::_phys::_147::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons9, _countof(::_phys::_147::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons10, _countof(::_phys::_147::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_147::fixture1::polygons11, _countof(::_phys::_147::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_168::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_168::fixture1::density;
		def.material.friction = ::_phys::_168::fixture1::friction;
		def.material.restitution = ::_phys::_168::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons1, _countof(::_phys::_168::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons2, _countof(::_phys::_168::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons3, _countof(::_phys::_168::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons4, _countof(::_phys::_168::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons5, _countof(::_phys::_168::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons6, _countof(::_phys::_168::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons7, _countof(::_phys::_168::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons8, _countof(::_phys::_168::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons9, _countof(::_phys::_168::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons10, _countof(::_phys::_168::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons11, _countof(::_phys::_168::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_168::fixture1::polygons12, _countof(::_phys::_168::fixture1::polygons12));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_17::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_17::fixture1::density;
		def.material.friction = ::_phys::_17::fixture1::friction;
		def.material.restitution = ::_phys::_17::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons1, _countof(::_phys::_17::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons2, _countof(::_phys::_17::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons3, _countof(::_phys::_17::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons4, _countof(::_phys::_17::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons5, _countof(::_phys::_17::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons6, _countof(::_phys::_17::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons7, _countof(::_phys::_17::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons8, _countof(::_phys::_17::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons9, _countof(::_phys::_17::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons10, _countof(::_phys::_17::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_17::fixture1::polygons11, _countof(::_phys::_17::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_18::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_18::fixture1::density;
		def.material.friction = ::_phys::_18::fixture1::friction;
		def.material.restitution = ::_phys::_18::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons1, _countof(::_phys::_18::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons2, _countof(::_phys::_18::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons3, _countof(::_phys::_18::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons4, _countof(::_phys::_18::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons5, _countof(::_phys::_18::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons6, _countof(::_phys::_18::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons7, _countof(::_phys::_18::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons8, _countof(::_phys::_18::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons9, _countof(::_phys::_18::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons10, _countof(::_phys::_18::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons11, _countof(::_phys::_18::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons12, _countof(::_phys::_18::fixture1::polygons12));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_18::fixture1::polygons13, _countof(::_phys::_18::fixture1::polygons13));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_192::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_192::fixture1::density;
		def.material.friction = ::_phys::_192::fixture1::friction;
		def.material.restitution = ::_phys::_192::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons1, _countof(::_phys::_192::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons2, _countof(::_phys::_192::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons3, _countof(::_phys::_192::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons4, _countof(::_phys::_192::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons5, _countof(::_phys::_192::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons6, _countof(::_phys::_192::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons7, _countof(::_phys::_192::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons8, _countof(::_phys::_192::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons9, _countof(::_phys::_192::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons10, _countof(::_phys::_192::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons11, _countof(::_phys::_192::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons12, _countof(::_phys::_192::fixture1::polygons12));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons13, _countof(::_phys::_192::fixture1::polygons13));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons14, _countof(::_phys::_192::fixture1::polygons14));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons15, _countof(::_phys::_192::fixture1::polygons15));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons16, _countof(::_phys::_192::fixture1::polygons16));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons17, _countof(::_phys::_192::fixture1::polygons17));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons18, _countof(::_phys::_192::fixture1::polygons18));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons19, _countof(::_phys::_192::fixture1::polygons19));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons20, _countof(::_phys::_192::fixture1::polygons20));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons21, _countof(::_phys::_192::fixture1::polygons21));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons22, _countof(::_phys::_192::fixture1::polygons22));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons23, _countof(::_phys::_192::fixture1::polygons23));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons24, _countof(::_phys::_192::fixture1::polygons24));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons25, _countof(::_phys::_192::fixture1::polygons25));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_192::fixture1::polygons26, _countof(::_phys::_192::fixture1::polygons26));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_206::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_206::fixture1::density;
		def.material.friction = ::_phys::_206::fixture1::friction;
		def.material.restitution = ::_phys::_206::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons1, _countof(::_phys::_206::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons2, _countof(::_phys::_206::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons3, _countof(::_phys::_206::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons4, _countof(::_phys::_206::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons5, _countof(::_phys::_206::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons6, _countof(::_phys::_206::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons7, _countof(::_phys::_206::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons8, _countof(::_phys::_206::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons9, _countof(::_phys::_206::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons10, _countof(::_phys::_206::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons11, _countof(::_phys::_206::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons12, _countof(::_phys::_206::fixture1::polygons12));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons13, _countof(::_phys::_206::fixture1::polygons13));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_206::fixture1::polygons14, _countof(::_phys::_206::fixture1::polygons14));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_243::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_243::fixture1::density;
		def.material.friction = ::_phys::_243::fixture1::friction;
		def.material.restitution = ::_phys::_243::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons1, _countof(::_phys::_243::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons2, _countof(::_phys::_243::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons3, _countof(::_phys::_243::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons4, _countof(::_phys::_243::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons5, _countof(::_phys::_243::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons6, _countof(::_phys::_243::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons7, _countof(::_phys::_243::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons8, _countof(::_phys::_243::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons9, _countof(::_phys::_243::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons10, _countof(::_phys::_243::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons11, _countof(::_phys::_243::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_243::fixture1::polygons12, _countof(::_phys::_243::fixture1::polygons12));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_244::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_244::fixture1::density;
		def.material.friction = ::_phys::_244::fixture1::friction;
		def.material.restitution = ::_phys::_244::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons1, _countof(::_phys::_244::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons2, _countof(::_phys::_244::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons3, _countof(::_phys::_244::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons4, _countof(::_phys::_244::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons5, _countof(::_phys::_244::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons6, _countof(::_phys::_244::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons7, _countof(::_phys::_244::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons8, _countof(::_phys::_244::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons9, _countof(::_phys::_244::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons10, _countof(::_phys::_244::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_244::fixture1::polygons11, _countof(::_phys::_244::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_26::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_26::fixture1::density;
		def.material.friction = ::_phys::_26::fixture1::friction;
		def.material.restitution = ::_phys::_26::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons1, _countof(::_phys::_26::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons2, _countof(::_phys::_26::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons3, _countof(::_phys::_26::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons4, _countof(::_phys::_26::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons5, _countof(::_phys::_26::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons6, _countof(::_phys::_26::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons7, _countof(::_phys::_26::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons8, _countof(::_phys::_26::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons9, _countof(::_phys::_26::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons10, _countof(::_phys::_26::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_26::fixture1::polygons11, _countof(::_phys::_26::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_283::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_283::fixture1::density;
		def.material.friction = ::_phys::_283::fixture1::friction;
		def.material.restitution = ::_phys::_283::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_283::fixture1::polygons1, _countof(::_phys::_283::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_283::fixture1::polygons2, _countof(::_phys::_283::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_283::fixture1::polygons3, _countof(::_phys::_283::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_283::fixture1::polygons4, _countof(::_phys::_283::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_283::fixture1::polygons5, _countof(::_phys::_283::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_285::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_285::fixture1::density;
		def.material.friction = ::_phys::_285::fixture1::friction;
		def.material.restitution = ::_phys::_285::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_285::fixture1::polygons1, _countof(::_phys::_285::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_285::fixture1::polygons2, _countof(::_phys::_285::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_285::fixture1::polygons3, _countof(::_phys::_285::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_285::fixture1::polygons4, _countof(::_phys::_285::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_285::fixture1::polygons5, _countof(::_phys::_285::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_285::fixture1::polygons6, _countof(::_phys::_285::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_285::fixture1::polygons7, _countof(::_phys::_285::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_288::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_288::fixture1::density;
		def.material.friction = ::_phys::_288::fixture1::friction;
		def.material.restitution = ::_phys::_288::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons1, _countof(::_phys::_288::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons2, _countof(::_phys::_288::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons3, _countof(::_phys::_288::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons4, _countof(::_phys::_288::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons5, _countof(::_phys::_288::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons6, _countof(::_phys::_288::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons7, _countof(::_phys::_288::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons8, _countof(::_phys::_288::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons9, _countof(::_phys::_288::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons10, _countof(::_phys::_288::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_288::fixture1::polygons11, _countof(::_phys::_288::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_296::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_296::fixture1::density;
		def.material.friction = ::_phys::_296::fixture1::friction;
		def.material.restitution = ::_phys::_296::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_296::fixture1::polygons1, _countof(::_phys::_296::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_296::fixture1::polygons2, _countof(::_phys::_296::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_296::fixture1::polygons3, _countof(::_phys::_296::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_296::fixture1::polygons4, _countof(::_phys::_296::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_296::fixture1::polygons5, _countof(::_phys::_296::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_296::fixture1::polygons6, _countof(::_phys::_296::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_296::fixture1::polygons7, _countof(::_phys::_296::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_296::fixture1::polygons8, _countof(::_phys::_296::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_303::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_303::fixture1::density;
		def.material.friction = ::_phys::_303::fixture1::friction;
		def.material.restitution = ::_phys::_303::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons1, _countof(::_phys::_303::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons2, _countof(::_phys::_303::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons3, _countof(::_phys::_303::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons4, _countof(::_phys::_303::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons5, _countof(::_phys::_303::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons6, _countof(::_phys::_303::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons7, _countof(::_phys::_303::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons8, _countof(::_phys::_303::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons9, _countof(::_phys::_303::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons10, _countof(::_phys::_303::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons11, _countof(::_phys::_303::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_303::fixture1::polygons12, _countof(::_phys::_303::fixture1::polygons12));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_309::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_309::fixture1::density;
		def.material.friction = ::_phys::_309::fixture1::friction;
		def.material.restitution = ::_phys::_309::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_309::fixture1::polygons1, _countof(::_phys::_309::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_309::fixture1::polygons2, _countof(::_phys::_309::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_309::fixture1::polygons3, _countof(::_phys::_309::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_309::fixture1::polygons4, _countof(::_phys::_309::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_309::fixture1::polygons5, _countof(::_phys::_309::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_309::fixture1::polygons6, _countof(::_phys::_309::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_309::fixture1::polygons7, _countof(::_phys::_309::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_309::fixture1::polygons8, _countof(::_phys::_309::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_402::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_402::fixture1::density;
		def.material.friction = ::_phys::_402::fixture1::friction;
		def.material.restitution = ::_phys::_402::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_402::fixture1::polygons1, _countof(::_phys::_402::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_402::fixture1::polygons2, _countof(::_phys::_402::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_402::fixture1::polygons3, _countof(::_phys::_402::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_402::fixture1::polygons4, _countof(::_phys::_402::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_402::fixture1::polygons5, _countof(::_phys::_402::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_402::fixture1::polygons6, _countof(::_phys::_402::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_402::fixture1::polygons7, _countof(::_phys::_402::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_407::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_407::fixture1::density;
		def.material.friction = ::_phys::_407::fixture1::friction;
		def.material.restitution = ::_phys::_407::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_407::fixture1::polygons1, _countof(::_phys::_407::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_407::fixture1::polygons2, _countof(::_phys::_407::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_407::fixture1::polygons3, _countof(::_phys::_407::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_407::fixture1::polygons4, _countof(::_phys::_407::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_407::fixture1::polygons5, _countof(::_phys::_407::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_407::fixture1::polygons6, _countof(::_phys::_407::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_407::fixture1::polygons7, _countof(::_phys::_407::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_408::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_408::fixture1::density;
		def.material.friction = ::_phys::_408::fixture1::friction;
		def.material.restitution = ::_phys::_408::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons1, _countof(::_phys::_408::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons2, _countof(::_phys::_408::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons3, _countof(::_phys::_408::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons4, _countof(::_phys::_408::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons5, _countof(::_phys::_408::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons6, _countof(::_phys::_408::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons7, _countof(::_phys::_408::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons8, _countof(::_phys::_408::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons9, _countof(::_phys::_408::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons10, _countof(::_phys::_408::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_408::fixture1::polygons11, _countof(::_phys::_408::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_415::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_415::fixture1::density;
		def.material.friction = ::_phys::_415::fixture1::friction;
		def.material.restitution = ::_phys::_415::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_415::fixture1::polygons1, _countof(::_phys::_415::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_415::fixture1::polygons2, _countof(::_phys::_415::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_415::fixture1::polygons3, _countof(::_phys::_415::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_415::fixture1::polygons4, _countof(::_phys::_415::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_415::fixture1::polygons5, _countof(::_phys::_415::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_415::fixture1::polygons6, _countof(::_phys::_415::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_421::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_421::fixture1::density;
		def.material.friction = ::_phys::_421::fixture1::friction;
		def.material.restitution = ::_phys::_421::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_421::fixture1::polygons1, _countof(::_phys::_421::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_421::fixture1::polygons2, _countof(::_phys::_421::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_421::fixture1::polygons3, _countof(::_phys::_421::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_421::fixture1::polygons4, _countof(::_phys::_421::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_421::fixture1::polygons5, _countof(::_phys::_421::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_421::fixture1::polygons6, _countof(::_phys::_421::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_421::fixture1::polygons7, _countof(::_phys::_421::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_421::fixture1::polygons8, _countof(::_phys::_421::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_421::fixture1::polygons9, _countof(::_phys::_421::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_421::fixture1::polygons10, _countof(::_phys::_421::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_422::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_422::fixture1::density;
		def.material.friction = ::_phys::_422::fixture1::friction;
		def.material.restitution = ::_phys::_422::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_422::fixture1::polygons1, _countof(::_phys::_422::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_422::fixture1::polygons2, _countof(::_phys::_422::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_422::fixture1::polygons3, _countof(::_phys::_422::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_422::fixture1::polygons4, _countof(::_phys::_422::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_422::fixture1::polygons5, _countof(::_phys::_422::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_422::fixture1::polygons6, _countof(::_phys::_422::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_422::fixture1::polygons7, _countof(::_phys::_422::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_422::fixture1::polygons8, _countof(::_phys::_422::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_443::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_443::fixture1::density;
		def.material.friction = ::_phys::_443::fixture1::friction;
		def.material.restitution = ::_phys::_443::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_443::fixture1::polygons1, _countof(::_phys::_443::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_443::fixture1::polygons2, _countof(::_phys::_443::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_443::fixture1::polygons3, _countof(::_phys::_443::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_443::fixture1::polygons4, _countof(::_phys::_443::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_443::fixture1::polygons5, _countof(::_phys::_443::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_443::fixture1::polygons6, _countof(::_phys::_443::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_443::fixture1::polygons7, _countof(::_phys::_443::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_443::fixture1::polygons8, _countof(::_phys::_443::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_443::fixture1::polygons9, _countof(::_phys::_443::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_443::fixture1::polygons10, _countof(::_phys::_443::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_449::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_449::fixture1::density;
		def.material.friction = ::_phys::_449::fixture1::friction;
		def.material.restitution = ::_phys::_449::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_449::fixture1::polygons1, _countof(::_phys::_449::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_449::fixture1::polygons2, _countof(::_phys::_449::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_449::fixture1::polygons3, _countof(::_phys::_449::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_449::fixture1::polygons4, _countof(::_phys::_449::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_449::fixture1::polygons5, _countof(::_phys::_449::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_449::fixture1::polygons6, _countof(::_phys::_449::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_449::fixture1::polygons7, _countof(::_phys::_449::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_449::fixture1::polygons8, _countof(::_phys::_449::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_449::fixture1::polygons9, _countof(::_phys::_449::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_449::fixture1::polygons10, _countof(::_phys::_449::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_458::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_458::fixture1::density;
		def.material.friction = ::_phys::_458::fixture1::friction;
		def.material.restitution = ::_phys::_458::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons1, _countof(::_phys::_458::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons2, _countof(::_phys::_458::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons3, _countof(::_phys::_458::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons4, _countof(::_phys::_458::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons5, _countof(::_phys::_458::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons6, _countof(::_phys::_458::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons7, _countof(::_phys::_458::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons8, _countof(::_phys::_458::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons9, _countof(::_phys::_458::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons10, _countof(::_phys::_458::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons11, _countof(::_phys::_458::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons12, _countof(::_phys::_458::fixture1::polygons12));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons13, _countof(::_phys::_458::fixture1::polygons13));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons14, _countof(::_phys::_458::fixture1::polygons14));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons15, _countof(::_phys::_458::fixture1::polygons15));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons16, _countof(::_phys::_458::fixture1::polygons16));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons17, _countof(::_phys::_458::fixture1::polygons17));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons18, _countof(::_phys::_458::fixture1::polygons18));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons19, _countof(::_phys::_458::fixture1::polygons19));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons20, _countof(::_phys::_458::fixture1::polygons20));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons21, _countof(::_phys::_458::fixture1::polygons21));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_458::fixture1::polygons22, _countof(::_phys::_458::fixture1::polygons22));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_461::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_461::fixture1::density;
		def.material.friction = ::_phys::_461::fixture1::friction;
		def.material.restitution = ::_phys::_461::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_461::fixture1::polygons1, _countof(::_phys::_461::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_461::fixture1::polygons2, _countof(::_phys::_461::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_461::fixture1::polygons3, _countof(::_phys::_461::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_461::fixture1::polygons4, _countof(::_phys::_461::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_461::fixture1::polygons5, _countof(::_phys::_461::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_461::fixture1::polygons6, _countof(::_phys::_461::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_461::fixture1::polygons7, _countof(::_phys::_461::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_461::fixture1::polygons8, _countof(::_phys::_461::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_461::fixture1::polygons9, _countof(::_phys::_461::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_461::fixture1::polygons10, _countof(::_phys::_461::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_462::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_462::fixture1::density;
		def.material.friction = ::_phys::_462::fixture1::friction;
		def.material.restitution = ::_phys::_462::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons1, _countof(::_phys::_462::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons2, _countof(::_phys::_462::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons3, _countof(::_phys::_462::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons4, _countof(::_phys::_462::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons5, _countof(::_phys::_462::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons6, _countof(::_phys::_462::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons7, _countof(::_phys::_462::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons8, _countof(::_phys::_462::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons9, _countof(::_phys::_462::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons10, _countof(::_phys::_462::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons11, _countof(::_phys::_462::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_462::fixture1::polygons12, _countof(::_phys::_462::fixture1::polygons12));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_463::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_463::fixture1::density;
		def.material.friction = ::_phys::_463::fixture1::friction;
		def.material.restitution = ::_phys::_463::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons1, _countof(::_phys::_463::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons2, _countof(::_phys::_463::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons3, _countof(::_phys::_463::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons4, _countof(::_phys::_463::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons5, _countof(::_phys::_463::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons6, _countof(::_phys::_463::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons7, _countof(::_phys::_463::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons8, _countof(::_phys::_463::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons9, _countof(::_phys::_463::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons10, _countof(::_phys::_463::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_463::fixture1::polygons11, _countof(::_phys::_463::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_464::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_464::fixture1::density;
		def.material.friction = ::_phys::_464::fixture1::friction;
		def.material.restitution = ::_phys::_464::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons1, _countof(::_phys::_464::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons2, _countof(::_phys::_464::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons3, _countof(::_phys::_464::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons4, _countof(::_phys::_464::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons5, _countof(::_phys::_464::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons6, _countof(::_phys::_464::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons7, _countof(::_phys::_464::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons8, _countof(::_phys::_464::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons9, _countof(::_phys::_464::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons10, _countof(::_phys::_464::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_464::fixture1::polygons11, _countof(::_phys::_464::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_467::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_467::fixture1::density;
		def.material.friction = ::_phys::_467::fixture1::friction;
		def.material.restitution = ::_phys::_467::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons1, _countof(::_phys::_467::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons2, _countof(::_phys::_467::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons3, _countof(::_phys::_467::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons4, _countof(::_phys::_467::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons5, _countof(::_phys::_467::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons6, _countof(::_phys::_467::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons7, _countof(::_phys::_467::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons8, _countof(::_phys::_467::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons9, _countof(::_phys::_467::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons10, _countof(::_phys::_467::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_467::fixture1::polygons11, _countof(::_phys::_467::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_470::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_470::fixture1::density;
		def.material.friction = ::_phys::_470::fixture1::friction;
		def.material.restitution = ::_phys::_470::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons1, _countof(::_phys::_470::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons2, _countof(::_phys::_470::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons3, _countof(::_phys::_470::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons4, _countof(::_phys::_470::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons5, _countof(::_phys::_470::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons6, _countof(::_phys::_470::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons7, _countof(::_phys::_470::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons8, _countof(::_phys::_470::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons9, _countof(::_phys::_470::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons10, _countof(::_phys::_470::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons11, _countof(::_phys::_470::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons12, _countof(::_phys::_470::fixture1::polygons12));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_470::fixture1::polygons13, _countof(::_phys::_470::fixture1::polygons13));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_472::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_472::fixture1::density;
		def.material.friction = ::_phys::_472::fixture1::friction;
		def.material.restitution = ::_phys::_472::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons1, _countof(::_phys::_472::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons2, _countof(::_phys::_472::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons3, _countof(::_phys::_472::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons4, _countof(::_phys::_472::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons5, _countof(::_phys::_472::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons6, _countof(::_phys::_472::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons7, _countof(::_phys::_472::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons8, _countof(::_phys::_472::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons9, _countof(::_phys::_472::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons10, _countof(::_phys::_472::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_472::fixture1::polygons11, _countof(::_phys::_472::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_51::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_51::fixture1::density;
		def.material.friction = ::_phys::_51::fixture1::friction;
		def.material.restitution = ::_phys::_51::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons1, _countof(::_phys::_51::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons2, _countof(::_phys::_51::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons3, _countof(::_phys::_51::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons4, _countof(::_phys::_51::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons5, _countof(::_phys::_51::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons6, _countof(::_phys::_51::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons7, _countof(::_phys::_51::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons8, _countof(::_phys::_51::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons9, _countof(::_phys::_51::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons10, _countof(::_phys::_51::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_51::fixture1::polygons11, _countof(::_phys::_51::fixture1::polygons11));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_75::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_75::fixture1::density;
		def.material.friction = ::_phys::_75::fixture1::friction;
		def.material.restitution = ::_phys::_75::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_75::fixture1::polygons1, _countof(::_phys::_75::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_75::fixture1::polygons2, _countof(::_phys::_75::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_75::fixture1::polygons3, _countof(::_phys::_75::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_75::fixture1::polygons4, _countof(::_phys::_75::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_75::fixture1::polygons5, _countof(::_phys::_75::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_75::fixture1::polygons6, _countof(::_phys::_75::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_75::fixture1::polygons7, _countof(::_phys::_75::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_75::fixture1::polygons8, _countof(::_phys::_75::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_75::fixture1::polygons9, _countof(::_phys::_75::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_75::fixture1::polygons10, _countof(::_phys::_75::fixture1::polygons10));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}
void ::_phys::_98::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::_phys::_98::fixture1::density;
		def.material.friction = ::_phys::_98::fixture1::friction;
		def.material.restitution = ::_phys::_98::fixture1::restitution;
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_98::fixture1::polygons1, _countof(::_phys::_98::fixture1::polygons1));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_98::fixture1::polygons2, _countof(::_phys::_98::fixture1::polygons2));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_98::fixture1::polygons3, _countof(::_phys::_98::fixture1::polygons3));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_98::fixture1::polygons4, _countof(::_phys::_98::fixture1::polygons4));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_98::fixture1::polygons5, _countof(::_phys::_98::fixture1::polygons5));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_98::fixture1::polygons6, _countof(::_phys::_98::fixture1::polygons6));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_98::fixture1::polygons7, _countof(::_phys::_98::fixture1::polygons7));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_98::fixture1::polygons8, _countof(::_phys::_98::fixture1::polygons8));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		{
			auto hull = b2ComputeHull((b2Vec2*)::_phys::_98::fixture1::polygons9, _countof(::_phys::_98::fixture1::polygons9));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
	}
}