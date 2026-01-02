#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

namespace Test1 {

	void Tree::Init(Scene* scene_, int32_t treeTypeId_, XY pos_) {
		scene = scene_;
		typeId = cTypeId;
		treeTypeId = treeTypeId_;
		pos = pos_;
		y = pos_.y;
		Idle();
	}

	void Tree::Idle() {
		state = 0;
		_1 = 0;
		auto& fs = gg.treeIdles[treeTypeId];
		frames = fs.buf;
		framesLen = fs.len;
		frameIndex = gg.rnd.Next<float>(0, framesLen);
		frameInc = framesLen / (gg.cFps * 1.f);
	}

	void Tree::TurnLeft() {
		state = 1;
		_1 = 0;
		auto& fs = gg.treeTurnLefts[treeTypeId];
		frames = fs.buf;
		framesLen = fs.len;
		frameIndex = framesLen - 0.0001f;
		frameInc = framesLen / (gg.cFps * 0.1f);
	}

	void Tree::TurnRight() {
		state = 2;
		_1 = 0;
		auto& fs = gg.treeTurnRights[treeTypeId];
		frames = fs.buf;
		framesLen = fs.len;
		frameIndex = 0.f;
		frameInc = framesLen / (gg.cFps * 0.1f);
	}

	void Tree::_Idle() {
		XX_BEGIN(_1);
		if (gg.rnd.Next<bool>()) goto LabTurnRight;
		for (; frameIndex >= 0.f; frameIndex -= frameInc) {
			XX_YIELD(_1);
		}
		frameIndex = 0.f;
	LabTurnRight:
		for (; frameIndex < framesLen; frameIndex += frameInc) {
			XX_YIELD(_1);
		}
		frameIndex = framesLen - 0.0001f;
		XX_YIELD_TO_BEGIN(_1);
		XX_END(_1);
	}

	void Tree::_TurnLeft() {
		XX_BEGIN(_1);
		for (; frameIndex >= 0.f; frameIndex -= frameInc) {
			XX_YIELD(_1);
		}
		frameIndex = 0.f;
		for (; frameIndex < framesLen; frameIndex += frameInc) {
			XX_YIELD(_1);
		}
		Idle();
		return;
		XX_END(_1);
	}

	void Tree::_TurnRight() {
		XX_BEGIN(_1);
		for (; frameIndex < framesLen; frameIndex += frameInc) {
			XX_YIELD(_1);
		}
		frameIndex = framesLen - 0.0001f;
		for (; frameIndex >= 0.f; frameIndex -= frameInc) {
			XX_YIELD(_1);
		}
		Idle();
		return;
		XX_END(_1);
	}

	bool Tree::Update() {
		if (state == 0) _Idle();
		else if (state == 1) _TurnLeft();
		else if(state == 2) _TurnRight();
		return false;
	}

	void Tree::Draw() {
		auto& f = frames[(int32_t)frameIndex];
		//auto& f = gg._pics.s_[1];
		gg.Quad().DrawFrame(f, scene->cam.ToGLPos(pos), scene->cam.scale);
	}
	

	/***********************************************************************************/
	/***********************************************************************************/

	void Scene::Init() {
		MakeUI();

		auto logicScale_ = 1.f;
		mapSize = gg.designSize / logicScale_;
		mapCenterPos = mapSize * 0.5f;
		cam.Init(gg.scale, logicScale_, mapCenterPos);
		sortContainer.Resize<true>((int32_t)mapSize.y + 1);

		// gen rock pos using image filter
		auto& img = gg._mask_bg_1;
		//y = pos.y + gg._mask_bg_1.h;
		assert(img.comp == 4);
		auto s = 1.f;// / (gg.designSize / img.Size());

		float density{ 1 };
		XYi cGridSize{ 35 * density, 18 * density };
		auto treeScale = 0.6f / density;
		auto treeMargin = img.Size() / cGridSize;
		auto treeMarginOffsetRange = { treeMargin.x / 6, treeMargin.y / 10 };

		auto cellSize = (int32_t)std::ceilf(std::max(treeMargin.x, treeMargin.y));
		auto numCRs = img.Size().As<int32_t>() / cellSize + 1;

		XY minXY{ std::numeric_limits<float>::max() };
		XY maxXY{ std::numeric_limits<float>::min() };
		XY basePoss[]{ 
			{ treeMargin.x * 0.5f, treeMargin.y * 0.25f }, 
			{ treeMargin.x * 0.5f, treeMargin.y * 0.75f }
		};
		for (int y = 0; y < cGridSize.y; ++y) {
			for (int x = 0; x < cGridSize.x; ++x) {
				auto& basePos = basePoss[x & 1];
				XY pos{ basePos.x + treeMargin.x * x, basePos.y + treeMargin.y * y };
				auto ipos = (pos * s).As<int32_t>();
				// filte by img
				auto cidx = ipos.y * img.w + ipos.x;
				if (img.At(cidx).a) {
					fixedPosPool.Emplace(pos);
					if (pos.x > maxXY.x) maxXY.x = pos.x;
					if (pos.x < minXY.x) minXY.x = pos.x;
					if (pos.y > maxXY.y) maxXY.y = pos.y;
					if (pos.y < minXY.y) minXY.y = pos.y;
				}
			}
		}

#if 0
		for (size_t i = 0; i < 10; i++) {
			//XY pos{
			//	gg.rnd.Next(-300.f, 300.f)
			//	, gg.rnd.Next(-200.f, 200.f)
			//};
			auto pos = gg.rnd.NextElement(fixedPosPool);
			trees.Emplace().Emplace()->Init(this, gg.rnd.Next(7), pos);
		}
#else
		for (auto& p : fixedPosPool) {
			trees.Emplace().Emplace()->Init(this, gg.rnd.Next(7), p);
		}
#endif
	}

	void Scene::MakeUI() {
		ui.Emplace()->InitRoot(gg.scale);
		// todo
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

		if (gg.mouse[GLFW_MOUSE_BUTTON_1](0.2f)) {
			for (auto& o : trees) o->TurnLeft();
		}
		if (gg.mouse[GLFW_MOUSE_BUTTON_2](0.2f)) {
			for (auto& o : trees) o->TurnRight();
		}

		for (auto i = trees.len - 1; i >= 0; --i) {
			if (trees[i]->Update()) {
				trees.SwapRemoveAt(i);
			}
		}
	}

	void Scene::Draw() {
		// bg
		gg.Quad().DrawTinyFrame(gg._pics.bg_[0], 0, 0.5f, cam.scale);

		for (auto& o : trees) SortContainerAdd(o.pointer);
		SortContainerDraw();

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale);
		cam.SetBaseScale(gg.scale);
	}

	/***********************************************************************************/
	/***********************************************************************************/

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
