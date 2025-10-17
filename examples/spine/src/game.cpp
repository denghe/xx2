﻿#include "pch.h"
#include "game.h"
#include "scene_mainmenu.h"
Game gg;

int32_t main() {
	// enter game loop
	return gg.Run();
}

void Game::Init() {
	DisableIME();
	title = "examples_spine";
}

void Game::GLInit() {
	// init shader
	shaderGrass.Init();

	// begin load res

	// load pngs
	for (size_t i = 1; i <= 15; i++) {
		res.brush_[i-1] = LoadTexture(xx::ToStringFormat("res/brush_{}.png", i));
	}
	// combine some pngs into single texture
	xx::RectPacker rp;
	for (auto& tf : res.brush_) {
		rp.tfs.Add(&tf);
	}
	if (auto r = rp.Pack(2048); r) {
		xx::CoutN("pack failed");
	}
	else {
		res.brush_[0].tex->TryGenerateMipmap();
	}

	
	// load spine res & parse
	auto& se = xx::gSpineEnv;

	se.Load("res/owl-pro", res.owl.skel, res.owl.tex);
	res.owl.tex->TryGenerateMipmap();
	res.owl.idle = res.owl.skel->findAnimation("idle");
	res.owl.blink = res.owl.skel->findAnimation("blink");
	res.owl.up = res.owl.skel->findAnimation("up");
	res.owl.right = res.owl.skel->findAnimation("right");
	res.owl.down = res.owl.skel->findAnimation("down");
	res.owl.left = res.owl.skel->findAnimation("left");

	se.Load("res/spineboy-pro", res.spineBoy.skel, res.spineBoy.tex);
	res.spineBoy.tex->TryGenerateMipmap();
	res.spineBoy.walk = res.spineBoy.skel->findAnimation("walk");
	res.spineBoy.aim = res.spineBoy.skel->findAnimation("aim");
	res.spineBoy.jump = res.spineBoy.skel->findAnimation("jump");
	res.spineBoy.shoot = res.spineBoy.skel->findAnimation("shoot");

	se.Load("res/man1", res.man1.skel, res.man1.tex);
	res.man1.tex->TryGenerateMipmap();
	res.man1.idle = res.man1.skel->findAnimation("idle");

	se.Load("res/eye", res.eye.skel, res.eye.tex);
	res.eye.tex->TryGenerateMipmap();

	se.Load("res/grass1", res.grass1.skel, res.grass1.tex);
	res.grass1.tex->TryGenerateMipmap();
	res.grass1.idle = res.grass1.skel->findAnimation("idle");

	// init first scene
	scene.Emplace<Scene_MainMenu>()->Init();
}

// game loop
void Game::Update() {
	scene->Update();
	scene->Draw();
	if (oldScene) oldScene.Reset();
}

void Game::Delay() {
#if 1
	// for power saving, fps limit
	SleepSecs(cDelta - (glfwGetTime() - time));	
#endif
}

void Game::OnResize(bool modeChanged_) {
	if (scene) {
		scene->OnResize(modeChanged_);
	}
}

void Game::Stat() {
#if 0
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " uiAutoUpdates.len = ", uiAutoUpdates.len
		, " delayUpdates.len = ", delayUpdates.len
	);
#endif
}
