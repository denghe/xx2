#include "pch.h"
#include "g.h"

namespace G {

	float Fish::Init(Scene* scene_, XY pos_, float scale_) {
		scene = scene_;
		scale = scale_;
		indexAtContainer = scene->fishs.len - 1;
		assert(scene->fishs[indexAtContainer].pointer == this);

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = (b2Vec2&)pos_;
		y = pos_.y;
		bodyDef.angularDamping = 1.f;
		//bodyDef.gravityScale = 2.f;
		bodyDef.linearVelocity = { 1000.f, 0 };
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		auto& ff = gg.rnd.NextElement(gg.fis);
		frame = *ff.first;

		auto def = b2DefaultShapeDef();
		def.enableSensorEvents = true;
		ff.second(b2body, scale_, &def, {});

		return frame.uvRect.h * scale_;
	}

	bool Fish::Update() {
		auto p = b2body.GetPos();
		y = p.y;
		return (p.x > 5000 || p.x < -5000 || p.y > 3000 || p.y < -3000);
	}

	void Fish::Draw() {
		auto [p, r] = b2body.GetPosRadians();
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(p), scale * scene->cam.scale, r);
	}

	void Fish::Dispose() {
		auto i = indexAtContainer;
		assert(scene->fishs[i].pointer == this);
		scene->fishs.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->fishs.SwapRemoveAt(i);
	}

}
