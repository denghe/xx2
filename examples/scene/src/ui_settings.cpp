#include "pch.h"
#include "ui_settings.h"

namespace UI {

	/*
+-----------------------------+
		   Settings

		   todo

					  [ESC]back
+-----------------------------+
*/

	void Settings::Init(int32_t z_) {
		escHandler = true;
		InitDerived<Settings>(z_);

		static constexpr XY cSize{ 1100, 800 };
		static constexpr XY cItemSize{ 1000, 80 };
		static constexpr float cLineHeight{ 100 };
		static constexpr float cSliderWidths[]{ 400, 450, 150 };

		// border
		MakeChildren<xx::Scale9>()->Init(z + 1, 0, 0.5f, cSize);

		MakeChildren<xx::Label>()->Init(z + 2, gg.p5 + XY{ 0, 450 }, gg.a5, 120)(gg.lang(Strs::settings));

		auto basePos = gg.p5;
		auto offset = basePos + XY{ 0, 200 };
		auto anchor = gg.a5;

		// todo: block unavailable‌ resolutions
		uiResolutions = MakeChildren<xx::DropDownList>();
		uiResolutions->InitBegin(z + 2, offset, anchor, cItemSize);
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
		uiWindowModes = MakeChildren<xx::DropDownList>();
		uiWindowModes->InitBegin(z + 2, offset, anchor, cItemSize);
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
			Init(z);	// unsafe
			};

		offset.y -= cLineHeight * 2;
		MakeChildren<xx::CheckBox>()->Init(z + 2, offset, anchor, cItemSize, false)(gg.lang(Strs::mute))
			.onValueChanged = [this](bool v) {
			uiAudioVolume->SetEnabledRecursive(!v);
			uiMusicVolume->SetEnabledRecursive(!v);
			};

		offset.y -= cLineHeight;
		uiAudioVolume = MakeChildren<xx::Slider>();
		uiAudioVolume->Init(z + 2, offset, anchor, cItemSize.y
			, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2], 0.5)(gg.lang(Strs::audioVolume))
			.onChanged = [this](double v) {
			// todo
			};

		offset.y -= cLineHeight;
		uiMusicVolume = MakeChildren<xx::Slider>();
		uiMusicVolume->Init(z + 2, offset, anchor, cItemSize.y
			, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2], 1)(gg.lang(Strs::musicVolume))
			.onChanged = [this](double v) {
			// todo
			};
	}
}
