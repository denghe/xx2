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
	uiResolutions = ui->MakeChildren<xx::DropDownList>();
	uiResolutions->InitBegin(2, offset, anchor, cSize);
	uiResolutions->items.Add(U"1280 x 720", U"1366 x 768", U"1920 x 1080", U"2560 x 1440", U"3840 x 2160");
	uiResolutions->InitEnd(2);	// todo: restore from backup?
	uiResolutions->onSelectedIndexChanged = [](int32_t idx) {
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
	uiWindowModes = ui->MakeChildren<xx::DropDownList>();
	uiWindowModes->InitBegin(2, offset, anchor, cSize);
	uiWindowModes->items.Add(
		gg.lang(Strs::windowMode), 
		gg.lang(Strs::fullScreenBorderless),
		gg.lang(Strs::fullScreenExclusive)
	);
	int i{};
	if (gg.isFullScreen) {
		if (gg.isBorderless) {
			i = 1;
			uiResolutions->SetEnabledRecursive(false);
		}
		else {
			i = 2;
			uiResolutions->SetEnabledRecursive(true);
		}
	}
	else {
		uiResolutions->SetEnabledRecursive(false);
	}
	uiWindowModes->InitEnd(i);
	uiWindowModes->onSelectedIndexChanged = [this](int i) {
		switch (i) {
		case 0:
			gg.SetWindowMode();
			break;
		case 1:
			gg.SetBorderlessMode();
			break;
		case 2:
			uiResolutions->onSelectedIndexChanged(uiResolutions->selectedIndex);
			break;
		default:
			assert(false);
		}
		Init();	// unsafe
	};

	offset.y -= cLineHeight * 2;
	ui->MakeChildren<xx::CheckBox>()->Init(2, offset, anchor, cSize, false)(gg.lang(Strs::mute))
		.onValueChanged = [this](bool v) {
		uiAudioVolume->SetEnabledRecursive(!v);
		uiMusicVolume->SetEnabledRecursive(!v);
	};

	offset.y -= cLineHeight;
	uiAudioVolume = ui->MakeChildren<xx::Slider>();
	uiAudioVolume->Init(2, offset, anchor, cSize.y
		, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2], 0.5)(gg.lang(Strs::audioVolume))
		.onChanged = [this](double v) {
		// todo
	};

	offset.y -= cLineHeight;
	uiMusicVolume = ui->MakeChildren<xx::Slider>();
	uiMusicVolume->Init(2, offset, anchor, cSize.y
		, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2], 1)(gg.lang(Strs::musicVolume))
		.onChanged = [this](double v) {
		// todo
	};
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
