#include "pch.h"
#include "scene_test4.h"
#include "scene_mainmenu.h"

namespace Test4 {

	// todo

	float SceneItem1::Init(Scene* scene_, XY pos_, float scale_) {
		scene = scene_;
		scale = scale_;
		indexAtContainer = scene->item1s.len - 1;
		assert(scene->item1s[indexAtContainer].pointer == this);

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
		ff.second(b2body, scale_, {}, {});

		return frame.uvRect.h * scale_;
	}

	bool SceneItem1::Update() {
		auto trans = b2body.GetTransform();
		auto& p = (XY&)trans.p;
		y = p.y;
		b2body.SetTransform(p.AddX(10), trans.q);
		return (p.x >= scene->mapSize.x || p.x < 0 || p.y >= scene->mapSize.y || p.y < 0);
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
		mapSize = { 5500, 3000 };
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);
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

		item2.Init(this, {}, 200);
	}

	void Scene::Gen(int32_t num_) {
		for (int i = 0; i < num_; ++i) {
			XY pos;
			pos.x = 1;
			pos.y = lastGenY;
			auto h = item1s.Emplace().Emplace()->Init(this, pos, gg.rnd.Next(0.2f, 0.3f));
			lastGenY += h + 10.f;
			if (lastGenY >= mapSize.y) {
				lastGenY = 0;
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

		// mouse circle
		item2.Update();

		// fishs
		for (auto i = item1s.len - 1; i >= 0; --i) {
			auto& o = item1s[i];
			if (o->Update()) o->Dispose();
		}

		//b2world.Step();

#if 1
		if (genTimer < time) {
			genTimer += 0.01;
			Gen(10);
		}
#endif
#if 1
		if (logTimer < time) {
			logTimer += 1.f;
			xx::CoutN("num items = ", item1s.len);
		}
#endif
	}

	void Scene::Draw() {
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, 0, 0.5f, gg.windowSize, 0, 1, {38,194,208,255});
		gg.Quad().DrawFrame(gg.fs.bg_fish, { 0, -200 }, cam.scale * 3.3);

		// sort order by y. map y to 0 ~ 1080 ( design size.y ). FPS 3x faster than std::sort
		for (auto& o : item1s) SortContainerAdd(o.pointer);
		SortContainerDraw();

		item2.Draw();

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}



	XX_INLINE void Scene::SortContainerAdd(SceneItem1* o) {
		//assert(o->y >= 0.f);
		//assert(o->y < gg.designSize.y);
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
