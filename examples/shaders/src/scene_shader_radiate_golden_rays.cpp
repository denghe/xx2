#include "pch.h"
#include "scene_shader_radiate_golden_rays.h"
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
	gg.shaderRadiateGoldenRays().Draw({}, -time, gg.designSize.y * 0.5f * cam.scale
		, /*RGBA8 centerColor_ = */{ 255, 255, 250, 255 }
		, /*RGBA8 rayColor_ = */{ 255, 224, 112, 255 }
		, /*float rayCount_ = */12.f
		, /*float raySharpness_ = */2.f
		, /*float radiusFade_ = */1.1f
		, /*float centerGlowRadius_ = */0.5f
		, /*float rotationSpeed_ = */0.1f
		, /*float flickerSpeed_ = */2.0f
		, /*float flickerStrength_ = */1.0f
	);
#if 0
	gg.shaderRadiateGoldenRays().Draw({}, time, gg.designSize.y * 0.5f * cam.scale
		, /*RGBA8 centerColor_ = */{ 255, 255, 250, 255 }
		, /*RGBA8 rayColor_ = */{ 255, 224, 112, 255 }
		, /*float rayCount_ = */24.f
		, /*float raySharpness_ = */2.f
		, /*float radiusFade_ = */1.1f
		, /*float centerGlowRadius_ = */0.1f
		, /*float rotationSpeed_ = */0.1f
		, /*float flickerSpeed_ = */2.0f
		, /*float flickerStrength_ = */1.0f
	);
#endif
	// ...
	gg.DrawNode(ui);
}

void Scene_Shader_RadiateGoldenRays::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
