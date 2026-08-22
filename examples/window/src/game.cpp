#include "pch.h"
#include "game.h"
#include "monster.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	DisableIME();
	title = "examples_window";
	windowSize = designSize = { 1280, 800 };
}

void Game::GLInit() {
	// load res
	pics.heart = LoadTexture("res/heart.png");

	// init cam
	cam.Init(scale, 1.f, {});

	// init ui
	ui.Emplace()->InitRoot(scale);

	ui->Make<xx::Label>()->Init(1, p5 + XY{ 0, -69 }, a5, 48)("hi");

	ui->Make<xx::LabelButton>()->Init(2, p5 + XY{ 0, 50 }, a5, 48)("change color!!!").onClicked = [this] {
		heart->ChangeColor();
	};

	ui->Make<xx::LabelButton>()->Init(3, p5 + XY{ 0, 0 }, a5, 48)("change anim").onClicked = [this] {
		heart->ChangeAnim();
	};

	// init logic
	heart.Emplace()->Init(pics.heart);
}

void Game::Update() {
#if 0
	for (int i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; ++i) {
		xx::Cout(joy.btns[i].pressed, " ");
	}
	for (int i = 0; i <= GLFW_GAMEPAD_AXIS_LAST; ++i) {
		xx::Cout(joy.axes[i], " ");
	}
	xx::CoutN();
#endif

	// handle inputs
	if (keyboard[GLFW_KEY_ESCAPE]) {
		running = false;
		return;
	}
	if (mouse[GLFW_MOUSE_BUTTON_WHEEL_UP](0.01f)
		|| keyboard[GLFW_KEY_Z](0.01f)
		|| joy.btns[GLFW_GAMEPAD_BUTTON_A](0.01f)
		) {
		cam.SetLogicScale(cam.logicScale + 0.001f);
	}
	if (mouse[GLFW_MOUSE_BUTTON_WHEEL_DOWN](0.01f)
		|| keyboard[GLFW_KEY_X](0.01f)
		|| joy.btns[GLFW_GAMEPAD_BUTTON_B](0.01f)
		) {
		cam.SetLogicScale(cam.logicScale - 0.001f);
	}

	// logic update
	heart->Update();
	bgOffset += XY{ 0.5f, 0.5f } * delta;

	// draw bg
	QuadEx().DrawFrame(pics.heart
		, { 0.f, 0.f }
		, designSize.x / pics.heart.uvRect.w * cam.scale
		, 100
		, bgOffset
		, 0.f
		, 0.5f
	);

	heart->Draw();
	DrawNode(ui);
}

void Game::Delay() {
#if 0
	// for power saving, fps limit
	SleepSecs(cDelta - (glfwGetTime() - time));
#endif
}

void Game::OnResize(bool modeChanged_) {
	ui->Resize(scale);
	cam.SetBaseScale(scale);
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " delayUpdates.len = ", delayUpdates.len
	);
}
