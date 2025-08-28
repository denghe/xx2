#pragma once
#include "pch.h"
using XY = xx::XY;

struct Game : xx::Game<Game> {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };
	static constexpr float cMaxDelta{ 0.1f };

	xx::Shared<xx::SceneBase> scene, oldScene;	// oldScene: delay remove( after draw )
	template<typename T>
	xx::Shared<T>& MakeScene() {
		oldScene = std::move(scene);
		scene = xx::MakeShared<T>();
		return (xx::Shared<T>&)scene;
	}

	struct {
		xx::Ref<xx::Scale9Config> s9bN, s9bH, s9bg;
	} cfg;

	struct {
		xx::Ref<xx::GLTexture>
			heart,
			ui_button,
			ui_button_h,
			ui_button_n,
			ui_imgbtn_h,
			ui_imgbtn_n,
			ui_dropdownlist_icon,
			ui_dropdownlist_head,
			ui_panel,
			ui_slider_bar,
			ui_slider_block
			;
	} res;

	xx::Shared<xx::Node> ui;

	void Init();
	xx::Task<> Task();
	void Delay();
	void Stat();
	void OnResize();
};
extern Game gg;
