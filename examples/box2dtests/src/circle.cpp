#include "pch.h"
#include "g.h"

namespace G {

	void Circle::Init(Scene* scene_, XY pos_, float radius_) {
		scene = scene_;
		radius = radius_;
		b2body.InitTypePos(scene_->b2world, pos_, b2_staticBody);
		auto def = b2DefaultShapeDef();
		auto circle = b2Circle{ .center = {}, .radius = radius_ };
		b2CreateCircleShape(b2body, &def, &circle);
	}

	bool Circle::Update() {
		auto mp = scene->cam.ToLogicPos(gg.mousePos);// .FlipY();
		b2Body_SetTransform(b2body, (b2Vec2&)mp, {1,0});
		return false;
	}

	void Circle::Draw() {
		auto [p, r] = b2body.GetPosRadians();
		gg.Quad().DrawFrame(gg.fs.circle256, scene->cam.ToGLPos(p), radius * 2.f / 256.f * scene->cam.scale, r);
	}

}
