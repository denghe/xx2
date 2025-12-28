#include "pch.h"
#include "scene_test3.h"
#include "scene_mainmenu.h"

namespace Test3 {

	void Wall::Init(Scene* scene_, XY pos_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cRadius;
		scale = radius * 2.f / gg.fs.circle256.uvRect.w;
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

	void WoodFactor::Init(Scene* scene_, XY pos_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cRadius;
		scale = radius * 2.f / gg.fs.wood1.uvRect.w;
		radians = {};
		indexAtContainer = scene->factories.len - 1;
		assert(scene->factories[indexAtContainer].pointer == this);
		scene->gridBuildings.Add(indexAtGrid, this);
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
			if (scene->woods.len < 20000) {				// todo
				if (nextGenTime <= scene->time) {
					nextGenTime = scene->time + 0.01f;
					shaking = true;
					offset = {};
					radians = 0;
					_1 = _2 = 0;
					scene->woods.Emplace().Emplace()->Init(scene, pos);
				}
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


	void Wood::Init(Scene* scene_, XY pos_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cRadius;
		scale = radius * 2.f / gg.fs.wood2.uvRect.w;
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

	void Wood::PreUpdate() {
		if (!ready) return;
		static constexpr float cSpeed{ 1000.f / gg.cFps };
		auto& cache = scene->gridMaterials.NodeAt(indexAtGrid).cache;
		auto cri = scene->gridMaterials.PosToCRIndex(cache.pos);
		auto limit{ 3 };
		scene->gridMaterials.ForeachBy9(cri.y, cri.x, [&](decltype(scene->gridMaterials)::Node& node, float range)
			->bool {
			if (node.value == this) return false;	// is self?
			auto d = cache.pos - node.cache.pos;
			auto mag2 = d.x * d.x + d.y * d.y;
			auto r = cache.radius + node.cache.radius;
			auto rr = r * r;
			if (mag2 >= rr || mag2 <= 0.0001f) return false;	// not cross?
			auto mag = std::sqrtf(mag2);
			auto a = 0.5f * (r - mag);
			auto spd = d / mag * a;
			if (spd.x > cSpeed) spd.x = cSpeed;
			else if (spd.x < -cSpeed) spd.x = -cSpeed;
			if (spd.y > cSpeed) spd.y = cSpeed;
			else if (spd.y < -cSpeed) spd.y = -cSpeed;
			cache.pos += spd;
			//y = pos.y;
			//scene->gridMaterials.Update(indexAtGrid, this);
			node.cache.pos -= spd;
			//o->y = o->pos.y;
			//scene->gridMaterials.Update(indexAtGrid, o);
			return --limit == 0;
		});

		cri = scene->gridBuildings.PosToCRIndex(cache.pos);
		scene->gridBuildings.ForeachBy9(cri.y, cri.x, [&](decltype(scene->gridBuildings)::Node& node, float range)
			->void {
			auto d = cache.pos - node.cache.pos;
			auto mag2 = d.x * d.x + d.y * d.y;
			auto r = node.cache.radius + cache.radius;
			auto rr = r * r;
			if (mag2 < rr) {	// cross?
				if (mag2 > 0) {
					auto mag = std::sqrtf(mag2);
					auto norm = d / mag;
					cache.pos = node.cache.pos + norm * r;	// force move away
				}
			}
		});
	}
	bool Wood::Update() {
		Anim();
		if (!ready) return false;
		auto& cache = scene->gridMaterials.NodeAt(indexAtGrid).cache;
		if (cache.pos != pos) {
			pos = cache.pos;
			scene->gridMaterials.Update(indexAtGrid, this);
			y = pos.y;
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

	void GridCache::operator=(SceneItem* p) {
		pos = p->pos;
		radius = p->radius;
	}

	/***************************************************************************************/
	/***************************************************************************************/

	void Scene::Init() {
		static constexpr XYi cWallMapSize{ 50, 50 };
		mapSize = Wall::cRadius * (cWallMapSize + 1);
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);

		gridMaterials.Init(130, mapSize.y / 130 + 1, mapSize.x / 130 + 1);
		gridBuildings.Init(Wall::cRadius * 2, mapSize.y / (Wall::cRadius * 2) + 1, mapSize.x / (Wall::cRadius * 2) + 1);

		static constexpr float cFactoryDistance{ 800.f };
		static constexpr float cFactoryCRHalfNums{ 3 };
		for (int32_t x = -cFactoryDistance * cFactoryCRHalfNums; x < cFactoryDistance * cFactoryCRHalfNums; x += cFactoryDistance) {
			for (int32_t y = -cFactoryDistance * cFactoryCRHalfNums; y < cFactoryDistance * cFactoryCRHalfNums; y += cFactoryDistance) {
				factories.Emplace().Emplace()->Init(this, cam.original + XY{ x, y } + cFactoryDistance * 0.5f);
			}
		}

		for (int32_t x = 0; x < cWallMapSize.x; ++x) {
			walls.Emplace().Emplace()->Init(this, {
				Wall::cRadius + x * Wall::cRadius
				, Wall::cRadius + 0 * Wall::cRadius
				});
			walls.Emplace().Emplace()->Init(this, {
				Wall::cRadius + x * Wall::cRadius
				, Wall::cRadius + (cWallMapSize.y - 1) * Wall::cRadius
				});
		}
		for (int32_t y = 1; y < cWallMapSize.y - 1; ++y) {
			walls.Emplace().Emplace()->Init(this, {
				Wall::cRadius + 0 * Wall::cRadius
				, Wall::cRadius + y * Wall::cRadius
				});
			walls.Emplace().Emplace()->Init(this, {
				Wall::cRadius + (cWallMapSize.x - 1) * Wall::cRadius
				, Wall::cRadius + y * Wall::cRadius
				});
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

		for (auto& o : woods) o->PreUpdate();
		for (auto i = woods.len - 1; i >= 0; --i) {
			auto& o = woods[i];
			if (o->Update()) o->Dispose();
		}
	}

	void Scene::Draw() {
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, 0, 0.5f, gg.windowSize, 0, 1, { 0x81,0xbd,0x57,255 });

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
