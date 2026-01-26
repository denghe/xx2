#include "pch.h"
#include "scene_test4.h"
#include "scene_mainmenu.h"

namespace Test4 {

	Miner& Miner::Init(Scene* scene_, int32_t minerTypeId_, XY pos_, float radius_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		minerTypeId = minerTypeId_;
		radius = radius_;
		speedScale = 1.f;
		auto& mc = gg.mcs[minerTypeId];
		frameDelay = mc.animFPS / gg.cFps;
		resRadius = mc.resRadius;
		moveSpeed = mc.moveSpeed;
		attackRange = mc.attackRange;
		SetAnim(AnimTypes::Idle);
		return *this;
	}

	void Miner::SetAnim(AnimTypes t) {
		auto& mc = gg.mcs[minerTypeId];
		tfs = mc.fss[(int32_t)t];
		tfsLen = mc.fsLens[(int32_t)t];
		if (t == AnimTypes::Atk) {
			cds = mc.cd;
		}
		else {
			cds = {};
		}
		tfIndex = 0;
	}

	bool Miner::StepAnimOnce() {
		tfIndex += frameDelay * speedScale;
		return tfIndex >= tfsLen;
	}

	void Miner::StepAnimLoop() {
		tfIndex += frameDelay * speedScale;
		while (tfIndex >= tfsLen) {
			tfIndex -= tfsLen;
		}
	}

	char Miner::GetHitData() const {
		return cds[(int32_t)tfIndex];
	}

	void Miner::Draw() {
		auto& c = scene->cam;
		auto i = (int32_t)tfIndex;
		auto& f = tfs[i];
		XY s{ radius / resRadius * c.scale };
		if (flipX) s.x = -s.x;
		gg.Quad().DrawFrame(f, c.ToGLPos(pos), s);
	}

	void Miner::Update() {
		XX_BEGIN(_1);
	LabSearch:
		SetAnim(AnimTypes::Idle);
	LabSearchLoop:
		// simulate spending time thinking
		stepTime = (0.2f + gg.rnd.Next<float>(0.3f)) / speedScale + scene->time;
		while (stepTime > scene->time) {
			StepAnimLoop();
			XX_YIELD(_1);
		}
		// search
		if (!SearchTarget()) goto LabSearchLoop;
	LabMove:
		// move to target pos. step cost 0.5s
		SetAnim(AnimTypes::Move);
	LabMoveLoop:
		stepTime = 0.5f / speedScale + scene->time;
		while (stepTime > scene->time) {
			flipX = targetPos.x < pos.x;
			StepAnimLoop();
			XX_YIELD(_1);
			if (!target) goto LabSearch;
			auto d = targetPos - pos;
			auto mag2 = d.x * d.x + d.y * d.y;
			auto moveStep = moveSpeed * speedScale;
			if (mag2 < moveStep) {
				flipX = target->pos.x < pos.x;
				goto LabAttack;	// reached
			}
			pos += d / std::sqrtf(mag2) * moveStep;	// move
			y = pos.y;
		}
		goto LabMoveLoop;
	LabAttack:
		SetAnim(AnimTypes::Atk);
		if (gg.GetActiveVoiceCount() < 32) {
			gg.PlayAudio(gg.mcs[minerTypeId].ss.Lock());
		}
		hited = 0;
		while (!StepAnimOnce()) {
			XX_YIELD(_1);
			if (!target) goto LabSearch;
			if (auto r = GetHitData(); r > hited) {
				// todo: sound? effect?
				target->Hit();
				// todo: hit logic
				hited = r;
			}
		}
		goto LabAttack;
		XX_END(_1);
	}

	bool Miner::SearchTarget() {
		target.Reset();
		float minMag2 = std::numeric_limits<float>::max();
		for (auto& o : scene->rocks) {	// todo: optimize by Grid
			auto d = o->pos - pos;
			auto dd = d.x * d.x + d.y * d.y;
			if (dd < minMag2) {
				minMag2 = dd;
				target = o;
			}
		}
		if (!target) return false;
		if (target->pos.x > pos.x) {
			targetPos = target->pos + xx::GetRndPosDoughnut(gg.rnd, attackRange, attackRange * 0.7f
				, 155 / 360.f * M_PI * 2, 200 / 360.f * M_PI * 2);
		}
		else {
			targetPos = target->pos + xx::GetRndPosDoughnut(gg.rnd, attackRange, attackRange * 0.7f
				, -40 / 360.f * M_PI * 2, 5 / 360.f * M_PI * 2);
		}
		return true;
	}

	/***************************************************************************************/

	void Rock::Hit() {
		BeginWhite();
		BeginBounce();
	}

	void Rock::BeginWhite() {
		whiteEndTime = scene->time + 0.05f;
	}

	void Rock::BeginBounce() {
		bouncing = true;
		_2 = 0;
		scale = 1.f;
	}

	void Rock::Bounce() {
		static constexpr float cScaleStep{ 0.1f / (gg.cFps * 0.133333f) };
		XX_BEGIN(_2);
		for (scale.x = 1.f; scale.x < 1.1f; scale.x += cScaleStep) {
			scale.y = 2.f - scale.x;
			XX_YIELD(_2);
		}
		for (; scale.x > 0.95f; scale.x -= cScaleStep) {
			scale.y = 2.f - scale.x;
			XX_YIELD(_2);
		}
		for (; scale.x < 1.f; scale.x += cScaleStep) {
			scale.y = 2.f - scale.x;
			XX_YIELD(_2);
		}
		scale = 1.f;
		bouncing = false;
		XX_END(_2);
	}

	Rock& Rock::Init(Scene* scene_, XY pos_, float radius_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = radius_;
		scale = 1.f;
		return *this;
	}

	void Rock::Update() {
		// hit bounce effect
		if (bouncing) {
			Bounce();
		}
	}

	void Rock::Draw() {
		auto& c = scene->cam;
		auto& f = gg.all_rocks_()[0][0];
		float cp;
		if (whiteEndTime > scene->time) cp = 100000.f;
		else cp = 1.f;
		gg.Quad().Draw(f, f, c.ToGLPos(pos), { 0.5f, 0.1f }, radius / 64.f * scale * c.scale, 0, cp);
	}


	/***************************************************************************************/

	void Scene::Init(int32_t minerTypeId_) {
		cam.Init(gg.scale, 2.5f);
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		rocks.Emplace().Emplace<Rock>()->Init(this, { 0, 0 }, 23);
		miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { -50, 0 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { -200, -50 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { -200, 50 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { -200, 100 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { -200, 150 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { 200, 0 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { 200, -50 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { 200, 50 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { 200, 100 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { 200, 150 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { 10, -100 }, 23);
		//miners.Emplace().Emplace<Miner>()->Init(this, minerTypeId_, { -10, 100 }, 23);
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
		if (timer <= time) {
			timer += 2.f;
			std::sort((OrderByYItem**)rocks.buf, (OrderByYItem**)rocks.buf + rocks.len, [](auto& a, auto& b) { return a->y < b->y; });
			std::sort((OrderByYItem**)miners.buf, (OrderByYItem**)miners.buf + miners.len, [](auto& a, auto& b) { return a->y < b->y; });
		}

		for (auto& o : miners) o->Update();
		for (auto& o : rocks) o->Update();
	}

	void Scene::Draw() {
		// sort order by y
		assert(obyis.Empty());
		for (auto& o : miners) obyis.Emplace(o->y, o.pointer);
		for (auto& o : rocks) obyis.Emplace(o->y, o.pointer);
		std::sort(obyis.buf, obyis.buf + obyis.len, [](auto& a, auto& b) { return a.first < b.first; });

		// draw by y
		for (auto& o : obyis) o.second->Draw();
		obyis.Clear();

		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}

}
