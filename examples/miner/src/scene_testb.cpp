#include "pch.h"
#include "scene_testb.h"
#include "scene_mainmenu.h"

namespace TestB {

	void Rock::Init(Scene* scene_, int32_t typeId_, XY pos_) {
		scene = scene_;
		typeId = typeId_;
		pos = pos_;
		y = pos.y;
		scale = 0.5f;
		colorPlus = 0.4f;
		flipX = gg.rnd.Next<bool>();
	}

	void Rock::Draw() {
		XY s;
		s.y = scale;
		if (flipX) s.x = scale;
		else s.x = -scale;
		gg.Quad().DrawFrame(gg.all.r_[typeId]
			, scene->cam.ToGLPos(pos), s * scene->cam.scale, 0, colorPlus);
	}

	/***************************************************************************************/

	void Grass::Init(Scene* scene_, XY pos_, bool randomFrameIndex_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		scale = 0.5f;
		frames = gg.spines.grassIdle.buf;
		framesLen = gg.spines.grassIdle.len;
		colorPlus = 0.4f;
		if (randomFrameIndex_) {
			frameIndex = gg.rnd.Next(framesLen);
		}
		flipX = gg.rnd.Next<bool>();
	}

	int Grass::Update() {
		++frameIndex;
		if (frameIndex >= framesLen) {
			frameIndex = 0;
		}
		return 0;
	}

	void Grass::Draw() {
		XY s;
		s.y = scale;
		if (flipX) s.x = scale;
		else s.x = -scale;
		gg.Quad().DrawFrame(frames[frameIndex], scene->cam.ToGLPos(pos), s * scene->cam.scale, 0, colorPlus);
	}

	/***************************************************************************************/

	void Miner::Init(Scene* scene_, int32_t index_, XY pos_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		scale = 1.f;
		frames = gg.spines.attacks[index_].buf;
		framesLen = gg.spines.attacks[index_].len;
		colorPlus = 1.f;
		flipX = gg.rnd.Next<bool>();
	}

	int Miner::Update() {
		++frameIndex;
		if (frameIndex >= framesLen) {
			frameIndex = 0;
		}
		return 0;
	}

	void Miner::Draw() {
		XY s;
		s.y = scale;
		if (flipX) s.x = scale;
		else s.x = -scale;
		gg.Quad().DrawFrame(frames[frameIndex], scene->cam.ToGLPos(pos), s * scene->cam.scale, 0, colorPlus);
	}

	/***************************************************************************************/

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale);
		cam.Init(gg.scale, 1.f, gg.designSize / 2.f);
		sortContainer.Resize<true>((int32_t)gg.designSize.y);

		XYi cGridSize{ 100, 50 };
		auto cRockMargin = gg.designSize / cGridSize;
		cGrassMarginOffsetRange = { cRockMargin / 3 };
		cGrassMaxCount = cGridSize.x * cGridSize.y;

		XY basePoss[]{ { cRockMargin.x * 0.5f, cRockMargin.y * 0.25f }, { cRockMargin.x * 0.5f, cRockMargin.y * 0.75f } };
		for (int y = 0; y < cGridSize.y; ++y) {
			for (int x = 0; x < cGridSize.x; ++x) {
				auto& basePos = basePoss[x & 1];
				XY pos{ basePos.x + cRockMargin.x * x, basePos.y + cRockMargin.y * y };
				// todo: gen grass
				grasses.Emplace().Init(this, pos);
			}
		}

		for (int32_t i = 0; i < 200; ++i) {
			XY pos;
			pos.x = gg.rnd.Next(gg.designSize.x);
			pos.y = gg.rnd.Next(64.f, gg.designSize.y);
			auto idx = gg.rnd.Next(gg.all.r_.size());
			rocks.Emplace().Init(this, idx, pos);
		}

		for (int32_t i = 0; i < 100; ++i) {
			XY pos;
			pos.x = gg.rnd.Next(gg.designSize.x);
			pos.y = gg.rnd.Next(64.f, gg.designSize.y);
			auto idx = gg.rnd.Next(gg.spines.N);
			miners.Emplace().Emplace()->Init(this, idx, pos);
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
		for (auto& o : grasses) o.Update();
		for (auto& o : miners) o->Update();
	}

	void Scene::Draw() {
		for (auto& o : grasses) SortContainerAdd(&o);
		for (auto& o : rocks) SortContainerAdd(&o);
		for (auto& o : miners) SortContainerAdd(o.pointer);
		SortContainerDraw();

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale);
		cam.SetBaseScale(gg.scale);
	}


	/*********************************************************************************/
	// for draw order by Y
	XX_INLINE void Scene::SortContainerAdd(SceneItem* o) {
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
	/*********************************************************************************/
}
