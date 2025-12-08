#pragma once
#include <soloud.h>
#include <soloud_wav.h>
//#include <soloud_file.h>

// todo: opus support

namespace xx {
	struct SoundSource {
		SoLoud::Wav wav;
	};

	struct Sound {
		SoLoud::Soloud soloud;
		float globalVolume{ 1 };

		XX_INLINE void Init() {
			// small buffer for low latency. default is 0 ( auto: 4096 )
			soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF, 0, 0, 1024);
		}

		XX_INLINE void SetGlobalVolume(float v) {
			globalVolume = v;
			soloud.setGlobalVolume(v);
		}

		XX_INLINE unsigned int GetActiveVoiceCount() {
			if (globalVolume == 0.f) return 0;
			return soloud.getActiveVoiceCount();
		}

		XX_INLINE int Play(Shared<SoundSource> const& ss, float volume = 1.f, float pan = 0.f, float speed = 1.f) {
			if (globalVolume == 0.f) return -1;
			int h = soloud.play(ss->wav, volume, pan);
			if (speed != 1.f) {
				soloud.setRelativePlaySpeed(h, speed);
			}
			return h;
		}

		XX_INLINE void Stop(int h) {
			soloud.stop(h);
		}

		XX_INLINE void StopAll() {
			soloud.stopAll();
		}

		XX_INLINE void SetPauseAll(bool b) {
			soloud.setPauseAll(b);
		}

		// todo: more wrapper functions
	};

}
