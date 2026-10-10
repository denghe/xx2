#pragma once
#include "xx_ptr.h"
#include "xx_queue.h"
#include <soloud.h>
#include <soloud_wav.h>

namespace xx {
	struct Sound {
		Shared<SoLoud::Soloud> soloud;
		// count limit play config ( per file. total MAX_ACTIVE_VOICE_COUNT voices )
		int32_t playConcurrentCount{ 16 };
		// maybe need clear when SoLoud::Wav released
		std::unordered_map<SoLoud::Wav*, Queue<SoLoud::handle>> playingWavs;
		struct {
			SoLoud::handle h{};
			Weak<SoLoud::Wav> w;
			float volume{};
			float pan{};
			float speed{};
			bool loop{};
		} bgm;

		void Init();
		// check default device changed & auto reinit
		// return !0: reinit
		int32_t Update();

		// for audio
		// count limit play
		SoLoud::handle Play(SoLoud::Wav* w, float volume = 1.f, float pan = 0.f, float speed = 1.f);
		SoLoud::handle PlayDirect(SoLoud::Wav* w, float volume = 1.f, float pan = 0.f, float speed = 1.f);
		void Stop(SoLoud::handle const& h);
		void StopAll(bool _includeBGM = true);

		// for background music
		SoLoud::handle PlayBGM(SoLoud::Wav* w, float volume = 1.f, float pan = 0.f, float speed = 1.f, bool loop = true);
		void StopBGM();

		// soloud function mappings
		void SetPauseAll(bool b);
		unsigned int GetActiveVoiceCount();
	};

}
