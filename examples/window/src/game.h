#pragma once
#include "pch.h"
using XY = xx::XY;

struct Monster;
struct Game : xx::Game<Game> {

	struct {
		xx::Ref<xx::GLTexture> heart;
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
