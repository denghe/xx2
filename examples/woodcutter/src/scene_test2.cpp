#include "pch.h"
#include "scene_test2.h"
#include "scene_mainmenu.h"
#include "particle.h"

namespace Test2 {
	void Scene::Init() {
		cam.Init(gg.scale, 0.2f);
		ui.Emplace()->InitRoot(gg.scale);
		// todo

		auto cfg = xx::MakeShared<xx::ParticleConfig>();
		cfg->frame = gg.fs.p11a;
		cfg->emission = 5000;
		cfg->lifetime = -1.f;
		cfg->particleLife = { 2.f, 3.5f };
		cfg->direction = 0;
		cfg->spread = M_PI * 2;
		cfg->relative = {};
		cfg->speed = { -100.f, 300.f };
		cfg->gravity = { -200.f, 180.f };
		cfg->radialAccel = { -130.f, 200.f };
		cfg->tangentialAccel = { -200.f, 270.f };
		cfg->size = { 0.5f, 5.f };
		cfg->sizeVar = 0.4;
		cfg->spin = {};
		cfg->spinVar = 0;
		cfg->color = { { 1, 0, 0, 1 },{ 0, 1, 1, 0 } };
		cfg->colorVar = 0.5;
		cfg->alphaVar = 1;

		p.Init(cfg, 200000, true);
	}

	void Scene::Update() {
		// handle inputs
		if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
			gg.MakeScene<Scene_MainMenu>()->Init();
			return;
		}

#if 1
		p.FireAt(cam.ToLogicPos(gg.mousePos));
#else
		// entire follow mouse
		p.rootPos = gg.mousePos;
		p.Fire();
#endif
		p.Update(gg.rnd, gg.delta);

		//// fixed update
		//auto d = float(std::min(gg.delta, gg.cMaxDelta) * timeScale);
		//timePool += d;
		//while (timePool >= gg.cDelta) {
		//	time += gg.cDelta;
		//	timePool -= gg.cDelta;
		//	FixedUpdate();
		//}
	}

	void Scene::FixedUpdate() {
//#if 1
//		p.FireAt(cam.ToLogicPos(gg.mousePos));
//#else
//		// entire follow mouse
//		p.rootPos = gg.mousePos;
//		p.Fire();
//#endif
//		p.Update(gg.rnd, gg.cDelta);
	}

	void Scene::Draw() {
		p.Draw(cam);

		gg.uiText->SetText(xx::ToString("num particals = ", p.particles.Count()));
		gg.SetBlendPremultipliedAlpha(false);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale);
		cam.SetBaseScale(gg.scale);
	}

}
