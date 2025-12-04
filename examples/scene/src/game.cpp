#include "pch.h"
#include "game.h"
#include "scene_mainmenu.h"
#include "equip.h"
Game gg;

int32_t main() {
	// load settings from player data store file
	if (int r = gg.SavInit(std::filesystem::current_path())) return r;
	if (int r = gg.Load()) return r;

	// enter game loop
	return gg.Run();
}

void Game::Init() {
	DisableIME();
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
	// begin load res

	// ogg
	ss.explosion = LoadSoundSource("res/explosion.ogg");

	// png
	res.damage_numbers = LoadTexture("res/damage_numbers.png");	// do not pack
	res.hpbar = LoadTexture("res/hpbar.png");					// do not pack

	fs.blade = LoadTexture("res/blade.png");
	fs.monster = LoadTexture("res/monster.png");
	fs.player = LoadTexture("res/player.png");
	fs.cell_bg = LoadTexture("res/cell_bg.png");
	fs.cell_bg1 = LoadTexture("res/cell_bg1.png");
	fs.cell_bg2 = LoadTexture("res/cell_bg2.png");

	fs.helm_1 = LoadTexture("res/helm_1.png");
	fs.amulet_1 = LoadTexture("res/amulet_1.png");
	fs.ring_1 = LoadTexture("res/ring_1.png");
	fs.ring_2 = LoadTexture("res/ring_2.png");
	fs.ring_3 = LoadTexture("res/ring_3.png");
	fs.ring_4 = LoadTexture("res/ring_4.png");
	fs.armor_1 = LoadTexture("res/armor_1.png");
	fs.armor_2 = LoadTexture("res/armor_2.png");
	fs.armor_3 = LoadTexture("res/armor_3.png");
	fs.weapon1_1 = LoadTexture("res/weapon1_1.png");
	fs.weapon1_2 = LoadTexture("res/weapon1_2.png");
	fs.weapon1_3 = LoadTexture("res/weapon1_3.png");
	fs.weapon2_1 = LoadTexture("res/weapon2_1.png");
	fs.boots_1 = LoadTexture("res/boots_1.png");
	fs.currency_2 = LoadTexture("res/currency_2.png");
	fs.currency_1 = LoadTexture("res/currency_1.png");
	fs.material_1 = LoadTexture("res/material_1.png");
	fs.consumable_1 = LoadTexture("res/consumable_1.png");

	fs.bug_body = LoadTexture("res/bug_body.png");
	fs.bug_head = LoadTexture("res/bug_head.png");

	for (int32_t i = 0; i < fs.explosion_.size(); ++i) {
		fs.explosion_[i] = LoadTexture(xx::ToString("res/explosion_", i, ".png"));
	}

	// combine some pngs into single texture
	xx::RectPacker tp;
	for (int32_t i = 0; i < sizeof(fs) / sizeof(xx::Frame); ++i) {
		tp.tfs.Add((xx::TinyFrame*)&((xx::Frame*)&fs)[i]);
	}
	tp.AutoPack();

	// init equip static configs & makers
	Equip::InitCfgs();

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
#if 0
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
#if 1
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " uiAutoUpdates.len = ", uiAutoUpdates.len
		, " delayUpdates.len = ", delayUpdates.len
	);
#endif
}
