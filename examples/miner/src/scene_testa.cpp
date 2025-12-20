#include "pch.h"
#include "scene_testa.h"
#include "scene_mainmenu.h"

namespace TestA {

	void Miner::Init(Scene* scene_, int32_t index_, XY pos_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		frames = gg.spines.attacks[index_].buf;
		framesLen = gg.spines.attacks[index_].len;
	}

	void Miner::Update() {
		++frameIndex;
		if (frameIndex >= framesLen) {
			frameIndex = 0;
		}
	}

	void Miner::Draw() {
		gg.Quad().DrawFrame(frames[frameIndex], scene->cam.ToGLPos(pos), 0.5f * scene->cam.scale);
	}

	/***************************************************************************************/

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale);
		static constexpr float cLineHeight{ 50 };
		static constexpr float cMargin{ 10 };
		auto cfg = xx::MakeShared<xx::Scale9Config>();
		*cfg = *gg.embed.cfg_s9bN;
		cfg->paddings = { 10, 20, 10, 20 };
		auto cfgH = xx::MakeShared<xx::Scale9Config>();
		*cfgH = *gg.embed.cfg_s9bH;
		cfgH->paddings = { 10, 20, 10, 20 };
		auto fontSize = cLineHeight - cfg->paddings.TopBottom();
		auto basePos = gg.p7 + XY{ cMargin * 2, -cMargin * 2 };
		auto anchor = gg.a7;
		basePos = gg.p1 + cMargin;
		anchor = gg.a1;
		ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("miner nums: 1K").onClicked = [this]() {
			Gen(1000);
		};
		basePos.x += ui->children.Back()->size.x + cMargin;
		ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("2K").onClicked = [this]() {
			Gen(2000);
		};
		basePos.x += ui->children.Back()->size.x + cMargin;
		ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("5K").onClicked = [this]() {
			Gen(5000);
		};
		basePos.x += ui->children.Back()->size.x + cMargin;
		ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("10K").onClicked = [this]() {
			Gen(10000);
		};
		basePos.x += ui->children.Back()->size.x + cMargin;
		ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("20K").onClicked = [this]() {
			Gen(20000);
		};
		basePos.x += ui->children.Back()->size.x + cMargin;
		ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("50K").onClicked = [this]() {
			Gen(50000);
		};
		basePos.x += ui->children.Back()->size.x + cMargin;
		ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("100K").onClicked = [this]() {
			Gen(100000);
		};
		basePos.x += ui->children.Back()->size.x + cMargin;
		ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("200k").onClicked = [this]() {
			Gen(200000);
		};

		cam.Init(gg.scale, 1.f, gg.designSize / 2.f);
		sortContainer.Resize<true>( (int32_t)gg.designSize.y );
		Gen(1000);
#if 0
		XY basePos{ -500, -200 };
		for (int32_t i = 0; i < gg.spines.N; ++i) {
			miners.Emplace().Emplace()->Init(this, i, basePos);
			basePos.x += 200;
			if (basePos.x > 500) {
				basePos.x = -500;
				basePos.y += 200;
			}
		}
#endif
	}

	void Scene::Gen(int32_t count_) {
		miners.Clear();
		for (int32_t i = 0; i < count_; ++i) {
			XY pos;
			pos.x = gg.rnd.Next(gg.designSize.x);
			pos.y = gg.rnd.Next(64.f, gg.designSize.y);
			auto idx = gg.rnd.Next(gg.spines.attacks.len);
			miners.Emplace().Emplace()->Init(this, idx, pos);
		}
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
		//auto& tex = *frames[0].tex;
		//gg.Quad().Draw(tex, tex, 0, 0.5f, 0.5f * cam.scale);

		// sort order by y
		for (auto& o : miners) SortContainerAdd(o.pointer);
		SortContainerDraw();

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale);
		cam.SetBaseScale(gg.scale);
	}
}
