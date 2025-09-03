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

	static constexpr XY cSize{ 1000, 80 };
	static constexpr float cLineHeight{ 100 };
	static constexpr float cSliderWidths[]{ 400, 450, 150 };

	ui->MakeChildren<xx::Label>()->Init(2, gg.p5 + XY{ 0, 450 }, gg.a5, 120)(gg.lang(Strs::settings));

	auto basePos = gg.p5;
	auto offset = basePos + XY{ 0, 200 };
	auto anchor = gg.a5;

	// todo: block unavailable‌ resolutions
	ddlResolutions = ui->MakeChildren<xx::DropDownList>();
	ddlResolutions->InitBegin(2, offset, anchor, cSize);
	ddlResolutions->items.Add("1280x720", "1366x768", "1920x1080", "2560x1440", "3840x2160");
	ddlResolutions->InitEnd(2);	// todo: restore from backup?
	ddlResolutions->onSelectedIndexChanged = [](int32_t idx) {
		switch (idx) {
		case 0:
			gg.SetFullScreenMode({ 1280,720 });
			break;
		case 1:
			gg.SetFullScreenMode({ 1366,768 });
			break;
		case 2:
			gg.SetFullScreenMode({ 1920,1080 });
			break;
		case 3:
			gg.SetFullScreenMode({ 2560,1440 });
			break;
		case 4:
			gg.SetFullScreenMode({ 3840,2160 });
			break;
		default:
			assert(false);
		}
	};

	offset.y += cLineHeight;
	ddlWindowModes = ui->MakeChildren<xx::DropDownList>();
	ddlWindowModes->InitBegin(2, offset, anchor, cSize);
	ddlWindowModes->items.Add("window mode", "full screen (borderless)", "full screen (exclusive)");
	int i{};
	if (gg.isFullScreen) {
		if (gg.isBorderless) {
			i = 1;
			ddlResolutions->SetEnabledRecursive(false);
		}
		else {
			i = 2;
			ddlResolutions->SetEnabledRecursive(true);
		}
	}
	else {
		ddlResolutions->SetEnabledRecursive(false);
	}
	ddlWindowModes->InitEnd(i);
	ddlWindowModes->onSelectedIndexChanged = [this](int i) {
		switch (i) {
		case 0:
			gg.SetWindowMode();
			break;
		case 1:
			gg.SetBorderlessMode();
			break;
		case 2:
			ddlResolutions->onSelectedIndexChanged(ddlResolutions->selectedIndex);
			break;
		default:
			assert(false);
		}
		Init();	// unsafe
	};

	offset.y -= cLineHeight * 2;
	ui->MakeChildren<xx::CheckBox>()->Init(2, offset, anchor, cSize, true)("mute")
		.onValueChanged = [](bool v) {
		// todo
	};

	offset.y -= cLineHeight;
	ui->MakeChildren<xx::Slider>()->Init(2, offset, anchor, cSize.y
		, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2], 0.5)("asdf");
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
