#include "pch.h"
#include "scene_shaderRadiateGoldenRays.h"
#include "scene_mainmenu.h"

void Scene_Shader_RadiateGoldenRays::Init() {
	ui.Emplace()->InitRoot(gg.scale);
	cam.Init(gg.scale, 1.f, 0);

}

void Scene_Shader_RadiateGoldenRays::Update() {
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

void Scene_Shader_RadiateGoldenRays::FixedUpdate() {

}

void Scene_Shader_RadiateGoldenRays::Draw() {
	gg.shaderRadiateGoldenRays().Draw({}, time, gg.designSize.y * 0.5f * cam.scale);
	// ...
	gg.DrawNode(ui);
}

void Scene_Shader_RadiateGoldenRays::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
