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
		colorplus = 1.f;
		auto& cd = gg.treeCollisionDatas[treeTypeId_];
		collisionData = cd.buf;
		collisionDataLen = cd.len;
		Idle();	// will init frames*
	}

	void Tree::Idle(bool fromTurn_) {
		auto& fs = gg.treeIdles[treeTypeId];
		frames = fs.buf;
		framesLen = fs.len;
		frameInc = framesLen / (gg.cFps * 1.f);
		if (fromTurn_) {
			frameIndex = framesLen * 0.5f;
		}
		else {
			frameIndex = gg.rnd.Next<float>(0, framesLen);
		}
		state = 0;
		_1 = 0;
	}

	void Tree::Turn(bool isLeft_) {
		if (state == 0) {
			auto& fs = gg.treeTurns[treeTypeId];
			frames = fs.buf;
			framesLen = fs.len;
			frameInc = framesLen / (gg.cFps * (1.f / 6.f));
			frameIndex = framesLen * 0.5f;
		}
		if (isLeft_) state = 1;
		else state = 2;
		_1 = 0;
	}

	void Tree::White() {
		needWhite = true;
		_2 = 0;
	}

	bool Tree::Hit() {
		// todo: collision detection
		auto mp = scene->cam.ToLogicPos(gg.mousePos);
		for (auto& a : gg.axeCollisionData) {
			for (int32_t i = 0; i < collisionDataLen; ++i) {
				auto& b = collisionData[i];
				auto ap = a.pos + mp;
				auto bp = b.pos + pos;
				auto d = ap - bp;
				auto rr = a.r + b.r;
				if (d.x * d.x + d.y * d.y < rr * rr) goto LabHit;
			}
		}
		return false;
	LabHit:
		// todo: hp check  is dead  return true
		White();
		Turn(gg.rnd.Next<bool>());
		return false;
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
		for (; frameIndex < framesLen * 0.5f; frameIndex += frameInc) {
			XX_YIELD(_1);
		}
		Idle(true);
		return;
		XX_END(_1);
	}

	void Tree::_TurnRight() {
		XX_BEGIN(_1);
		for (; frameIndex < framesLen; frameIndex += frameInc) {
			XX_YIELD(_1);
		}
		frameIndex = framesLen - 0.0001f;
		for (; frameIndex >= framesLen * 0.5f; frameIndex -= frameInc) {
			XX_YIELD(_1);
		}
		Idle(true);
		return;
		XX_END(_1);
	}

	void Tree::_White() {
		// gradually change whilte 1/12 seconds
		static constexpr float cColorplusMax{ 10.f };
		static constexpr float cDuration{ 1.f / 12.f };
		static constexpr float cStep{ (cColorplusMax - 1.f) / (gg.cFps * cDuration) * 2.f };
		XX_BEGIN(_2);
		for (colorplus = 1.f; colorplus < cColorplusMax; colorplus += cStep) {
			XX_YIELD(_2);
		}
		for (colorplus = cColorplusMax; colorplus > 1.f; colorplus -= cStep) {
			XX_YIELD(_2);
		}
		colorplus = 1.f;
		needWhite = false;
		XX_END(_2);
	}

	bool Tree::Update() {
		if (state == 0) _Idle();
		else if (state == 1) _TurnLeft();
		else if (state == 2) _TurnRight();
		if (needWhite) _White();
		return false;
	}

	void Tree::Draw() {
		auto& f = frames[(int32_t)frameIndex];
		//auto& f = gg._pics.s_[1];
		gg.Quad().DrawFrame(f, scene->cam.ToGLPos(pos), scene->cam.scale, 0, colorplus);
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

#if 1
		trees.Emplace().Emplace()->Init(this, 0, mapSize * 0.5f + XY{-400, 50.f});
		trees.Emplace().Emplace()->Init(this, 1, mapSize * 0.5f + XY{-250, 50.f});
		trees.Emplace().Emplace()->Init(this, 2, mapSize * 0.5f + XY{-100, 50.f});
		trees.Emplace().Emplace()->Init(this, 3, mapSize * 0.5f + XY{50, 50.f});
		trees.Emplace().Emplace()->Init(this, 4, mapSize * 0.5f + XY{200, 50.f});
		trees.Emplace().Emplace()->Init(this, 5, mapSize * 0.5f + XY{350, 50.f});
		trees.Emplace().Emplace()->Init(this, 6, mapSize * 0.5f + XY{0, 300.f});
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
			for (auto& o : trees) o->Turn(true);
		}
		if (gg.mouse[GLFW_MOUSE_BUTTON_2](0.2f)) {
			for (auto& o : trees) o->Turn(false);
		}
		if (gg.keyboard[GLFW_KEY_SPACE]) {
			for (auto& o : trees) o->Hit();
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

		{
			auto& f = gg._pics.s_[1];
			for (auto& o : trees) {
				for (int32_t i = 0; i < o->collisionDataLen; ++i) {
					auto& c = o->collisionData[i];
					gg.Quad().DrawFrame(f, cam.ToGLPos(c.pos + o->pos), c.r / f.uvRect.w * 2.f * cam.scale);
				}
			}
		}

		gg.Quad().DrawFrame(gg._pics.a_[0], gg.mousePos, cam.scale);
		{
			auto mp = cam.ToLogicPos(gg.mousePos);
			auto& f = gg._pics.s_[1];
			for (auto& c : gg.axeCollisionData) {
				gg.Quad().DrawFrame(f, cam.ToGLPos(c.pos + mp), c.r / f.uvRect.w * 2.f * cam.scale);
			}
		}

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
