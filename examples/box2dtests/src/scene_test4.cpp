#include "pch.h"
#include "scene_test4.h"
#include "scene_mainmenu.h"

namespace Test4 {

	void Wall::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
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

	XY Box::PivotOffset() {
		auto scale = cRadius / gg.fs.wood3.uvRect.w;
		auto size = gg.fs.wood3.Size() * scale;
		auto center = size * 0.5f;
		auto pivot = size * gg.fs.wood3.anchor;
		return center - pivot;
	}

	void Box::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cRadius;
		scale = radius * 2.f / gg.fs.wood3.uvRect.w;
		radians = {};
		indexAtContainer = scene->factories.len - 1;
		assert(scene->factories[indexAtContainer].pointer == this);
		scene->gridBuildings.Add(indexAtGrid, this);
	}

	bool Box::Update() {
		if (nextEffectTime <= scene->time) {
			nextEffectTime = scene->time + 0.1f;

			// auto collect ground materials
			auto cri = scene->gridMaterials.PosToCRIndex(pos);
			static constexpr float cCollectRange{ cRadius * 2 };
			scene->gridMaterials.ForeachByRange(cri.y, cri.x, cRadius * 3, gg.sgrdd, [&](decltype(scene->gridMaterials)::Node& node, float range)
				->void {
					auto d = pos - node.cache.pos;
					auto mag2 = d.x * d.x + d.y * d.y;
					static constexpr float rr = cCollectRange * cCollectRange;
					if (mag2 >= rr || mag2 <= 0.0001f) return;	// not cross?
					flyingWoods.Emplace().Emplace()->Init(this, node.value);
					node.value->Dispose();
				});

			shaking = true;
			offset = {};
			radians = 0;
			_1 = _2 = 0;
		}

		if (shaking) {
			ShakeA();
			ShakeB();
		}

		for (auto i = flyingWoods.len - 1; i >= 0; --i) {
			auto& o = flyingWoods[i];
			if (o->Update()) o->Dispose();
		}

		return false;
	}

	void Box::Draw() {
		gg.Quad().DrawFrame(gg.fs.wood3, scene->cam.ToGLPos(pos + offset)
			, scale * scene->cam.scale, radians);
	}

	/***************************************************************************************/
	/***************************************************************************************/

	XY WoodFactor::PivotOffset() {
		auto scale = cRadius / gg.fs.wood1.uvRect.w;
		auto size = gg.fs.wood1.Size() * scale;
		auto center = size * 0.5f;
		auto pivot = size * gg.fs.wood1.anchor;
		return center - pivot;
	}

	void WoodFactor::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
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
			if (scene->woods.len < 30000) {				// todo
				if (nextEffectTime <= scene->time) {
					nextEffectTime = scene->time + 0.01f;
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
		typeId = cTypeId;
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
		static constexpr float cSpeed{ 5.f };
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

	void FlyingWood::Init(Box* owner_, SceneItem* tar_) {
		typeId = cTypeId;
		owner = owner_;
		scene = tar_->scene;
		pos = tar_->pos;
		y = pos.y;
		radius = tar_->radius;
		radians = tar_->radians;
		scale = tar_->scale;
		indexAtContainer = owner_->flyingWoods.len - 1;
		assert(owner_->flyingWoods[indexAtContainer].pointer == this);

		auto d = owner_->pos - tar_->pos;
		auto mag2 = d.x * d.x + d.y * d.y;
		if (mag2 > 0.0001f) {
			auto mag = std::sqrtf(mag2);
			auto norm = d / mag;
			inc = norm * cSpeed;
			numSteps = int32_t(mag / cSpeed);
		}
	}

	bool FlyingWood::Update() {
		if (--numSteps >= 0) {
			pos += inc;
			y = pos.y;
			--numSteps;
			return false;
		}
		return true;
	}

	void FlyingWood::Draw() {
		gg.Quad().DrawFrame(gg.fs.wood2, scene->cam.ToGLPos(pos), scale * scene->cam.scale);
	}

	void FlyingWood::Dispose() {
		auto i = indexAtContainer;
		assert(owner->flyingWoods[i].pointer == this);
		owner->flyingWoods.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		owner->flyingWoods.SwapRemoveAt(i);
	}


	/***************************************************************************************/
	/***************************************************************************************/

	void GridCache::operator=(SceneItem* p) {
		pos = p->pos;
		radius = p->radius;
	}

	/***************************************************************************************/
	/***************************************************************************************/

	void Scene::GenWallHorizontal(int32_t xFrom_, int32_t xTo_, int32_t y_, bool leftOverflow_, bool rightOverflow_) {
		for (int32_t x = xFrom_; x <= xTo_; ++x) {
			walls.Emplace().Emplace()->Init(this, XY{ x, y_ } * cCellSize + cHalfCellSize);
		}
		for (int32_t x = xFrom_; x < xTo_; ++x) {
			walls.Emplace().Emplace()->Init(this, XY{ x, y_ } * cCellSize + XY{ cCellSize, cHalfCellSize });
		}
		if (leftOverflow_) {
			walls.Emplace().Emplace()->Init(this, XY{ xFrom_, y_ } * cCellSize + XY{ 0, cHalfCellSize });
		}
		if (rightOverflow_) {
			walls.Emplace().Emplace()->Init(this, XY{ xTo_, y_ } * cCellSize + XY{ cCellSize, cHalfCellSize });
		}
	}

	void Scene::GenWallVertical(int32_t x_, int32_t yFrom_, int32_t yTo_, bool topOverflow_, bool bottomOverflow_) {
		for (int32_t y = yFrom_; y <= yTo_; ++y) {
			walls.Emplace().Emplace()->Init(this, XY{ x_, y } * cCellSize + cHalfCellSize);
		}
		for (int32_t y = yFrom_; y < yTo_; ++y) {
			walls.Emplace().Emplace()->Init(this, XY{ x_, y } * cCellSize + XY{ cHalfCellSize, cCellSize });
		}
		if (topOverflow_) {
			walls.Emplace().Emplace()->Init(this, XY{ x_, yFrom_ } * cCellSize + XY{ cHalfCellSize, 0 });
		}
		if (bottomOverflow_) {
			walls.Emplace().Emplace()->Init(this, XY{ x_, yTo_ } * cCellSize + XY{ cHalfCellSize, cCellSize });
		}
	}

	void Scene::GenFactory(int32_t x_, int32_t y_) {
		factories.Emplace().Emplace<WoodFactor>()->Init(this, XY{ x_, y_ } * cCellSize + cHalfCellSize + WoodFactor::PivotOffset());
	}

	void Scene::GenBox(int32_t x_, int32_t y_) {
		factories.Emplace().Emplace<Box>()->Init(this, XY{ x_, y_ } * cCellSize + cHalfCellSize + Box::PivotOffset());
	}

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		// [][][][][][][][][][]
		// []F.              []
		// []F.              []
		// [][][][][][][][]  []
		// []B.              []
		// []B.              []
		// [][][][][][][][][][]

		static constexpr XYi cWallMapSize{ 10, 7 };
		mapSize = cCellSize * cWallMapSize;
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);

		gridBuildings.Init(cCellSize, mapSize.y / (cCellSize) + 1, mapSize.x / (cCellSize) + 1);
		gridMaterials.Init(cItemSize, mapSize.y / cItemSize + 1, mapSize.x / cItemSize + 1);

		GenWallHorizontal(0, cWallMapSize.x - 1, 0);
		GenWallHorizontal(1, cWallMapSize.x - 3, cWallMapSize.y / 2, true);
		GenWallHorizontal(0, cWallMapSize.x - 1, cWallMapSize.y - 1);
		GenWallVertical(0, 1, cWallMapSize.y - 2, true, true);
		GenWallVertical(cWallMapSize.x - 1, 1, cWallMapSize.y - 2, true, true);

		GenFactory(1, 1);
		GenFactory(1, 2);

		GenBox(1, 4);
		GenBox(1, 5);
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
		for (auto& o : factories) {
			SortContainerAdd(o.pointer);
			if (o->typeId == Box::cTypeId) {
				for (auto& p : ((Box*)o.pointer)->flyingWoods) {
					SortContainerAdd(p.pointer);
				}
			}
		}
		for (auto& o : woods) SortContainerAdd(o.pointer);
		SortContainerDraw();

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}



	XX_INLINE void Scene::SortContainerAdd(SceneItem* o_) {
		auto& slot = sortContainer[(int32_t)o_->y];
		o_->next = slot;
		slot = o_;
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
