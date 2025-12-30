#include "pch.h"
#include "g2.h"

namespace G2 {

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
		factories.Emplace().Emplace<WoodFactory>()->Init(this, XY{ x_, y_ } * cCellSize + cHalfCellSize + WoodFactory::PivotOffset());
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
