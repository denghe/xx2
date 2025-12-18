#include "pch.h"
#include "scene_testc.h"
#include "scene_mainmenu.h"

namespace TestC {

	void Miner::Init(Scene* scene_, int32_t index_, XY pos_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		scale = 1.f;
		frames = gg.spines.attacks[index_].buf;
		framesLen = gg.spines.attacks[index_].len;
		colorPlus = 1.f;
		flipX = gg.rnd.Next<bool>();

		AnimInit();
		bounceHeightMax = 20.f;
	}

	int Miner::Update() {
		XX_BEGIN(_1);
	LabLoop:
		// idle a while
		for (timer = scene->time + 0.5f; timer >= scene->time;) {
			XX_YIELD_I(_1);
		}

		// bounce + rotate move to target
		SetMoveTargetPos();
		while (MoveToTarget()) {
			AnimBounceRotate();
			XX_YIELD_I(_1);
		}

		// waiting for the miner to fall to the ground
		while (bouncing) {
			AnimBounceRotate();
			XX_YIELD_I(_1);
		}
		AnimInit();	// reset anim

		goto LabLoop;
		XX_END(_1);
		return 0;
	}

	void Miner::Draw() {
		auto& f = frames[frameIndex];
		XY p{ pos.x, pos.y + yOffset };
		XY anchor{ f.anchor.x, 0.5f };
		XY s;
		s.y = scale;
		if (flipX) s.x = scale;
		else s.x = -scale;
		gg.Quad().Draw(f, f, scene->cam.ToGLPos(p), anchor, s * scene->cam.scale, radians, colorPlus);
	}

	XX_INLINE void Miner::SetYOffset() {
		auto& frame = frames[frameIndex];
		auto frameHeight = (float)frame.uvRect.h;
		auto centerHeight = frameHeight * 0.5f;
		auto pivotHeight = frameHeight * frame.anchor.y;
		yOffset = centerHeight - pivotHeight + bounceHeight;
	}

	XX_INLINE void Miner::SetMoveTargetPos() {
		moveTargetPos.x = gg.rnd.Next(gg.designSize.x);
		moveTargetPos.y = gg.rnd.Next(gg.designSize.y);
		flipX = moveTargetPos.x <= pos.x;
	}

	XX_INLINE int32_t Miner::MoveToTarget() {
		static constexpr float moveSpeed{ 300.f / gg.cFps };
		auto d = moveTargetPos - pos;
		auto mag2 = d.x * d.x + d.y * d.y;
		if (mag2 <= moveSpeed * moveSpeed) {
			pos = moveTargetPos;
			return 0;
		}
		pos += d / std::sqrtf(mag2) * moveSpeed;
		y = pos.y;
		return 1;	// moving
	}

	XX_INLINE void Miner::AnimInit() {
		radians = 0;
		radiansTarget = 0;
		bounceHeight = 0;
		bouncing = {};
		SetYOffset();
	}

	// jump + rotate anim
	XX_INLINE void Miner::AnimBounceRotate() {
		assert(bounceHeightMax > 0);
		static constexpr float bounceStepNums{ 1.f / 6.f * gg.cFps };
		float bounceStepSpeed { bounceHeightMax / bounceStepNums };
		float bounceStepSpeedMax { bounceStepSpeed * 2.f };
		float bounceStepSpeedDecrease { bounceStepSpeedMax / bounceStepNums };
		float bounceRadiansTarget1 { 25.f / 180.f };
		float bounceRadiansTarget2 { -35.f / 180.f };
		
		XX_BEGIN(_2);
		AnimInit();
	LabLoop:
		// jump up
		bouncing = true;
		bounceInc = bounceStepSpeedMax;
		radians = radiansTarget;
		radiansTarget = bounceRadiansTarget1;
		radiansStep = (radiansTarget - radians) / bounceStepNums;
		do {
			bounceHeight -= bounceInc;
			bounceInc -= bounceStepSpeedDecrease;
			SetYOffset();
			radians += radiansStep;
			XX_YIELD(_2);
		} while (bounceInc >= 0);

		// falling
		bounceInc = 0;
		radians = radiansTarget;
		do {
			bounceHeight += bounceInc;
			bounceInc += bounceStepSpeedDecrease;
			SetYOffset();
			XX_YIELD(_2);
		} while (bounceInc < bounceStepSpeedMax);

		// for anim end notice
		bouncing = false;
		XX_YIELD(_2);

		// jump up
		bouncing = true;
		bounceInc = bounceStepSpeedMax;
		radians = radiansTarget;
		radiansTarget = bounceRadiansTarget2;
		radiansStep = (radiansTarget - radians) / bounceStepNums;
		do {
			bounceHeight -= bounceInc;
			bounceInc -= bounceStepSpeedDecrease;
			SetYOffset();
			radians += radiansStep;
			XX_YIELD(_2);
		} while (bounceInc >= 0);

		// falling
		bounceInc = 0;
		radians = radiansTarget;
		do {
			bounceHeight += bounceInc;
			bounceInc += bounceStepSpeedDecrease;
			SetYOffset();
			XX_YIELD(_2);
		} while (bounceInc < bounceStepSpeedMax);

		// for anim end notice
		bouncing = false;
		XX_YIELD(_2);

		// loop
		goto LabLoop;
		XX_END(_2);
	}

	/***************************************************************************************/

	void Scene::GenMiners() {
		for (int32_t i = 0; i < 100; ++i) {
			XY pos;
			pos.x = gg.rnd.Next(gg.designSize.x);
			pos.y = gg.rnd.Next(64.f, gg.designSize.y);
			auto idx = gg.rnd.Next(gg.spines.N);
			miners.Emplace().Emplace()->Init(this, idx, pos);
		}
	}

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale);
		cam.Init(gg.scale, 1.f, gg.designSize / 2.f);
		sortContainer.Resize<true>((int32_t)gg.designSize.y);
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
		for (auto& o : miners) o->Update();
	}

	void Scene::Draw() {
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
