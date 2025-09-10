#include "pch.h"
#include "game.h"
#include "scene_mainmenu.h"
Game gg;

// todo: ui add try register update to container? call in event lifecycle?

int32_t main() {
    auto cp = std::filesystem::current_path();
	xx::CoutN( "curr path = ", cp );

    auto value = "asdfasdfasasdfasdfasasdfasdfasasdfasdfasasdfasdfasasdfasdfasasdfasdfasasdfasdfasasdfasdfasasdfasdfas";
    {
        auto secs = xx::NowEpochSeconds();
        for (int i = 0; i < 1000; ++i) {
            xx::LMDB db;
            if (db.Init(cp)) return __LINE__;
            if (db.Save("asdf", value)) return __LINE__;
        }
        xx::CoutN(xx::NowEpochSeconds(secs));
    }

    {
        auto secs = xx::NowEpochSeconds();
        for (int i = 0; i < 1000; ++i) {
			xx::LMDB db;
            if (db.Init(cp)) return __LINE__;
            auto v = db.Load("asdf");
            if (v != value) return __LINE__;
        }
        xx::CoutN(xx::NowEpochSeconds(secs));
    }

	return gg.Run();
}

void Game::Init() {
	title = "examples_scene";
}

xx::Task<> Game::Task() {
	// load res
	res.heart = LoadTexture("res/heart.png");
	res.gear = LoadTexture("res/gear.png");
	res.ui_flags_.Emplace(LoadTexture("res/ui_flags_0.png"));
	res.ui_flags_.Emplace(LoadTexture("res/ui_flags_1.png"));

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

void Game::OnResize() {
	if (scene) {
		scene->OnResize();
	}
}

void Game::Stat() {
#if 1
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " delayFuncs.len = ", delayFuncs.len
	);
#endif
}
