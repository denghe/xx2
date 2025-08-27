#pragma once
#include "pch.h"
using XY = xx::XY;

struct Monster;
struct Game : xx::Game<Game> {
	struct {
		xx::Ref<xx::Scale9Config> s9bN, s9bH;
	} cfg;

	struct {
		xx::Ref<xx::GLTexture> heart, ui_button_n, ui_button_h;
	} res;

	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	xx::Shared<Monster> heart;

	void Init();
	void GLInit();
	void Update();
	void Stat();
	void OnResize();
};
extern Game gg;
