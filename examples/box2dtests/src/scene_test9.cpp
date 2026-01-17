#include "pch.h"
#include "scene_test9.h"
#include "scene_mainmenu.h"

namespace Test9 {

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

	void Block::Init(Scene* scene_, XY pos_, XY size_) {
		frame = gg.pics.block444;
		scene = scene_;
		pos = pos_;
		y = pos_.y;
		size = size_;

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.position = (b2Vec2&)pos;
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		auto halfSize = size_ * 0.5f;
		auto poly = b2MakeBox(halfSize.x, halfSize.y);
		auto def = b2DefaultShapeDef();
		b2CreatePolygonShape(b2body, &def, &poly);
	}

	void Block::Draw() {
		auto scale = size / frame.Size();
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(pos), scale * scene->cam.scale, radians);
	}

	/***************************************************************************************/

	void Ball::Init(Scene* scene_, XY pos_, float radius_) {
		frame = gg.pics.ball4324;
		scene = scene_;
		pos = pos_;
		y = pos_.y;
		radius = radius_;

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.position = (b2Vec2&)pos;
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		b2Circle circle{
			.center = { 0,0 },
			.radius = radius_
		};
		auto def = b2DefaultShapeDef();
		def.material.friction = 0.f;
		def.material.restitution = 0.5f;
		b2CreateCircleShape(b2body, &def, &circle);
	}

	void Ball::Draw() {
		auto scale = radius * 2 / frame.Size();
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(pos), scale * scene->cam.scale, radians);
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
		bodyDef.linearVelocity = { 0, 300 };
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		auto o = gg.rnd.NextElement(gg.rocksFramePhysMaps);
		frame = *std::get<0>(o);

		auto def = b2DefaultShapeDef();
		def.enableSensorEvents = true;
		def.material.friction = 0.f;
		def.material.restitution = 0.5f;

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

		// edge check
		return (pos.x < 0 || pos.x > 1920
			|| pos.y < 0 || pos.y > 1080);
	}

	/***************************************************************************************/

	void Scene::Init() {
		gg.clearColor = { 130, 130, 130, 255 };
		cam.Init(gg.scale, 1.f, gg.designSize / 2);
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		auto def = b2DefaultWorldDef();
		def.gravity = { 0, 1000 };
		def.maximumLinearSpeed = 1000 * b2GetLengthUnitsPerMeter();
		def.contactSpeed = 1000.0f * b2GetLengthUnitsPerMeter();
		//def.contactHertz = 120.0;
		//def.contactDampingRatio = 0;
		b2world.InitDef(def, 1);

		auto ds = gg.designSize;
		auto ds_2 = gg.designSize / 2;
		static constexpr float edgeWidth{ 100.f }, edgeWidth_2{ edgeWidth / 2 };
		blocks.Emplace().Init(this, XY{ edgeWidth_2, ds_2.y }, XY{ edgeWidth, ds.y });
		blocks.Emplace().Init(this, XY{ ds.x - edgeWidth_2, ds_2.y }, XY{ edgeWidth, ds.y });

		static constexpr XY blockSize{ 12, 150 }, blockSize_2{ blockSize / 2 };
		static constexpr int32_t numBlocks = 12;
		auto freeWidth = ds.x - blockSize.x * numBlocks - edgeWidth * 2;
		auto step = freeWidth / (numBlocks + 1) + blockSize.x;
		for (int i = 1; i <= numBlocks; ++i) {
			auto x = edgeWidth + step * i - blockSize_2.x;
			blocks.Emplace().Init(this, XY{ x, ds.y - blockSize_2.y }, blockSize);
		}

		static constexpr float ballRadius{ 10.f };
		static constexpr int32_t numBallRows{ 7 }, numBallCols{ 16 };
		step = freeWidth / (numBallCols + 1) + ballRadius;
		float baseY = 200.f;
		for (int j = 0; j < numBallRows; ++j) {
			int32_t isSingle = j & 1;
			float baseX = edgeWidth - (isSingle ? step / 2 : 0);
			auto y = baseY + step * j - ballRadius;
			for (int i = 1; i <= numBallCols + isSingle; ++i) {
				auto x = baseX + step * i - ballRadius;
				balls.Emplace().Init(this, XY{ x, y }, ballRadius);
			}
			if (isSingle) {
				auto x = baseX + step * 1 - ballRadius;
				balls.Emplace().Init(this, XY{ x - ballRadius * 1.4142 * 1, y - ballRadius * 1.4142 * 1 }, ballRadius);
				balls.Emplace().Init(this, XY{ x - ballRadius * 1.4142 * 2, y - ballRadius * 1.4142 * 2 }, ballRadius);
				x = baseX + step * (numBallCols + isSingle) - ballRadius;
				balls.Emplace().Init(this, XY{ x + ballRadius * 1.4142 * 1, y - ballRadius * 1.4142 * 1 }, ballRadius);
				balls.Emplace().Init(this, XY{ x + ballRadius * 1.4142 * 2, y - ballRadius * 1.4142 * 2 }, ballRadius);
			}
		}
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

		if (gg.mouse[GLFW_MOUSE_BUTTON_1](0.1f)) {
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
		for (auto& o : blocks) o.Draw();
		for (auto& o : balls) o.Draw();
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
