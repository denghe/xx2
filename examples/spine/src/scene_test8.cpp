#include "pch.h"
#include "scene_test8.h"
#include "scene_mainmenu.h"

void Scene_Test8::Init() {
	cam.Init(gg.scale, 1.f);
	ui.Emplace()->InitRoot(gg.scale);

	// fill texBG
	xx::FromTo<float> xRange{ -gg.designSize.x / 2, gg.designSize.x / 2 };
	xx::FromTo<float> yRange{ -gg.designSize.y / 2, gg.designSize.y / 2 };
	texBG = xx::FrameBuffer{}.Init().Draw(gg.designSize, true, xx::RGBA8{ 54, 128, 41, 255 }, [&] {
		for (size_t i = 0; i < 100000; i++) {
			auto idx = gg.rnd.Next<int32_t>(gg.res.brush_.size());
			auto& tf = gg.res.brush_[idx];
			XY pos{ gg.rnd.Next<float>(xRange.from, xRange.to)
				, gg.rnd.Next<float>(yRange.from, yRange.to) };
			gg.Quad().Draw(tf.tex->id, tf.uvRect, pos, 0.5f, 0.05f);
		}
	});
	texBG->TryGenerateMipmap();
}

void Scene_Test8::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}
}

void Scene_Test8::Draw() {
	gg.Quad().Draw(texBG->id, texBG->Rect(), 0, 0.5f, cam.scale);
	gg.DrawNode(ui);
}

void Scene_Test8::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
