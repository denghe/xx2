#include "pch.h"
#include "scene_testa.h"
#include "scene_mainmenu.h"

namespace TestA {

	void Miner::Init(Scene* scene_, int32_t index_, XY pos_) {
		scene = scene_;
		pos = pos_;
		frames = gg.spines.framess[index_].buf;
		framesLen = gg.spines.framess[index_].len;
		eventDatas = gg.spines.eventDatass[index_].buf;
	}

	void Miner::Update() {
		++frameIndex;
		if (frameIndex >= framesLen) {
			frameIndex = 0;
		}
	}

	void Miner::Draw() {
		gg.Quad().DrawFrame(frames[frameIndex], scene->cam.ToGLPos(pos), scene->cam.scale);
	}

	/***************************************************************************************/

	void Scene::Init() {
		cam.Init(gg.scale, 1.f);
		ui.Emplace()->InitRoot(gg.scale);

		XY basePos{ -500, -200 };
		for (int32_t i = 0; i < gg.spines.N; ++i) {
			miners.Emplace().Emplace()->Init(this, i, basePos);
			basePos.x += 200;
			if (basePos.x > 500) {
				basePos.x = -500;
				basePos.y += 200;
			}
		}
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

		for (auto& o : miners) o->Draw();

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale);
		cam.SetBaseScale(gg.scale);
	}
}
