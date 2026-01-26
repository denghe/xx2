#include "pch.h"
#include "scene_testb.h"
#include "scene_mainmenu.h"

namespace TestB {

	void Rock::Init(Scene* scene_, int32_t typeId_, int32_t qualityId_, XY pos_) {
		scene = scene_;
		typeId = typeId_;
		qualityId = qualityId_;
		pos = pos_;
		y = pos.y;
		scale = 0.5f;
		colorPlus = 1.f;
		flipX = gg.rnd.Next<bool>();
	}

	void Rock::Draw() {
		XY s;
		s.y = scale;
		if (flipX) s.x = scale;
		else s.x = -scale;
		auto& fs = (std::array<std::array<xx::Frame, 6>, 5>&)gg.all.r0_;
		gg.Quad().DrawFrame(fs[qualityId][typeId]
			, scene->cam.ToGLPos(pos), s * scene->cam.scale, 0, colorPlus);
	}

	/***************************************************************************************/

	void Grass::Init(Scene* scene_, XY pos_, bool randomFrameIndex_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		scale = 1.f;
		auto idx = gg.rnd.Next(gg.spines.idles.len);
		frames = gg.spines.idles[idx].buf;
		framesLen = gg.spines.idles[idx].len;
		colorPlus = 1.f;
		if (randomFrameIndex_) {
			frameIndex = gg.rnd.Next(framesLen);
		}
		flipX = gg.rnd.Next<bool>();
	}

	int Grass::Update() {
		frameIndex += 1.f;// 0.1f;
		if (frameIndex >= framesLen) {
			frameIndex -= framesLen;
		}
		return 0;
	}

	void Grass::Draw() {
		XY s;
		s.y = scale;
		if (flipX) s.x = scale;
		else s.x = -scale;
		gg.Quad().DrawFrame(frames[(int32_t)frameIndex], scene->cam.ToGLPos(pos)
			, s * scene->cam.scale, 0, colorPlus, { 255, 211, 211, 255 });
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

	void Miner::DrawLight() {
		gg.Quad().DrawFrame(gg.all.light64, scene->cam.ToGLPos(pos + XY{0, -32}), scale * 7 * scene->cam.scale
		, 0, 0.6);
	}

	/***************************************************************************************/

	void Scene::GenGrass() {
		XYi cGridSize{ 70, 50 };
		auto cMargin = gg.designSize / cGridSize;
		auto cOffsetRange = cMargin / 5;
		XY basePoss[]{ { cMargin.x * 0.5f, cMargin.y * 0.25f }, { cMargin.x * 0.5f, cMargin.y * 0.75f } };
		for (int y = 0; y < cGridSize.y; ++y) {
			for (int x = 0; x < cGridSize.x; ++x) {
				auto& basePos = basePoss[x & 1];
				XY pos{ basePos.x + cMargin.x * x, basePos.y + cMargin.y * y };
				XY posOffset{
					gg.rnd.Next<float>(-cOffsetRange.x, cOffsetRange.x),
					gg.rnd.Next<float>(-cOffsetRange.y, cOffsetRange.y)
				};
				grasses.Emplace().Init(this, pos + posOffset);
			}
		}
	}

	void Scene::GenRocks() {
		XYi cGridSize{ 28, 10 };
		auto cMargin = gg.designSize / cGridSize;
		cGrassMarginOffsetRange = { cMargin / 5 };
		cGrassMaxCount = cGridSize.x * cGridSize.y;

		XY basePoss[]{ { cMargin.x * 0.5f, cMargin.y * 0.25f }, { cMargin.x * 0.5f, cMargin.y * 0.75f } };
		for (int y = 0; y < cGridSize.y; ++y) {
			for (int x = 0; x < cGridSize.x; ++x) {
				auto& basePos = basePoss[x & 1];
				XY pos{ basePos.x + cMargin.x * x, basePos.y + cMargin.y * y };
				rocksFixedPosPool.Emplace(pos);
				if (gg.rnd.Next<bool>()) continue;
				XY posOffset{
					gg.rnd.Next<float>(-cGrassMarginOffsetRange.x, cGrassMarginOffsetRange.x),
					gg.rnd.Next<float>(-cGrassMarginOffsetRange.y, cGrassMarginOffsetRange.y)
				};
				auto typeId = gg.rnd.Next(gg.all.r0_.size());
				auto qualityId = gg.rnd.Next(2);
				rocks.Emplace().Init(this, typeId, qualityId, pos + posOffset);
			}
		}

		// todo

		for (int32_t i = 0; i < 100; ++i) {
			auto pos = gg.rnd.NextElement(rocksFixedPosPool);
			XY posOffset{
				gg.rnd.Next<float>(-cGrassMarginOffsetRange.x, cGrassMarginOffsetRange.x),
				gg.rnd.Next<float>(-cGrassMarginOffsetRange.y, cGrassMarginOffsetRange.y)
			};
			auto idx = gg.rnd.Next(gg.all.r0_.size());
			//rocks.Emplace().Init(this, idx, pos/* + posOffset*/);
		}
	}

	void Scene::GenMiners() {
		for (int32_t i = 0; i < 15; ++i) {
			XY pos;
			pos.x = gg.rnd.Next(gg.designSize.x);
			pos.y = gg.rnd.Next(64.f, gg.designSize.y);
			auto idx = gg.rnd.Next(gg.spines.attacks.len);
			miners.Emplace().Emplace()->Init(this, idx, pos);
		}
	}

	void Scene::Init() {
		fb.Init();
		ui.Emplace()->InitRoot(gg.scale);
		cam.Init(gg.scale, 1.f, gg.designSize / 2.f);
		sortContainer.Resize<true>((int32_t)gg.designSize.y);
		GenGrass();
		GenRocks();
		GenMiners();
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
		static constexpr xx::FromTo<float> cScaleRange{ 0.2f, 1.f };
		static constexpr float cScaleStep{ 0.035f };
		if (gg.mouse[GLFW_MOUSE_BUTTON_LAST + 1]) {	// mouse wheel up
			cam.SetLogicScale(cam.logicScale + cScaleStep);
		}
		if (gg.mouse[GLFW_MOUSE_BUTTON_LAST + 2]) { // mouse wheel down
			cam.SetLogicScale(cam.logicScale - cScaleStep);
		}

		for (auto& o : grasses) o.Update();
		for (auto& o : miners) o->Update();
	}

	void Scene::Draw() {
		// tex
		auto t = xx::FrameBuffer{}.Init().Draw(gg.windowSize, true, xx::RGBA8{ 0,0,0,0 }, [&]() {
			for (auto& o : grasses) SortContainerAdd(&o);
			for (auto& o : rocks) SortContainerAdd(&o);
			for (auto& o : miners) SortContainerAdd(o.pointer);
			SortContainerDraw();
		});

		// light
		static constexpr float lightTexScale{ 0.25f };	// for improve performance
		cam.SetBaseScale(gg.scale * lightTexScale);
		auto bgColor = xx::RGBA8{ 10,10,10,255 };
		auto t2 = fb.Draw(gg.windowSize * lightTexScale, true, bgColor, [&] {
			gg.GLBlendFunc({ GL_SRC_COLOR, GL_ONE, GL_FUNC_ADD });
			gg.Quad().DrawFrame(gg.all.light64, gg.mousePos * lightTexScale, 10.f * cam.scale, 0, 0.7f);
			for (auto& o : miners) o->DrawLight();
			// ...
		});

		cam.SetBaseScale(gg.scale);

		// combine tex & light
		gg.QuadLight().Draw(t, t2, xx::RGBA8_White, 2);

		gg.DrawNode(ui);
		gg.ShaderEnd();	// ensure tmp texture not release
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
