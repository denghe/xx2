#include "pch.h"
#include "scene_test8.h"
#include "scene_mainmenu.h"

namespace Test8 {

	bool SceneItem::Update() {
		auto [p, r] = b2body.GetPosRadians();
		pos = p;
		y = p.y;
		radians = r;
		return false;
	}

	void SceneItem::Draw() {
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(pos), scene->cam.scale, radians);
	}

	/***************************************************************************************/

	void Wall::Init(Scene* scene_) {
		frame = gg.pics.bag_[0];
		scene = scene_;
		pos = gg.designSize / 2;
		y = pos.y;

		static constexpr float cRadiusInside{200}, cRadiusOutside{500};
		for (float angle{}; angle < float(M_PI * 2); angle += float(M_PI * 2)/45) {
			XY norm{ std::cosf(angle), std::sinf(angle) };
			b2HullPointsInside.Emplace(norm * cRadiusInside);
		}
		for (float angle{ float(M_PI * 2) }; angle > 0; angle -= float(M_PI * 2)/75) {
			XY norm{ std::cosf(angle), std::sinf(angle) };
			b2HullPointsOutside.Emplace(norm * cRadiusOutside);
		}

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.position = (b2Vec2&)pos;
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		// ground chain
		b2ChainDef chainDef = b2DefaultChainDef();
		chainDef.isLoop = true;

		chainDef.points = (b2Vec2*)b2HullPointsInside.buf;
		chainDef.count = b2HullPointsInside.len;
		/*auto chainId = */b2CreateChain(b2body, &chainDef);

		chainDef.points = (b2Vec2*)b2HullPointsOutside.buf;
		chainDef.count = b2HullPointsOutside.len;
		/*auto chainId = */b2CreateChain(b2body, &chainDef);
	}

	void Wall::Draw() {
		auto basePos = pos;
		auto&& line = gg.Line();
		{
			auto len = b2HullPointsInside.len;
			auto ptr = line.Alloc(len + 1);
			for (size_t i = 0; i < len; ++i) {
				ptr[i] = (xx::Shader_LineData&)(scene->cam.ToGLPos(basePos + b2HullPointsInside[i]));
				(xx::RGBA8&)ptr[i] = xx::RGBA8_Red;
			}
			ptr[len] = ptr[0];
		}
		{
			auto len = b2HullPointsOutside.len;
			auto ptr = line.Alloc(len + 1);
			for (size_t i = 0; i < len; ++i) {
				ptr[i] = (xx::Shader_LineData&)(scene->cam.ToGLPos(basePos + b2HullPointsOutside[i]));
				(xx::RGBA8&)ptr[i] = xx::RGBA8_Red;
			}
			ptr[len] = ptr[0];
		}
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
		def.enableSensorEvents = true;
		auto& buf = std::get<1>(o);
		auto& len = std::get<2>(o);
		auto tarBuf = (XY*)alloca(sizeof(XY) * len);
		auto fsX = frame.Size().x;
		float scale = (fsX - 4) / fsX;	// remove black border pixels
		for (size_t i = 0; i < len; ++i) {
			tarBuf[i] = buf[i] * scale;
		}
		auto hull = b2ComputeHull((b2Vec2*)tarBuf, len);
		auto polygon = b2MakePolygon(&hull, 1);
		b2CreatePolygonShape(b2body, &def, &polygon);
	}

	bool Rock::Update() {
		if (SceneItem::Update()) return true;

		// give force
		auto wallPos = scene->wall->pos;
		auto d = pos - wallPos;
		auto r = std::atan2f(d.y, d.x) - float(M_PI * 0.51);
		XY norm{ std::cosf(r), std::sinf(r) };
		//b2Body_ApplyForceToCenter(b2body, (b2Vec2&)(norm * 500000.f), true);
		//b2Body_ApplyLinearImpulseToCenter(b2body, (b2Vec2&)(norm * 10000.f), true);

		// speed limit
		b2Vec2 vel = b2Body_GetLinearVelocity(b2body);
		vel = b2Normalize(vel);
		vel.x += norm.x;
		vel.y += norm.y;
		static constexpr float cSpeed{ 100.f };
		b2Body_SetLinearVelocity(b2body, (b2Vec2&)(vel * cSpeed * b2GetLengthUnitsPerMeter()));

		// edge check
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
