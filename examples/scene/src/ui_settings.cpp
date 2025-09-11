#include "pch.h"
#include "ui_settings.h"

namespace UI {

	/*
+-----------------------------+
		   title
        window modes
		resolutions
		   mute
        master volume
        audio volume
	    music volume
					  [ESC]back
+-----------------------------+
*/

	void Settings::Init(int32_t z_) {
		escHandler = true;
		children.Clear();	// unsafe
		InitDerived<Settings>(z_);

		static constexpr XY cSize{ 1100, 900 };
		static constexpr XY cItemSize{ 1000, 80 };
		static constexpr float cLineHeight{ 100 };
		static constexpr float cSliderWidths[]{ 400, 450, 150 };

		auto basePos = gg.p5;
		auto offset = basePos + XY{ 0, 300 };
		auto anchor = gg.a5;
		auto fontSize = cItemSize.y - gg.embed.cfg_s9bN->paddings.TopBottom();

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
		for (auto& xy : ::Settings::cResolutions) {
			uiResolutions->items.Add( xx::StringU8ToU32( xx::ToString( (uint32_t)xy.x, "x", (uint32_t)xy.y) ));
		}
		uiResolutions->InitEnd(gg.settings.resolutionsIndex);
		uiResolutions->onSelectedIndexChanged = [](int32_t idx) {
			gg.SetFullScreenMode(::Settings::cResolutions[idx]);
		};

		offset.y -= cLineHeight;
		Make<xx::CheckBox>()->Init(z + 2, offset, anchor, cItemSize, gg.mute)(gg.lang(Strs::mute))
			.onValueChanged = [this](bool v) {
			gg.mute = v;
			uiMasterVolume->SetEnabledRecursive(!v);
			uiAudioVolume->SetEnabledRecursive(!v);
			uiMusicVolume->SetEnabledRecursive(!v);
		};

		offset.y -= cLineHeight;
		uiMasterVolume = Make<xx::Slider>();
		uiMasterVolume->Init(z + 2, offset, anchor, cItemSize.y
			, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2], gg.masterVolume)(gg.lang(Strs::masterVolume))
			.onChanged = [this](double v) {
			gg.masterVolume = v;
			gg.sound.SetGlobalVolume(v);
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
		if (gg.settings.mute) {
			uiMasterVolume->SetEnabledRecursive(false);
			uiAudioVolume->SetEnabledRecursive(false);
			uiMusicVolume->SetEnabledRecursive(false);
		}
	}

	void Settings::HandleESC() {
		gg.settings.isBorderless = gg.isBorderless;
		gg.settings.isFullScreen = gg.isFullScreen;
		gg.settings.resolutionsIndex = uiResolutions->selectedIndex;

		gg.settings.mute = gg.mute;
		gg.settings.masterVolume = gg.masterVolume;
		gg.settings.audioVolume = gg.audioVolume;
		gg.settings.musicVolume = gg.musicVolume;

		gg.Save();	// todo: handle rtv
		SwapRemove();
	}
}
