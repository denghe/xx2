#include "pch.h"
#include "scene_test2.h"
#include "scene_mainmenu.h"

namespace Test2 {

	float SceneItem1::Init(Scene* scene_, XY pos_, float scale_) {
		scene = scene_;
		scale = scale_;

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = (b2Vec2&)pos_;
		bodyDef.angularDamping = 1.f;
		//bodyDef.gravityScale = 2.f;
		bodyDef.linearVelocity = { 1000.f, 0 };
		b2body.InitDef(scene_->b2world, bodyDef);

		auto& ff = gg.rnd.NextElement(scene_->frameAndFuncs);
		frame = ff.first;
		ff.second(b2body, scale_);
		return frame.uvRect.h * scale_;
	}

	bool SceneItem1::Update() {
		auto p = b2body.GetPos();
		return (p.x > 5000 || p.x < -5000 || p.y > 3000 || p.y < -3000);
	}

	void SceneItem1::Draw() {
		auto [p, r] = b2body.GetPosRadians();
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(p), scale * scene->cam.scale, r);
	}

	/***************************************************************************************/


	void SceneItem2::Init(Scene* scene_, XY pos_, float radius_) {
		scene = scene_;
		radius = radius_;
		b2body.InitTypePos(scene_->b2world, pos_, b2_staticBody);
		auto def = b2DefaultShapeDef();
		auto circle = b2Circle{ .center = {}, .radius = radius_ };
		b2CreateCircleShape(b2body, &def, &circle);
	}

	bool SceneItem2::Update() {
		// todo: follow mouse?
		return false;
	}

	void SceneItem2::Draw() {
		auto [p, r] = b2body.GetPosRadians();
		gg.Quad().DrawFrame(gg.fs.circle256, scene->cam.ToGLPos(p), radius * 2.f / 256.f * scene->cam.scale, r);
	}

	/***************************************************************************************/

	void Scene::Init() {
		cam.Init(gg.scale, 0.3f);
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		//b2SetLengthUnitsPerMeter(_phys::ptm_ratio);
		auto def = b2DefaultWorldDef();
		def.gravity = { 1000, 0 };
		def.maximumLinearSpeed = 1000;
		//def.restitutionThreshold = 5.f;
		//def.contactHertz = 240;
		//def.contactDampingRatio = 10;
		//def.contactSpeed = 0.5;
		b2world.InitDef(def, 1);

		frameAndFuncs.Adds({
			{gg.fs._10, _phys::_10::Init},
			{gg.fs._110, _phys::_110::Init},
			{gg.fs._123, _phys::_123::Init},
			{gg.fs._128, _phys::_128::Init},
			{gg.fs._138, _phys::_138::Init},
			{gg.fs._14, _phys::_14::Init},
			{gg.fs._147, _phys::_147::Init},
			{gg.fs._168, _phys::_168::Init},
			{gg.fs._17, _phys::_17::Init},
			{gg.fs._18, _phys::_18::Init},
			{gg.fs._192, _phys::_192::Init},
			{gg.fs._206, _phys::_206::Init},
			{gg.fs._243, _phys::_243::Init},
			{gg.fs._244, _phys::_244::Init},
			{gg.fs._26, _phys::_26::Init},
			{gg.fs._283, _phys::_283::Init},
			{gg.fs._285, _phys::_285::Init},
			{gg.fs._288, _phys::_288::Init},
			{gg.fs._296, _phys::_296::Init},
			{gg.fs._303, _phys::_303::Init},
			{gg.fs._309, _phys::_309::Init},
			{gg.fs._402, _phys::_402::Init},
			{gg.fs._407, _phys::_407::Init},
			{gg.fs._408, _phys::_408::Init},
			{gg.fs._415, _phys::_415::Init},
			{gg.fs._421, _phys::_421::Init},
			{gg.fs._422, _phys::_422::Init},
			{gg.fs._443, _phys::_443::Init},
			{gg.fs._449, _phys::_449::Init},
			{gg.fs._458, _phys::_458::Init},
			{gg.fs._461, _phys::_461::Init},
			{gg.fs._462, _phys::_462::Init},
			{gg.fs._463, _phys::_463::Init},
			{gg.fs._464, _phys::_464::Init},
			{gg.fs._467, _phys::_467::Init},
			{gg.fs._470, _phys::_470::Init},
			{gg.fs._472, _phys::_472::Init},
			{gg.fs._51, _phys::_51::Init},
			{gg.fs._75, _phys::_75::Init},
			{gg.fs._98, _phys::_98::Init},
			// ...
		});

		item2.Init(this, {}, 200);
	}

	void Scene::Gen(int32_t num_) {
		for (int i = 0; i < num_; ++i) {
			XY pos;
			pos.x = -3500;
			pos.y = lastGenY;
			auto h = item1s.Emplace().Emplace()->Init(this, pos, 0.3);
			lastGenY += h + 10.f;
			if (lastGenY >= 1500) {
				lastGenY = -1500.f;
			}
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

		// todo: mouse circle?
		item2.Update();

		for (auto i = item1s.len - 1; i >= 0; --i) {
			if (item1s[i]->Update()) item1s.SwapRemoveAt(i);
		}

		b2world.Step();

#if 1
		if (genTimer < time) {
			genTimer += 0.1;
			Gen(5);
			xx::CoutN("num items = ", item1s.len);
		}
#endif
	}

	void Scene::Draw() {
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, 0, 0.5f, gg.windowSize, 0, 1, {38,194,208,255});
		gg.Quad().DrawFrame(gg.fs.bg_fish, { 0, -200 }, cam.scale * 3.3);

		for (auto& o : item1s) o->Draw();
		item2.Draw();

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}

}
