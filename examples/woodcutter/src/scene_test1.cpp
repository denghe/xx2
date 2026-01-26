#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

namespace Test1 {

	// todo: sound

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
		// collision detection
		auto mp = scene->cam.ToLogicPos(gg.mousePos);
		int32_t flipX{ 1 };
		if (scene->mouseDirectionIsRight) flipX = -1;
		for (auto& a : gg.axeCollisionData) {
			auto ap = XY{ a.pos.x * flipX, a.pos.y } + mp;
			for (int32_t i = 0; i < collisionDataLen; ++i) {
				auto& b = collisionData[i];
				auto bp = b.pos * scene->treeScale + pos;
				auto d = ap - bp;
				auto rr = a.r + b.r * scene->treeScale;
				if (d.x * d.x + d.y * d.y < rr * rr) goto LabHit;
			}
		}
		return false;
	LabHit:
		if (lastMouseVersionNumber == scene->mouseVersionNumber) return false;
		// todo: hp check  is dead  return true
		lastMouseVersionNumber = scene->mouseVersionNumber;
		White();
		Turn(pos.x < scene->currentMousePos.x);
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
		//auto& f = gg._pics.s_[1];
		auto& f = frames[(int32_t)frameIndex];
		gg.Quad().DrawFrame(f, scene->cam.ToGLPos(pos), scene->cam.scale * scene->treeScale, 0, colorplus);
	}


	/***********************************************************************************/
	/***********************************************************************************/

	void Scene::FillFixedPosPool(float density_) {
		// fill fixedPosPool with image filter
		auto& img = gg._mask_bg_1;
		assert(img.comp == 4);
		auto siz = img.Size();
		XYi cGridSize{ 35 * density_, 18 * density_ };
		auto treeMargin = siz / cGridSize;
		treeMarginOffsetRange = treeMargin / 5;
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
				auto ipos = pos.As<int32_t>();
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
		// align center
		auto fix = siz * gg._pics.bg_[0].anchor.OneMinusY() + (maxXY - minXY) / 2 - maxXY;
		for (auto& p : fixedPosPool) p += fix;
	}

	void Scene::Init(float bgScale_, float treeScale_, float density_) {
		MakeUI();

		bgScale = bgScale_;
		treeScale = treeScale_;
		auto& bg = gg._pics.bg_[0];
		mapSize = bg.Size() * bgScale;
		mapCenterPos = mapSize * bg.anchor.OneMinusY();
		cam.Init(gg.scale, bgScale, mapCenterPos);
		sortContainer.Resize<true>((int32_t)mapSize.y + 1);
		lastMousePos = currentMousePos = cam.ToLogicPos(gg.mousePos);

#if 0
		//trees.Emplace().Emplace()->Init(this, 0, mapCenterPos + XY{-400, 50.f});
		//trees.Emplace().Emplace()->Init(this, 1, mapCenterPos + XY{-250, 50.f});
		//trees.Emplace().Emplace()->Init(this, 2, mapCenterPos + XY{-100, 50.f});
		//trees.Emplace().Emplace()->Init(this, 3, mapCenterPos + XY{50, 50.f});
		//trees.Emplace().Emplace()->Init(this, 4, mapCenterPos + XY{200, 50.f});
		//trees.Emplace().Emplace()->Init(this, 5, mapCenterPos + XY{350, 50.f});
		//trees.Emplace().Emplace()->Init(this, 6, mapCenterPos + XY{0, 300.f});
		trees.Emplace().Emplace()->Init(this, 6, mapCenterPos + XY{0, 0.f});
#else
		FillFixedPosPool(density_);
		for (auto& p : fixedPosPool) {
			XY pos{ 
				p.x + gg.rnd.Next(-treeMarginOffsetRange.x, treeMarginOffsetRange.x),
				p.y + gg.rnd.Next(-treeMarginOffsetRange.y, treeMarginOffsetRange.y)
			};
			trees.Emplace().Emplace()->Init(this, gg.rnd.Next(7), pos * bgScale);
		}
#endif
	}

	void Scene::MakeUI() {
		ui.Emplace()->InitRoot(gg.scale);
		// todo
	}

	void Scene::Update() {
		currentMousePos = cam.ToLogicPos(gg.mousePos);
		if (currentMousePos.x > lastMousePos.x) {
			if (!mouseDirectionIsRight) {
				++mouseVersionNumber;
				mouseDirectionIsRight = true;
			}
		}
		else if (currentMousePos.x < lastMousePos.x) {
			if (mouseDirectionIsRight) {
				++mouseVersionNumber;
				mouseDirectionIsRight = false;
			}
		}
		lastMousePos = cam.ToLogicPos(gg.mousePos);

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
			++mouseVersionNumber;
		}

		for (auto& o : trees) o->Hit();

		for (auto i = trees.len - 1; i >= 0; --i) {
			if (trees[i]->Update()) {
				trees.SwapRemoveAt(i);
			}
		}
	}

	void Scene::Draw() {
		// bg
		gg.Quad().DrawFrame(gg._pics.bg_[0], cam.ToGLPos(mapCenterPos), cam.scale * bgScale);

		// todo: small grass

		for (auto& o : trees) SortContainerAdd(o.pointer);
		SortContainerDraw();

		if (gg.keyboard[GLFW_KEY_SPACE]) {
			{
				auto& f = gg._pics.s_[1];
				for (auto& o : trees) {
					for (int32_t i = 0; i < o->collisionDataLen; ++i) {
						auto& c = o->collisionData[i];
						gg.Quad().DrawFrame(f, cam.ToGLPos(c.pos * treeScale + o->pos), c.r / f.uvRect.w * 2.f * cam.scale * treeScale, 0, 1, { 255,255,255,127 });
					}
				}
			}
			{
				XY cs{ cam.scale };
				if (mouseDirectionIsRight) cs.x = -cs.x;
				gg.Quad().DrawFrame(gg._pics.a_[0], gg.mousePos, cs);
				auto mp = cam.ToLogicPos(gg.mousePos);
				auto& f = gg._pics.s_[1];
				for (auto& c : gg.axeCollisionData) {
					auto cp{ c.pos };
					if (mouseDirectionIsRight) cp.x = -cp.x;
					gg.Quad().DrawFrame(f, cam.ToGLPos(cp + mp), c.r / f.uvRect.w * 2.f * cs, 0, 1, { 255,255,255,127 });
				}
			}
		}
		else {
			XY cs{ cam.scale };
			if (mouseDirectionIsRight) cs.x = -cs.x;
			gg.Quad().DrawFrame(gg._pics.a_[0], gg.mousePos, cs);
		}

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
