#pragma once
#include "xx_ptr.h"
#include "xx_queue.h"
#include <soloud.h>
#include <soloud_wav.h>

namespace xx {
	struct Sound {
		Shared<SoLoud::Soloud> soloud;
		// count limit play config ( total 128 voices )
		int32_t playConcurrentCount{ 8 };
		// maybe need clear when SoLoud::Wav released
		std::unordered_map<SoLoud::Wav*, Queue<SoLoud::handle>> playingWavs;
		SoLoud::handle bgm{};

		void Init();
		// global volume control, 0~1
		void SetMasterVolume(float v);
		// relate to Play() / PlayDirect()
		void SetAudioVolume(float v);
		// relate to PlayBGM()
		void SetMusicVolume(float v);

		// for audio
		// count limit play ( per file 8 voices, total 128 voices )
		SoLoud::handle Play(SoLoud::Wav* w, float volume = 1.f, float pan = 0.f, float speed = 1.f);
		SoLoud::handle PlayDirect(SoLoud::Wav* w, float volume = 1.f, float pan = 0.f, float speed = 1.f);
		void Stop(SoLoud::handle const& h);
		void StopAll();

		// for background music
		SoLoud::handle PlayBGM(SoLoud::Wav* w, float volume = 1.f, float pan = 0.f, float speed = 1.f, bool loop = true);
		void StopBGM();

		// soloud function mappings
		void SetPauseAll(bool b);
		unsigned int GetActiveVoiceCount();
	};

}
