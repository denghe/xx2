#include "pch.h"
#include "game.h"
#include "monster.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = "examples_window";
	windowSize = designSize = { 1024, 768 };
}

void Game::GLInit() {
	// load res
	res.heart = LoadTexture("res/heart.png");

	// init cam
	cam.Init(scale, 1.f, {});

	// init ui
	ui.Emplace()->InitRoot(scale);

	ui->MakeChildren<xx::Label>()->Init(1, p5 + XY{ 0, -69 }, a5, 48)("hi");

	ui->MakeChildren<xx::LabelButton>()->Init(2, p5 + XY{ 0, 50 }, a5, 48)("change color!!!").onClicked = [this] {
		heart->ChangeColor();
	};

	ui->MakeChildren<xx::LabelButton>()->Init(3, p5 + XY{ 0, 0 }, a5, 48)("change anim").onClicked = [this] {
		heart->ChangeAnim();
	};

	// init logic
	heart.Emplace()->Init(res.heart);
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

	// draw
	heart->Draw();
	DrawNode(ui);
}

void Game::Delay() {
#if 1
	// for power saving, fps limit
	SleepSecs(cDelta - (glfwGetTime() - time));
#endif
}

void Game::OnResize() {
	ui->Resize(scale);
	cam.SetBaseScale(scale);
}

void Game::Stat() {
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " delayFuncs.len = ", delayFuncs.len
	);
}
