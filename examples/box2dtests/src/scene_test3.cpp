#include "pch.h"
#include "scene_test3.h"
#include "scene_mainmenu.h"

namespace Test3 {

	// todo

	float SceneItem1::Init(Scene* scene_, XY pos_, float scale_) {
		scene = scene_;
		scale = scale_;
		indexAtContainer = scene->item1s.len - 1;
		assert(scene->item1s[indexAtContainer].pointer == this);

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = (b2Vec2&)pos_;
		bodyDef.angularDamping = 1.f;
		//bodyDef.gravityScale = 2.f;
		bodyDef.linearVelocity = { 1000.f, 0 };
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		auto& ff = gg.rnd.NextElement(scene_->frameAndFuncs);
		frame = *ff.first;
		ff.second(b2body, scale_, nullptr, "#\0");

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

	void SceneItem1::Dispose() {
		auto i = indexAtContainer;
		assert(scene->item1s[i].pointer == this);
		scene->item1s.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->item1s.SwapRemoveAt(i);
	}

	/***************************************************************************************/


	void SceneItem2::Init(Scene* scene_, XY pos_, float radius_) {
		scene = scene_;
		radius = radius_;
	}

	bool SceneItem2::Update() {
		auto mp = scene->cam.ToLogicPos(gg.mousePos);// .FlipY();
		pos = mp;

		// search mouse pose fish & delete

		b2Circle circle = {
			.center = (b2Vec2&)pos,
			.radius = radius,
		};
		auto proxy = b2MakeProxy(&circle.center, 1, circle.radius);

		b2World_OverlapShape(scene->b2world, &proxy, b2DefaultQueryFilter(), [](b2ShapeId shapeId, void* context)->bool {
			auto scene = (Scene*)context;
			auto bodyId = b2Shape_GetBody(shapeId);
			//xx::CoutN(
			//	"shapeId.generation = ", shapeId.generation
			//	, " shapeId.index1 = ", shapeId.index1
			//	, " bodyId.generation = ", bodyId.generation
			//	, " bodyId.index1 = ", bodyId.index1);
			auto item = (SceneItem1*)b2Body_GetUserData(bodyId);
			if (item->isDead) return true;	// 1 body N shape
			item->isDead = true;
			scene->tmp.Add(item);
			return true;	// true: continue search
		}, scene);

		auto& os = scene->item1s;
		for (auto o : scene->tmp) o->Dispose();
		scene->tmp.Clear();

		return false;
	}

	void SceneItem2::Draw() {
		gg.Quad().DrawFrame(gg.fs.circle256, scene->cam.ToGLPos(pos), radius * 2.f / 256.f * scene->cam.scale);
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
			{&gg.fs._10, _phys::_10::Init},
			{&gg.fs._110, _phys::_110::Init},
			{&gg.fs._123, _phys::_123::Init},
			{&gg.fs._128, _phys::_128::Init},
			{&gg.fs._138, _phys::_138::Init},
			{&gg.fs._14, _phys::_14::Init},
			{&gg.fs._147, _phys::_147::Init},
			{&gg.fs._168, _phys::_168::Init},
			{&gg.fs._17, _phys::_17::Init},
			{&gg.fs._18, _phys::_18::Init},
			{&gg.fs._192, _phys::_192::Init},
			{&gg.fs._206, _phys::_206::Init},
			{&gg.fs._243, _phys::_243::Init},
			{&gg.fs._244, _phys::_244::Init},
			{&gg.fs._26, _phys::_26::Init},
			{&gg.fs._283, _phys::_283::Init},
			{&gg.fs._285, _phys::_285::Init},
			{&gg.fs._288, _phys::_288::Init},
			{&gg.fs._296, _phys::_296::Init},
			{&gg.fs._303, _phys::_303::Init},
			{&gg.fs._309, _phys::_309::Init},
			{&gg.fs._402, _phys::_402::Init},
			{&gg.fs._407, _phys::_407::Init},
			{&gg.fs._408, _phys::_408::Init},
			{&gg.fs._415, _phys::_415::Init},
			{&gg.fs._421, _phys::_421::Init},
			{&gg.fs._422, _phys::_422::Init},
			{&gg.fs._443, _phys::_443::Init},
			{&gg.fs._449, _phys::_449::Init},
			{&gg.fs._458, _phys::_458::Init},
			{&gg.fs._461, _phys::_461::Init},
			{&gg.fs._462, _phys::_462::Init},
			{&gg.fs._463, _phys::_463::Init},
			{&gg.fs._464, _phys::_464::Init},
			{&gg.fs._467, _phys::_467::Init},
			{&gg.fs._470, _phys::_470::Init},
			{&gg.fs._472, _phys::_472::Init},
			{&gg.fs._51, _phys::_51::Init},
			{&gg.fs._75, _phys::_75::Init},
			{&gg.fs._98, _phys::_98::Init},
			// ...
		});

		item2.Init(this, {}, 200);
	}

	void Scene::Gen(int32_t num_) {
		for (int i = 0; i < num_; ++i) {
			XY pos;
			pos.x = -3500;
			pos.y = lastGenY;
			auto h = item1s.Emplace().Emplace()->Init(this, pos, gg.rnd.Next(0.2f, 0.7f));
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
			auto& o = item1s[i];
			if (o->Update()) o->Dispose();
		}

		b2world.Step();

#if 1
		if (genTimer < time) {
			genTimer += 0.2;
			Gen(5);
			//xx::CoutN("num items = ", item1s.len);
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
