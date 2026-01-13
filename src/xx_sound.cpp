#include "xx_sound.h"
//#include <soloud_file.h>

// todo: opus support

namespace xx {

	void Sound::Init() {
		// small buffer for low latency. default is 0 ( auto: 4096 )
		soloud.Emplace()->init(SoLoud::Soloud::CLIP_ROUNDOFF, 0, 0, 1024);
	}

	void Sound::SetGlobalVolume(float v) {
		globalVolume = v;
		soloud->setGlobalVolume(v);
	}

	unsigned int Sound::GetActiveVoiceCount() {
		if (globalVolume == 0.f) return 0;
		return soloud->getActiveVoiceCount();
	}

	int Sound::Play(SoLoud::Wav* ss, float volume, float pan, float speed) {
		if (globalVolume == 0.f) return -1;
		int h = soloud->play(*ss, volume, pan);
		if (speed != 1.f) {
			soloud->setRelativePlaySpeed(h, speed);
		}
		return h;
	}

	void Sound::Stop(int h) {
		soloud->stop(h);
	}

	void Sound::StopAll() {
		soloud->stopAll();
	}

	void Sound::SetPauseAll(bool b) {
		soloud->setPauseAll(b);
	}

}
