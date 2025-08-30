#include "pch.h"
#include "scene_mainmenu.h"
#include "scene_settings.h"

void Scene_Settings::Init() {
	// init ui
	ui.Emplace()->InitRoot(gg.scale);

	// todo: enable disable
	// todo: iconbutton richlabel

	// settings
	// music / sound volume
	// back:[esc]

#if 1
	auto ddl = ui->MakeChildren<xx::DropDownList>();
	ddl->InitBegin(2, gg.p5 + XY{ 0, 100 }, gg.a5
		, { 500, 80 }, gg.cfg.s9bN, gg.cfg.s9bH, gg.cfg.s9bg
		, gg.res.ui_dropdownlist_icon, gg.res.ui_dropdownlist_head);
	ddl->items.Add("1280x720", "1366x768", "1920x1080", "2560x1440", "3840x2160");
	ddl->InitEnd(2);
	ddl->onSelectedIndexChanged = [](int32_t idx) {
		switch (idx) {
		case 0:
			gg.SetWindowMode({ 1280,720 });
			break;
		case 1:
			gg.SetWindowMode({ 1366,768 });
			break;
		case 2:
			gg.SetWindowMode({ 1920,1080 });
			break;
		case 3:
			gg.SetWindowMode({ 2560,1440 });
			break;
		case 4:
			gg.SetWindowMode({ 3840,2160 });
			break;
		default:
			assert(false);
		}
	};
	// todo: need refresh ui values after set xxxxx mode
	// todo: block unavailable‌ resolutions
#endif

	ui->MakeChildren<xx::Label>()->Init(2, gg.p5 + XY{ 0, 300 }, gg.a5, 5)
		.SetText(gg.lang(Strs::settings));

	// borderless logic
	ui->MakeChildren<xx::CheckBox>()->Init(2, gg.p5 + XY{ 0, 0 }, gg.a5
		, { 500, 80 }, true, gg.cfg.s9bN, gg.cfg.s9bH
		, gg.res.ui_checkbox_0, gg.res.ui_checkbox_1, gg.lang(Strs::fullScreen), gg.isBorderless)
		.onValueChanged = [](bool v) {
		if (v) {
			gg.SetBorderlessMode();
		}
		else {
			gg.SetWindowMode();
		}
	};

	ui->MakeChildren<xx::Slider>()->Init(
		2, gg.p5 + XY{ 0, -100 }, gg.a5
		, gg.cfg.s9bN, gg.cfg.s9bH, gg.cfg.sbar, gg.cfg.sblock
		, 80, 400, 450, 150, "asdf", 0.5);


}

void Scene_Settings::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}
}

void Scene_Settings::Draw() {
	gg.DrawNode(ui);
}

void Scene_Settings::OnResize() {
	ui->Resize(gg.scale);
}
