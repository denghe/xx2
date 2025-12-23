#include "pch.h"
#include "scene_test2.h"
#include "scene_mainmenu.h"

namespace Test2 {

	void SceneItem1::Init(Scene* scene_, XY pos_, float scale_) {
		scene = scene_;
		frame = gg.fs._10;
		scale = scale_;

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = (b2Vec2&)pos_;
		bodyDef.angularDamping = 1.f;
		//bodyDef.gravityScale = 2.f;
		bodyDef.linearVelocity = { 1000.f, 0 };
		b2body.InitDef(scene_->b2world, bodyDef);

#if 1
		xx::List<XY> polys;
		auto AddPoly = [&](auto polydata_, size_t len_, auto& shapeDef)->void {
			polys.Clear();
			for (size_t i = 0; i < len_ / 2; ++i) polys.Add(((XY*)polydata_)[i].FlipY() * scale_);
			auto hull = b2ComputeHull((b2Vec2*)polys.buf, polys.len);
			auto polygon = b2MakePolygon(&hull, 1);
			b2CreatePolygonShape(b2body, &shapeDef, &polygon);
		};

		static constexpr float polygons1[] = { 44.0000, -189.0000, 68.0000, -185.0000, 97.0000, -168.0000, 134.0000, -117.0000, 154.0000, 113.0000, 83.0000, 126.0000, 31.0000, -124.0000, 29.0000, -172.0000 };
		static constexpr float polygons2[] = { -276.0000, 108.0000, -289.0000, 75.0000, -289.0000, 48.0000, -283.0000, 13.0000, -174.0000, -66.0000, -167.0000, 61.0000, -218.0000, 105.0000, -244.0000, 116.0000 };
		static constexpr float polygons3[] = { -153.0000, -90.0000, -152.0000, -112.0000, -130.0000, -145.0000, -113.0000, -159.0000, -77.0000, -156.0000, -51.0000, -134.0000, -36.0000, -112.0000, -121.0000, -68.0000 };
		static constexpr float polygons4[] = { 292.0000, 6.0000, 284.0000, 28.0000, 215.0000, 84.0000, 154.0000, 113.0000, 134.0000, -117.0000, 209.0000, -89.0000, 254.0000, -60.0000, 284.0000, -13.0000 };
		static constexpr float polygons5[] = { -147.0000, 96.0000, -146.0000, 78.0000, -124.0000, 59.0000, 83.0000, 126.0000, 43.0000, 170.0000, -37.0000, 177.0000, -138.0000, 105.0000 };
		static constexpr float polygons6[] = { 10.0000, 189.0000, -37.0000, 177.0000, 43.0000, 170.0000 };
		static constexpr float polygons7[] = { 286.0000, -31.0000, 284.0000, -13.0000, 254.0000, -60.0000 };
		static constexpr float polygons8[] = { -36.0000, -112.0000, 31.0000, -124.0000, 83.0000, 126.0000, -124.0000, 59.0000, -121.0000, -68.0000 };
		static constexpr float polygons9[] = { -267.0000, -111.0000, -247.0000, -110.0000, -223.0000, -101.0000, -174.0000, -66.0000, -283.0000, 13.0000, -292.0000, -59.0000, -281.0000, -104.0000 };
		static constexpr float polygons10[] = { -137.0000, -63.0000, -121.0000, -68.0000, -124.0000, 59.0000, -143.0000, 54.0000 };
		static constexpr float polygons11[] = { -174.0000, -66.0000, -137.0000, -63.0000, -143.0000, 54.0000, -167.0000, 61.0000 };

		auto def = b2DefaultShapeDef();
		def.material.restitution = 1.1f;
		def.material.friction = 1.f;
		//def.material.rollingResistance = 0.02f;
		//def.density = 50.0f;

		AddPoly(polygons1, _countof(polygons1), def);
		AddPoly(polygons2, _countof(polygons2), def);
		AddPoly(polygons3, _countof(polygons3), def);
		AddPoly(polygons4, _countof(polygons4), def);
		AddPoly(polygons5, _countof(polygons5), def);
		AddPoly(polygons6, _countof(polygons6), def);
		AddPoly(polygons7, _countof(polygons7), def);
		AddPoly(polygons8, _countof(polygons8), def);
		AddPoly(polygons9, _countof(polygons9), def);
		AddPoly(polygons10, _countof(polygons10), def);
		AddPoly(polygons11, _countof(polygons11), def);
#else
		auto def = b2DefaultShapeDef();
		auto circle = b2Circle{ .center = {}, .radius = 100.f };
		b2CreateCircleShape(b2body, &def, &circle);
#endif
	}

	bool SceneItem1::Update() {
		auto p = b2body.GetPos();
		return (p.x > 5000 || p.x < -5000 || p.y > 3000 || p.y < -3000);
	}

	void SceneItem1::Draw() {
		auto [p, r] = b2body.GetPosRadians();
#if 1
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(p), scale * scene->cam.scale, r);
#else
		gg.Quad().DrawFrame(gg.fs.circle256, scene->cam.ToGLPos(p), scale * 100.f * 2.f / 256.f * scene->cam.scale, r);
#endif
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

		//b2SetLengthUnitsPerMeter(1);
		auto def = b2DefaultWorldDef();
		def.gravity = { 1000, 0 };
		def.maximumLinearSpeed = 1000;
		//def.restitutionThreshold = 5.f;
		//def.contactHertz = 240;
		//def.contactDampingRatio = 10;
		//def.contactSpeed = 0.5;
		b2world.InitDef(def, 1);
		item2.Init(this, {}, 100);
	}

	void Scene::Gen(int32_t num_) {
		for (int i = 0; i < num_; ++i) {
			XY pos;
			pos.x = -3500;
			pos.y = lastGenY;
			lastGenY += 150.f;
			if (lastGenY >= 1500) {
				lastGenY = -1500.f;
			}
			item1s.Emplace().Emplace()->Init(this, pos, 0.3);
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
