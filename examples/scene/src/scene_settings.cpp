#include "pch.h"
#include "scene_mainmenu.h"
#include "scene_settings.h"

void Scene_Settings::Init() {
	// init ui
	ui.Emplace()->InitRoot(gg.scale);

	// todo: iconbutton richlabel

	// todo: enable disable

	// todo: 
	// window mode
	// Full screen border less mode
	// Full screen exclusive mode

	// settings
	// music / sound volume
	// keyboard remap ?
	// back:[esc]

#if 1
	auto ddl = ui->MakeChildren<xx::DropDownList>();
	ddl->InitBegin(2, gg.p5 + XY{ 0, 100 }, gg.a5, { 500, 80 });
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
	//ddl->SetEnabledRecursive(false);

	// todo: need refresh ui values after set xxxxx mode
	// todo: block unavailable‌ resolutions
#endif

	ui->MakeChildren<xx::Label>()->Init(2, gg.p5 + XY{ 0, 300 }, gg.a5, 5)
		.SetText(gg.lang(Strs::settings));

	// borderless logic
	ui->MakeChildren<xx::CheckBox>()->Init(2, gg.p5 + XY{ 0, 0 }, gg.a5
		, { 500, 80 }, true
		, gg.lang(Strs::fullScreen), gg.isBorderless)
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
