#include "pch.h"
#include "g.h"

namespace G {

	void Scene::Init() {
		mapSize = { 10000, 4000 };
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		// world
		{
			//b2SetLengthUnitsPerMeter(_phys::ptm_ratio);
			auto def = b2DefaultWorldDef();
			def.gravity = { 1000, 0 };
			def.maximumLinearSpeed = 1000;
			//def.restitutionThreshold = 5.f;
			//def.contactHertz = 240;
			//def.contactDampingRatio = 10;
			//def.contactSpeed = 0.5;
			b2world.InitDef(def, 1);
		}
		// edge
		{
			auto bodyDef = b2DefaultBodyDef();
			bodyDef.type = b2_staticBody;
			auto bodyId = b2CreateBody(b2world, &bodyDef);

			// ground chain
			b2Vec2 points[] { { 0,0 }, { 0,mapSize.y }, { mapSize.x,mapSize.y }, { mapSize.x,0 } };
			b2ChainDef chainDef = b2DefaultChainDef();
			chainDef.points = points;
			chainDef.count = _countof(points);
			//chainDef.enableSensorEvents = true;
			chainDef.isLoop = true;
			auto chainId = b2CreateChain(bodyId, &chainDef);
			//xx::CoutN(chainId.index1);

			b2Polygon box = b2MakeOffsetBox(100.f, mapSize.y / 2.f, { mapSize.x - 100.f, mapSize.y / 2.f }, b2Rot_identity);
			b2ShapeDef shapeDef = b2DefaultShapeDef();
			shapeDef.isSensor = true;
			shapeDef.enableSensorEvents = true;
			auto boxId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
			//xx::CoutN(boxId.index1);
		}


		circle.Emplace()->Init(this, {}, 200);
	}


	void Scene::Gen(int32_t num_) {
		for (int i = 0; i < num_; ++i) {
			XY pos;
			pos.x = 500;
			pos.y = lastGenY;
			auto h = fishs.Emplace().Emplace()->Init(this, pos, gg.rnd.Next(0.3f, 0.5f));
			lastGenY += h + 10.f;
			if (lastGenY >= mapSize.y - 500) {
				lastGenY = 500;
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
		circle->Update();

		for (auto i = fishs.len - 1; i >= 0; --i) {
			if (fishs[i]->Update()) fishs.SwapRemoveAt(i);
		}

		b2world.Step();

		auto events = b2World_GetSensorEvents(b2world);
		for (int i = 0; i < events.beginCount; ++i) {
			auto& event = events.beginEvents[i];
			auto bodyId = b2Shape_GetBody(event.visitorShapeId);
			auto o = (Fish*)b2Body_GetUserData(bodyId);
			if (o->isDead) continue;
			o->isDead = true;
			tmp.Add(o);
		}
		for (auto o : tmp) o->Dispose();
		tmp.Clear();

		if (genTimer < time) {
			genTimer += 0.1;
			Gen(10);
			xx::CoutN("num items = ", fishs.len);
		}
	}

	void Scene::Draw() {
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, 0, 0.5f, gg.windowSize, 0, 1, {38,194,208,255});
		gg.Quad().DrawFrame(gg.fs.bg_fish, { 0, -200 }, cam.scale * 3.3);

		// sort order by y. map y to 0 ~ 1080 ( design size.y ). FPS 3x faster than std::sort
		for (auto& o : fishs) SortContainerAdd(o.pointer);
		SortContainerDraw();
		circle->Draw();

		gg.uiText->SetText(xx::ToString("count = ", fishs.len));
		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}


	XX_INLINE void Scene::SortContainerAdd(Fish* o) {
		auto& slot = sortContainer[(int32_t)o->y];
		o->next = slot;
		slot = o;
	}

	XX_INLINE void Scene::SortContainerDraw() {
		for (auto o : sortContainer) {
			while (o) {
				o->Draw();
				o = o->next;
			}
		}
		memset(sortContainer.buf, 0, sortContainer.len * sizeof(void*));
	}
}
