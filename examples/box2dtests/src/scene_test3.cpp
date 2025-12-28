#include "pch.h"
#include "scene_test3.h"
#include "scene_mainmenu.h"

namespace Test3 {

	void Wall::Init(Scene* scene_, XY pos_, float radius_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		scale = radius_ / 128.f;
		radians = {};
		indexAtContainer = scene->walls.len - 1;
		assert(scene->walls[indexAtContainer].pointer == this);
		scene->gridBuildings.Add(indexAtGrid, this);
	}

	void Wall::Draw() {
		gg.Quad().DrawFrame(gg.fs.circle256, scene->cam.ToGLPos(pos)
			, scale * scene->cam.scale, radians);
	}

	void Wall::Dispose() {
		auto i = indexAtContainer;
		assert(scene->walls[i].pointer == this);
		scene->walls.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->walls.SwapRemoveAt(i);
	}

	Wall::~Wall() {
		if (indexAtGrid > -1) {
			scene->gridBuildings.Remove(indexAtGrid, this);
			indexAtGrid = -1;
		}
	}

	/***************************************************************************************/
	/***************************************************************************************/

	void WoodFactor::Init(Scene* scene_, XY pos_, float scale_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		scale = scale_;
		radians = {};
	}

	void WoodFactor::ShakeA() {
		XX_BEGIN(_1);
		{
			auto r = gg.rnd.Next(M_PI * 2);
			cos = std::cosf(r) * scale * 0.5f;
			sin = std::sinf(r) * scale * 0.5f;
		}
		for (i = 0; i < _countof(cDistances); ++i) {
			offset.x = cos * cDistances[i];
			offset.y = sin * cDistances[i];
			XX_YIELD(_1);
		}
		shaking = false;
		offset = {};
		radians = 0;
		XX_END(_1);
	}

	void WoodFactor::ShakeB() {
		XX_BEGIN(_2);
		for (j = 0; j < _countof(cDistances); ++j) {
			radians = cDistances[j] * 0.029f;
			XX_YIELD(_2);
		}
		shaking = false;
		XX_END(_2);
	}

	bool WoodFactor::Update() {
		if (gg.mouse[GLFW_MOUSE_BUTTON_1]) {
			if (nextGenTime <= scene->time) {
				nextGenTime = scene->time + 0.01f;
				shaking = true;
				offset = {};
				radians = 0;
				_1 = _2 = 0;
				scene->woods.Emplace().Emplace()->Init(scene, pos, scale);
			}
		}

		if (shaking) {
			ShakeA();
			ShakeB();
		}

		return false;
	}

	void WoodFactor::Draw() {
		// todo: shadow ?
		gg.Quad().DrawFrame(gg.fs.wood1, scene->cam.ToGLPos(pos + offset)
			, scale * scene->cam.scale, radians);
	}

	void WoodFactor::Dispose() {
		auto i = indexAtContainer;
		assert(scene->factories[i].pointer == this);
		scene->factories.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->factories.SwapRemoveAt(i);
	}

	WoodFactor::~WoodFactor() {
		if (indexAtGrid > -1) {
			scene->gridBuildings.Remove(indexAtGrid, this);
			indexAtGrid = -1;
		}
	}

	/***************************************************************************************/
	/***************************************************************************************/


	void Wood::Init(Scene* scene_, XY pos_, float scale_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		scale = scale_;
		indexAtContainer = scene->woods.len - 1;
		assert(scene->woods[indexAtContainer].pointer == this);
	}

	void Wood::Anim() {
		static constexpr float cD{ 1.2f };
		static constexpr float cDistances[]{
			cD + .9f,cD + .8f,cD + .7f,cD + .6f,cD + .5f,cD + .4f,cD + .3f,cD + .2f,cD + .1f,cD + .0f,
			cD - .1f,cD - .2f,cD - .3f,cD - .4f,cD - .5f,cD - .6f,cD - .7f,cD - .8f,cD - .9f,cD - 1.f,
			cD - 1.1f, cD - 1.15f, cD - 1.175f, cD - 1.19f, 0
		};
		static constexpr float cIdleYMaxOffset{ 2.f };
		static constexpr float cIdleYStep{ cIdleYMaxOffset / (gg.cFps * 0.58f) };

		XX_BEGIN(_1);
		{
			auto r = gg.rnd.Next(M_PI * 2);
			cos = std::cosf(r) * scale;
			sin = std::sinf(r) * scale;
		}
		for (i = 0; i < _countof(cDistances); ++i) {
			offset.x += cos * cDistances[i];
			offset.y += sin * cDistances[i];
			y = pos.y + offset.y;
			XX_YIELD(_1);
		}
		ready = true;
		pos += offset;
		y = pos.y;
		scene->gridMaterials.Add(indexAtGrid, this);
		offset = {};
		// idle
		while (true) {
			for (; offset.y < cIdleYMaxOffset * scale; offset.y += cIdleYStep * scale) {
				XX_YIELD(_1);
			}
			for (; offset.y > 0.f; offset.y -= cIdleYStep * scale) {
				XX_YIELD(_1);
			}
		}
		XX_END(_1);
	}

	bool Wood::Update() {
		Anim();
		if (ready) {
			// todo: phys simu
			scene->gridMaterials.Update(indexAtGrid, this);
		}
		return false;
	}

	void Wood::Draw() {
		// todo: shadow ?
		gg.Quad().DrawFrame(gg.fs.wood2, scene->cam.ToGLPos(pos + offset), scale * scene->cam.scale);
	}

	void Wood::Dispose() {
		auto i = indexAtContainer;
		assert(scene->woods[i].pointer == this);
		scene->woods.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->woods.SwapRemoveAt(i);
	}

	Wood::~Wood() {
		if (indexAtGrid > -1) {
			scene->gridMaterials.Remove(indexAtGrid, this);
			indexAtGrid = -1;
		}
	}

	/***************************************************************************************/
	/***************************************************************************************/

	void Scene::Init() {
		static constexpr XYi cWallMapSize{ 15, 15 };
		static constexpr float cWallRadius{ 200.f };
		mapSize = cWallRadius * (cWallMapSize + 1);
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);

		gridMaterials.Init(130, mapSize.y / 130 + 1, mapSize.x / 130 + 1);
		gridBuildings.Init(400, mapSize.y / 400 + 1, mapSize.x / 400 + 1);

		factories.Emplace().Emplace()->Init(this, cam.original + XY{ 400, 400 }, 10);
		factories.Emplace().Emplace()->Init(this, cam.original + XY{ -400, 400 }, 10);
		factories.Emplace().Emplace()->Init(this, cam.original + XY{ 400, -400 }, 10);
		factories.Emplace().Emplace()->Init(this, cam.original + XY{ -400, -400 }, 10);

		for (int32_t x = 0; x < cWallMapSize.x; ++x) {
			walls.Emplace().Emplace()->Init(this, {
				cWallRadius + x * cWallRadius
				, cWallRadius + 0 * cWallRadius
			}, cWallRadius);
			walls.Emplace().Emplace()->Init(this, {
				cWallRadius + x * cWallRadius
				, cWallRadius + (cWallMapSize.y - 1) * cWallRadius
			}, cWallRadius);
		}
		for (int32_t y = 1; y < cWallMapSize.y - 1; ++y) {
			walls.Emplace().Emplace()->Init(this, {
				cWallRadius + 0 * cWallRadius
				, cWallRadius + y * cWallRadius
			}, cWallRadius);
			walls.Emplace().Emplace()->Init(this, {
				cWallRadius + (cWallMapSize.x - 1) * cWallRadius
				, cWallRadius + y * cWallRadius
			}, cWallRadius);
		}

		ui.Emplace()->InitRoot(gg.scale * cUIScale);
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
		for (auto& o : factories) o->Update();

		for (auto i = woods.len - 1; i >= 0; --i) {
			auto& o = woods[i];
			if (o->Update()) o->Dispose();
		}
	}

	void Scene::Draw() {
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, 0, 0.5f, gg.windowSize, 0, 1, {0x81,0xbd,0x57,255});

		// sort order by y. map y to 0 ~ 1080 ( design size.y ). FPS 3x faster than std::sort
		for (auto& o : walls) SortContainerAdd(o.pointer);
		for (auto& o : factories) SortContainerAdd(o.pointer);
		for (auto& o : woods) SortContainerAdd(o.pointer);
		SortContainerDraw();

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}



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
}
