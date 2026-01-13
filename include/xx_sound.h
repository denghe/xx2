#pragma once
#include "xx_ptr.h"
#include <soloud.h>
#include <soloud_wav.h>

namespace xx {
	struct Sound {
		Shared<SoLoud::Soloud> soloud;
		float globalVolume{ 1 };

		void Init();
		void SetGlobalVolume(float v);
		unsigned int GetActiveVoiceCount();
		int Play(SoLoud::Wav* ss, float volume = 1.f, float pan = 0.f, float speed = 1.f);
		void Stop(int h);
		void StopAll();
		void SetPauseAll(bool b);

		// todo: more wrapper functions
	};

}
