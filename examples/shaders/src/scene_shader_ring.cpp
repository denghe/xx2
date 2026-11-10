#include "pch.h"
#include "scene_shader_ring.h"
#include "scene_mainmenu.h"

void Scene_Shader_Ring::Init() {
	ui.Emplace()->InitRoot(gg.scale);
	cam.Init(gg.scale, 1.f, 0);

}

void Scene_Shader_Ring::Update() {
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

void Scene_Shader_Ring::FixedUpdate() {
}

void Scene_Shader_Ring::Draw() {
	float tiling{ 1.f };
	gg.shaderRing().Draw({ 0, 0, uint16_t(gg.designSize.x * tiling), uint16_t(gg.designSize.y * tiling) }, {}, 0.5f, 1.f / tiling);
	// ...
	gg.DrawNode(ui);
}

void Scene_Shader_Ring::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
