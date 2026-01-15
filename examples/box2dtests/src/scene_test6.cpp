#include "pch.h"
#include "scene_test6.h"
#include "scene_mainmenu.h"

namespace Test6 {

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

	void Bag::Init(Scene* scene_, XY pos_) {
		frame = gg.pics.bag_[0];
		scene = scene_;
		pos = pos_;
		y = pos_.y;

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.position = (b2Vec2&)pos_;
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		// ground chain
		b2ChainDef chainDef = b2DefaultChainDef();
		chainDef.points = (b2Vec2*)cHullPoints;
		chainDef.count = _countof(cHullPoints);
		//chainDef.enableSensorEvents = true;
		chainDef.isLoop = true;
		auto chainId = b2CreateChain(b2body, &chainDef);
		//xx::CoutN(chainId.index1);
	}

	/***************************************************************************************/

	void Rock::Init(Scene* scene_, XY pos_) {
		scene = scene_;
		pos = pos_;
		y = pos_.y;

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = (b2Vec2&)pos_;
		bodyDef.angularDamping = 1.f;
		//bodyDef.gravityScale = 2.f;
		bodyDef.linearVelocity = { 0, 1000.f };
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		auto& fp = gg.rnd.NextElement(gg.rocksFramePhysMaps);
		frame = *fp.first;

		auto def = b2DefaultShapeDef();
		def.enableSensorEvents = true;
		fp.second(b2body, 1, &def, {});
	}

	/***************************************************************************************/

	void Scene::Init() {
		cam.Init(gg.scale, 1.f);
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		auto def = b2DefaultWorldDef();
		def.gravity = {};
		//def.restitutionThreshold = 10.0f * b2GetLengthUnitsPerMeter();
		//def.hitEventThreshold = 10.0f * b2GetLengthUnitsPerMeter();
		b2world.InitDef(def, 1);

		bag.Emplace()->Init(this, { 0, 0.f });

		rocks.Emplace().Emplace()->Init(this, {0.f, -100.f});
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
		bag->Update();
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
		bag->Draw();
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
