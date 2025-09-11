#include "pch.h"
#include "game.h"
#include "scene_mainmenu.h"
Game gg;

int32_t main() {
	// load settings from player data store file
	if (int r = gg.SavInit(std::filesystem::current_path())) return r;
	if (int r = gg.Load()) return r;

	// enter game loop
	return gg.Run();
}

void Game::Init() {
	title = "examples_scene";
}

void Game::GLInit() {
	// sync lang settings
	lang.Set((i18n::Languages)settings.language);

	// sync video settings
	if (settings.isFullScreen) {
		if (settings.isBorderless) {
			SetBorderlessMode();
		}
		else {
			SetFullScreenMode(Settings::cResolutions[settings.resolutionsIndex]);
		}
	}
	else {
		SetWindowMode(designSize);
	}

	// sync sound settings
	mute = settings.mute;
	masterVolume = settings.masterVolume;
	audioVolume = settings.audioVolume;
	musicVolume = settings.musicVolume;
	sound.SetGlobalVolume(masterVolume);
}

xx::Task<> Game::Task() {
	// load res
	//res.gear = LoadTexture("res/gear.png");

	// init first scene
	scene.Emplace<Scene_MainMenu>()->Init();

	// game loop
	while (true) {
		scene->Update();
		scene->Draw();
		if (oldScene) oldScene.Reset();
		co_yield 0;
	}
}

void Game::Delay() {
#if 1
	// for power saving, fps limit
	SleepSecs(cDelta - (glfwGetTime() - time));	
#endif
}

void Game::OnResize() {
	if (scene) {
		scene->OnResize();
	}
}

void Game::Stat() {
#if 1
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " uiAutoUpdates.len = ", uiAutoUpdates.len
		, " delayUpdates.len = ", delayUpdates.len
	);
#endif
}
