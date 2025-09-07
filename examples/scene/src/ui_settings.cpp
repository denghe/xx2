#include "pch.h"
#include "ui_settings.h"

namespace UI {

	/*
+-----------------------------+
		   title
        window modes
		resolutions
		   mute
        audio volume
	    music volume
					  [ESC]back
+-----------------------------+
*/

	// todo: Load & Save

	void Settings::Init(int32_t z_) {
		escHandler = true;
		children.Clear();	// unsafe
		InitDerived<Settings>(z_);

		static constexpr XY cSize{ 1100, 800 };
		static constexpr XY cItemSize{ 1000, 80 };
		static constexpr float cLineHeight{ 100 };
		static constexpr float cSliderWidths[]{ 400, 450, 150 };

		auto basePos = gg.p5;
		auto offset = basePos + XY{ 0, 300 };
		auto anchor = gg.a5;
		auto fontSize = cItemSize.y - gg.defaultCfg.s9bN->paddings.TopBottom();

		// border
		auto border = Make<xx::Scale9>();
		border->Init(z + 1, 0, 0.5f, cSize);

		// swallow bg
		Make<xx::Background>()->Init(z, border.ToWeak()).onOutsideClicked = [] {};

		// title
		Make<xx::Label>()->Init(z + 2, offset, anchor, fontSize * 2)(gg.lang(Strs::settings));

		offset.y -= cLineHeight * 1.5;
		uiWindowModes = Make<xx::DropDownList>();
		uiWindowModes->InitBegin(z + 2, offset, anchor, cItemSize);
		uiWindowModes->items.Add(gg.lang(Strs::windowMode), gg.lang(Strs::fullScreenBorderless), gg.lang(Strs::fullScreenExclusive));
		uiWindowModes->InitEnd();
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
			Init(z);	// unsafe
			};

		offset.y -= cLineHeight;
		uiResolutions = Make<xx::DropDownList>();
		uiResolutions->InitBegin(z + 2, offset, anchor, cItemSize);
		uiResolutions->items.Add(U"1280 x 720", U"1366 x 768", U"1920 x 1080", U"2560 x 1440", U"3840 x 2160");	// todo: block unavailable‌ resolutions
		uiResolutions->InitEnd(gg.resolutionsIndex);
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
			gg.resolutionsIndex = idx;
		};

		offset.y -= cLineHeight;
		Make<xx::CheckBox>()->Init(z + 2, offset, anchor, cItemSize, gg.mute)(gg.lang(Strs::mute))
			.onValueChanged = [this](bool v) {
			gg.mute = v;
			uiAudioVolume->SetEnabledRecursive(!v);
			uiMusicVolume->SetEnabledRecursive(!v);
		};

		offset.y -= cLineHeight;
		uiAudioVolume = Make<xx::Slider>();
		uiAudioVolume->Init(z + 2, offset, anchor, cItemSize.y
			, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2], gg.audioVolume)(gg.lang(Strs::audioVolume))
			.onChanged = [this](double v) {
			gg.audioVolume = v;
		};

		offset.y -= cLineHeight;
		uiMusicVolume = Make<xx::Slider>();
		uiMusicVolume->Init(z + 2, offset, anchor, cItemSize.y
			, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2], gg.musicVolume)(gg.lang(Strs::musicVolume))
			.onChanged = [this](double v) {
			gg.musicVolume = v;
		};

		// todo: esc back

		// apply config values
		if (gg.isFullScreen) {
			if (gg.isBorderless) {
				uiWindowModes->SetSelectedIndex(1);
				uiResolutions->SetEnabledRecursive(false);
			}
			else {
				uiWindowModes->SetSelectedIndex(2);
			}
		}
		else {
			uiResolutions->SetEnabledRecursive(false);
		}
		if (gg.mute) {
			uiAudioVolume->SetEnabledRecursive(false);
			uiMusicVolume->SetEnabledRecursive(false);
		}
	}
}
