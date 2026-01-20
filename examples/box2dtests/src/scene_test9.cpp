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
		typeId = cTypeId;
		scene = scene_;
		frame = gg.pics.block444;
		pos = pos_;
		y = pos_.y;
		size = size_;

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.position = (b2Vec2&)pos;
		bodyDef.userData = (SceneItem*)this;
		b2body.InitDef(scene_->b2world, bodyDef);

		auto halfSize = size_ * 0.5f;
		auto poly = b2MakeBox(halfSize.x, halfSize.y);
		auto def = b2DefaultShapeDef();
		b2CreatePolygonShape(b2body, &def, &poly);
	}

	bool Block::Update() {
		return false;
	}

	void Block::Draw() {
		auto scale = size / frame.Size();
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(pos), scale * scene->cam.scale, radians);
	}

	/***************************************************************************************/

	void Ball::Init(Scene* scene_, XY pos_, float radius_) {
		typeId = cTypeId;
		scene = scene_;
		frame = gg.pics.ball4324;
		pos = pos_;
		y = pos_.y;
		radius = radius_;
		scale = 1.f;
		colorplus = 0.5f;

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

		color = xx::GetRandomColor(gg.rnd, { 255,0,0,255 });
	}

	void Ball::Hit(Rock* rock_) {
		gg.PlayAudio(gg.embed.ss_ui_focus);
		// todo: calculate ball's value
		scene->effectTextManager.Add(pos, rock_->pos - pos, color, 3, gg.rnd.Next(100), true);
		// check same rock multi hit: temporarily disable ball's shape hit event
		if (lastRock == rock_) {
			++sameRockHitCount;
			if (sameRockHitCount > 5) {
				disabing = true;
				b2Body_Disable(b2body);
			}
		}
		else {
			sameRockHitCount = 1;
		}
		lastRock = rock_;
		bouncing = true;
		_1 = 0;
		_1inc = (pos - rock_->pos).Normalize() * 1.f;
	}

	void Ball::Bounce() {
		XX_BEGIN(_1);
		offset = {};
		scale = 1.f;
		colorplus = 1.f;
		for (_1i = 0; _1i < 9; ++_1i) {
			offset += _1inc;
			scale += 0.05f;
			XX_YIELD(_1);
		}
		colorplus = 0.5f;
		for (_1i = 0; _1i < 9 + 4; ++_1i) {
			offset -= _1inc;
			scale -= 0.03f;
			XX_YIELD(_1);
		}
		scale = 1.f;
		for (_1i = 0; _1i < 4 + 2; ++_1i) {
			offset += _1inc;
			XX_YIELD(_1);
		}
		for (_1i = 0; _1i < 2; ++_1i) {
			offset -= _1inc;
			XX_YIELD(_1);
		}
		offset = 0;
		bouncing = false;
		XX_END(_1);
	}

	bool Ball::Update() {
		if (bouncing) Bounce();
		if (disabing) {
			sameRockHitCount -= 0.1f;
			if (sameRockHitCount <= 0) {
				sameRockHitCount = 0;
				disabing = {};
				b2Body_Enable(b2body);
			}
		}
		return false;
	}

	void Ball::Draw() {
		auto s = scale * radius * 2 / frame.Size().x;
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(pos + offset), s * scene->cam.scale, radians, colorplus, color);
	}

	/***************************************************************************************/

	void Rock::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
		scene = scene_;
		pos = pos_;
		y = pos_.y;
		radians = gg.rnd.Next<float>(0, float(M_PI * 2));

		auto bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = (b2Vec2&)pos_;
		bodyDef.rotation = b2MakeRot(radians);
		bodyDef.angularDamping = 1.f;
		bodyDef.linearVelocity = { 0, 300 };
		bodyDef.userData = this;
		b2body.InitDef(scene_->b2world, bodyDef);

		auto o = gg.rnd.NextElement(gg.rocksFramePhysMaps);
		frame = *std::get<0>(o);

		auto def = b2DefaultShapeDef();
		def.enableHitEvents = true;
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

		// todo: check rock in BonusArea
		if (pos.y >= mapSize.y) {
			for (auto& ba : scene->bonusAreas) {
				if (ba.InArea(pos.x)) {
					// hit bonus area
					scene->effectTextManager.Add(ba.pos, { 0,-100 }, xx::RGBA8_White, 3, ba.ratio, true);
					//gg.PlayAudio(gg.embed.ss_ui_confirm);
					break;
				}
			}
		}

		// edge check
		return (pos.x < 0 || pos.x >= mapSize.x
			|| pos.y < 0 || pos.y >= mapSize.y);
	}

	/***************************************************************************************/

	void BonusArea::Init(Scene* scene_, XY pos_, float width_, int32_t ratio_) {
		scene = scene_;
		pos = pos_;
		halfWidth = width_ * 0.5f;
		ratio = ratio_;
		data.Fill(0, 1, xx::RGBA8_White, ratio_);
	}

	void BonusArea::Draw() {
		data.scale = scene->cam.scale * 2;
		data.pos = scene->cam.ToGLPos(pos)
			- XY{ data.cCharSize.x * data.Len(), data.cCharSize.y } * 0.5f * data.scale;
		gg.Numbers().Draw(data);
	}

	void BonusArea::SetRatio() {
		// todo
	}

	bool BonusArea::InArea(float x_) const {
		return x_ >= pos.x - halfWidth && x_ <= pos.x + halfWidth;
	}


	/***************************************************************************************/

	void Scene::InitUI() {
		static constexpr float cLineHeight{ 100 };
		static constexpr float cItemHeight{ 80 };
		static constexpr float cMargin{ 20 };
		auto fontSize = cItemHeight - gg.embed.cfg_s9bN->paddings.TopBottom();

		ui.Emplace()->InitRoot(gg.scale * cUIScale);
		auto C = ui->Make<xx::Node>();
		xx::Layouter L;
		auto ds = gg.designSize / cUIScale;
		auto hds = ds * 0.5f;
		L.InitBegin(C, 2, { -hds.x + 30, hds.y }, { 0, 1 }, ds.x - 60)
			.HAlign(xx::HAligns::Left)
			.LeftMargin(cMargin)
			.DefaultLineHeight(cLineHeight);

		L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("game speed: 1x")).onClicked = [this] {
			timeScale = 1.f;
			};

		L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("2x")).onClicked = [this] {
			timeScale = 2.f;
			};

		L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("5x")).onClicked = [this] {
			timeScale = 5.f;
			};

		L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("10x")).onClicked = [this] {
			timeScale = 10.f;
			};

		L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("20x")).onClicked = [this] {
			timeScale = 20.f;
			};

		L.InitEnd();
	}

	void Scene::Init() {
		InitUI();
		gg.clearColor = { 60, 60, 60, 255 };
		cam.Init(gg.scale, 0.5f, mapSize_2);
		effectTextManager.Init(&cam, 10000);

		auto def = b2DefaultWorldDef();
		def.gravity = { 0, 1000 };
		def.maximumLinearSpeed = 1000 * b2GetLengthUnitsPerMeter();
		def.contactSpeed = 1000.0f * b2GetLengthUnitsPerMeter();
		//def.contactHertz = 120.0;
		//def.contactDampingRatio = 0;
		b2world.InitDef(def, 1);

		blocks.Emplace().Emplace()->Init(this, XY{ edgeWidth_2, mapSize_2.y }, XY{ edgeWidth, mapSize.y });
		blocks.Emplace().Emplace()->Init(this, XY{ mapSize.x - edgeWidth_2, mapSize_2.y }, XY{ edgeWidth, mapSize.y });

		auto freeWidth = mapSize.x - blockSize.x * numBlocks - edgeWidth * 2;
		auto step = freeWidth / (numBlocks + 1) + blockSize.x;
		auto y = mapSize.y - blockSize_2.y;
		bonusAreas.Emplace().Init(this, XY{ edgeWidth + step / 2, y }, step, 123);
		for (int i = 1; i <= numBlocks; ++i) {
			auto x = edgeWidth + step * i - blockSize_2.x;
			blocks.Emplace().Emplace()->Init(this, XY{ x, y }, blockSize);
			bonusAreas.Emplace().Init(this, { x + step / 2 + blockSize_2.x, y }, step, 123);
		}

		step = freeWidth / (numBallCols + 1) + ballRadius;
		for (int j = 0; j < numBallRows; ++j) {
			int32_t isSingle = j & 1;
			float baseX = edgeWidth - (isSingle ? step / 2 : 0);
			y = baseY + step * j - ballRadius;
			for (int i = 1; i <= numBallCols + isSingle; ++i) {
				auto x = baseX + step * i - ballRadius;
				balls.Emplace().Emplace()->Init(this, XY{ x, y }, ballRadius);
			}
			if (isSingle) {
				auto x = baseX + step * 1 - ballRadius;
				balls.Emplace().Emplace()->Init(this, XY{ x - ballRadius * 1.4142 * 1, y - ballRadius * 1.4142 * 1 }, ballRadius);
				balls.Emplace().Emplace()->Init(this, XY{ x - ballRadius * 1.4142 * 2, y - ballRadius * 1.4142 * 2 }, ballRadius);
				x = baseX + step * (numBallCols + isSingle) - ballRadius;
				balls.Emplace().Emplace()->Init(this, XY{ x + ballRadius * 1.4142 * 1, y - ballRadius * 1.4142 * 1 }, ballRadius);
				balls.Emplace().Emplace()->Init(this, XY{ x + ballRadius * 1.4142 * 2, y - ballRadius * 1.4142 * 2 }, ballRadius);
			}
		}

		// todo: fill ratioNumbers

	}

	void Scene::Gen(int32_t num_) {
		for (int i = 0; i < num_; ++i) {
			XY pos;
			pos.x = gg.rnd.Next(spaceXRange.from, spaceXRange.to);
			pos.y = 1.f;
			rocks.Emplace().Emplace()->Init(this, pos);
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
		effectTextManager.Update(time);
		for (auto& o : balls) o->Update();
		for (auto i = rocks.len - 1; i >= 0; --i) {
			if (rocks[i]->Update()) rocks.SwapRemoveAt(i);
		}

		b2world.Step();

		auto events = b2World_GetContactEvents(b2world);
		for (int i = 0; i < events.hitCount; ++i) {
			auto& event = events.hitEvents[i];
			auto ud1 = (SceneItem*)b2Body_GetUserData(b2Shape_GetBody(event.shapeIdA));
			auto ud2 = (SceneItem*)b2Body_GetUserData(b2Shape_GetBody(event.shapeIdB));
			Ball* ball{};
			Rock* rock{};
			if (ud1->typeId == Ball::cTypeId) {
				ball = (Ball*)ud1;
				rock = (Rock*)ud2;
			}
			else if (ud2->typeId == Ball::cTypeId) {
				ball = (Ball*)ud2;
				rock = (Rock*)ud1;
			}
			if (ball) {
				assert(rock->typeId == Rock::cTypeId);
				ball->Hit(rock);
			}
		}

		genTimer += gg.cDelta * 1.f * 100;
		while (genTimer >= 1.f) {
			genTimer -= 1.f;
			Gen(1);
		}
	}

	void Scene::Draw() {
		for (auto& o : bonusAreas) o.Draw();
		for (auto& o : blocks) o->Draw();
		for (auto& o : balls) o->Draw();
		for (auto& o : rocks) o->Draw();
		effectTextManager.Draw();

		gg.uiText->SetText(xx::ToString("num rocks = ", rocks.len, " num numbers = ", effectTextManager.ens.Count()));
		gg.SetBlendPremultipliedAlpha(false);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}

}
