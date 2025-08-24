#include "pch.h"
#include "game.h"
Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = U"examples_window";
	windowSize = designSize = { 1024, 768 };
}

void Game::GLInit() {
	texs.heart = LoadTexture("res/heart.png");
	texs.ui_button_n = LoadTexture("res/ui_button_n.png");
	texs.ui_button_h = LoadTexture("res/ui_button_h.png");

	cfgs.s9bN.Emplace(xx::Scale9Config{
		.frame = texs.ui_button_n,
		.texScale = { 1, 1 },
		.center = { 2, 2, 2, 2 },
		.color = xx::RGBA8_White,
		.borderScale = { 4.f },
		.txtColor = xx::RGBA8_White,
		.txtPadding = { 20, 5 },
		.txtPaddingRightBottom = { 20, 10 },
		.txtScale = { 2 },
		.iconPadding = { 5 }
	});

	cfgs.s9bH.Emplace(*cfgs.s9bN);
	cfgs.s9bH->frame = texs.ui_button_h;

	cam.Init(scale, 1.f, {});

	ui.Emplace()->InitRoot(scale);

	ui->MakeChildren<xx::Label>()->Init(1, p5 + XY{ 0, -69 }, a5, 2)
		.SetText("hi");

	ui->MakeChildren<xx::LabelButton>()->Init(2, p5 + XY{ 0, 50 }, a5
		, cfgs.s9bN, cfgs.s9bH
		, "change color!!!").onClicked = [this] {
			heart->ChangeColor();
	};

	ui->MakeChildren<xx::LabelButton>()->Init(3, p5 + XY{ 0, 0 }, a5
		, cfgs.s9bN, cfgs.s9bH
		, "change anim").onClicked = [this] {
			heart->ChangeAnim();
	};

	heart.Emplace()->Init();
}

void Game::Update() {
	// handle inputs
	if (keyboardBtns[sf::Keyboard::Escape]) {
		Close();
		return;
	}
	if (mouseBtns[5](0.01f)		// mouse wheel up
		|| keyboardBtns[sf::Keyboard::Z](0.01f)) {
		cam.SetLogicScale(cam.logicScale + 0.001f);
	}
	if (mouseBtns[6](0.01f)		// mouse wheel down
		|| keyboardBtns[sf::Keyboard::X](0.01f)) {
		cam.SetLogicScale(cam.logicScale - 0.001f);
	}

	// logic
	heart->Update();

	// draw
	heart->Draw();
	DrawNode(ui);
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

/***************************************************************************/

void Heart::ChangeColor() {
	colorIndex++;
	if (colorIndex >= cColors.size()) {
		colorIndex = 0;
	}
}

void Heart::ChangeAnim() {
	animIndex++;
	if (animIndex >= cAnimCount) {
		animIndex = 0;
	}
}

void Heart::AnimScale() {
	XX_BEGIN(_1);
	for (scale = 1.f; scale.x > 0.75f; scale -= gg.delta) {
		XX_YIELD(_1);
	}
	for (scale = 0.75f; scale.x < 1.f; scale += gg.delta) {
		XX_YIELD(_1);
	}
	XX_YIELD_TO_BEGIN(_1);
	XX_END(_1);
}

void Heart::AnimBounce() {
	XX_BEGIN(_2);
	for (_2x = 0.0834f; _2x < 0.916f; _2x += gg.delta) {
		{
			auto r = xx::CalcBounce(_2x);
			scale = { r, 2.f - r };
		}
		XX_YIELD(_2);
	}
	XX_YIELD_TO_BEGIN(_2);
	XX_END(_2);
}

void Heart::Init() {
	scale = 1.f;
}

void Heart::Update() {
	switch (animIndex) {
	case 0:
		AnimBounce();
		break;
	case 1:
		AnimScale();
		break;
	}
}

void Heart::Draw() {
	auto& t = *gg.texs.heart;
	gg.Quad().Draw(t, t.Rect(), gg.cam.ToGLPos({}), 0.5f
		, 20.f * scale * gg.cam.scale, 0, 1, cColors[colorIndex]);
}
