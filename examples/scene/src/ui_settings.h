#pragma once
#include "game.h"

namespace UI {

	struct Settings : xx::Node {
		static constexpr int32_t cTypeId{ 101 };
		xx::Weak<xx::DropDownList> uiWindowModes, uiResolutions;
		xx::Weak<xx::Slider> uiAudioVolume, uiMusicVolume;
		void Init(int32_t z_);
		void HandleESC() override;
	};

}
