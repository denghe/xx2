#include "pch.h"
#include "scene_test7.h"
#include "scene_mainmenu.h"

namespace Test7 {

	bool SceneBase::Update() {
		auto [p, r] = b2body.GetPosRadians();
		pos = p;
		y = p.y;
		radians = r;
		return false;
	}

	void SceneBase::Draw() {
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(pos), scene->cam.scale, radians);
	}

	/***************************************************************************************/

	void Wall::Init(Scene* scene_) {
		frame = gg.pics.bag_[0];
		scene = scene_;
		pos = {};
		y = pos.y;
		scale = 1.f;

		auto len = b2HullPoints.size();
		assert(_countof(cHullPoints) == len);
		for (size_t i = 0; i < len; ++i) {
			(XY&)b2HullPoints[i] = cHullPoints[len - 1 - i] * scale;
		}

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.position = (b2Vec2&)pos;
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		// ground chain
		b2ChainDef chainDef = b2DefaultChainDef();
		chainDef.points = b2HullPoints.data();
		chainDef.count = b2HullPoints.size();
		chainDef.isLoop = true;
		auto chainId = b2CreateChain(b2body, &chainDef);
	}

	void Wall::Draw() {
		gg.Quad().DrawFrame(gg.pics.bag_bg_[0], scene->cam.ToGLPos(pos), scale * scene->cam.scale, radians);
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(pos), scale * scene->cam.scale, radians, 0.2f);
		auto basePos = pos;
		auto&& line = gg.Line();
		auto len = b2HullPoints.size();
		auto ptr = line.Alloc(len + 1);
		for (size_t i = 0; i < len; ++i) {
			ptr[i] = (xx::Shader_LineData&)(scene->cam.ToGLPos(basePos + b2HullPoints[i]));
			(xx::RGBA8&)ptr[i] = xx::RGBA8_Red;
		}
		ptr[len] = ptr[0];
	}

	/***************************************************************************************/

	void Rock::Init(Scene* scene_, XY pos_) {
		scene = scene_;
		pos = pos_;
		y = pos_.y;

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = (b2Vec2&)pos_;
		bodyDef.angularDamping = 0.f;
		bodyDef.linearVelocity = { 0, 0 };
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		auto o = gg.rnd.NextElement(gg.rocksFramePhysMaps);
		frame = *std::get<0>(o);

		auto def = b2DefaultShapeDef();
		def.material.friction = 0.2;
		def.material.restitution = 0.8;
		def.enableSensorEvents = true;
		auto& buf = std::get<1>(o);
		auto& len = std::get<2>(o);
		//for (size_t i = 0; i < len; ++i) {
		//	buf[i] = buf[i] * scale_;
		//}
		auto hull = b2ComputeHull((b2Vec2*)buf, len);
		auto polygon = b2MakePolygon(&hull, 1);
		b2CreatePolygonShape(b2body, &def, &polygon);
	}

	bool Rock::Update() {
		if (SceneBase::Update()) return true;
		return (pos.x < 0 || pos.x > 1920
			|| pos.y < 0 || pos.y > 1080);
	}

	/***************************************************************************************/

	void Scene::Init() {
		cam.Init(gg.scale, 1.f, gg.designSize / 2);
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		auto def = b2DefaultWorldDef();
		def.gravity = { 0, 0 };
		def.maximumLinearSpeed = 1000 * b2GetLengthUnitsPerMeter();
		def.contactSpeed = 1000.0f * b2GetLengthUnitsPerMeter();
		//def.contactHertz = 120.0;
		//def.contactDampingRatio = 0;
		b2world.InitDef(def, 1);

		wall.Emplace()->Init(this);
	}

	void Scene::Gen(int32_t num_) {
		for (int i = 0; i < num_; ++i) {
			//XY pos;
			//pos.x = gg.rnd.Next(-800, 800);
			//pos.y = gg.rnd.Next(-800, 800);
			//if (gg.rnd.Next<bool>())
			//	item1s.Emplace().Emplace()->Init(this, pos, 5);
			//else 
			//	item2s.Emplace().Emplace()->Init(this, pos, 5);
		}
	}

	void Scene::Update() {
		// handle inputs
		if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
			gg.MakeScene<Scene_MainMenu>()->Init();
			return;
		}

		if (gg.mouse[GLFW_MOUSE_BUTTON_1]) {
			rocks.Emplace().Emplace()->Init(this, cam.ToLogicPos(gg.mousePos));
		}

		// fixed update
		auto d = float(std::min((float)gg.delta, gg.cMaxDelta) * timeScale);
		timePool += d;
		while (timePool >= gg.cDelta) {
			time += gg.cDelta;
			timePool -= gg.cDelta;
			FixedUpdate();
		}
	}

	void Scene::FixedUpdate() {
		wall->Update();
		for (auto i = rocks.len - 1; i >= 0; --i) {
			if (rocks[i]->Update()) rocks.SwapRemoveAt(i);
		}

		b2world.Step();

		genTimer += gg.cDelta * 300.f;
		if (genTimer >= 1.f) {
			auto n = (int32_t)genTimer;
			genTimer -= n;
			Gen(n);
		}
	}

	void Scene::Draw() {
		wall->Draw();
		for (auto& o : rocks) o->Draw();

		gg.uiText->SetText(xx::ToString("num rocks = ", rocks.len));
		gg.SetBlendPremultipliedAlpha(false);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}

}
