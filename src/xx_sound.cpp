#include "xx_sound.h"

namespace xx {

	void Sound::Init() {
		// small buffer for low latency. default is 0 ( auto: 4096 )
		soloud.Emplace()->init(SoLoud::Soloud::CLIP_ROUNDOFF, 0, 0, 4096);
	}

	void Sound::SetMasterVolume(float v) {
        GameBase::instance->masterVolume = v;
		soloud->setGlobalVolume(v);
	}

    void Sound::SetAudioVolume(float v) {
        GameBase::instance->audioVolume = v;
    }

    void Sound::SetMusicVolume(float v) {
        GameBase::instance->musicVolume = v;
    }

	SoLoud::handle Sound::Play(SoLoud::Wav* w, float volume, float pan, float speed) {
		assert(w);
		auto iter = playingWavs.find(w);
		if (iter == playingWavs.end()) {
			auto r = playingWavs.emplace(w, xx::Queue<SoLoud::handle>{});
			assert(r.second);
			r.first->second.Reserve(playConcurrentCount);
			iter = std::move(r.first);
		}
		auto& queue = iter->second;
		while (queue.Count() >= playConcurrentCount) {
			Stop(queue.Top());
			queue.Pop();
		}
		auto h = PlayDirect(w, volume, pan, speed);
		queue.Push(h);
		return h;
	}

	SoLoud::handle Sound::PlayDirect(SoLoud::Wav* w, float volume, float pan, float speed) {
		assert(w);
		if (GameBase::instance->masterVolume == 0.f) return -1;
		auto h = soloud->play(*w, volume, pan);
		if (speed != 1.f) {
			soloud->setRelativePlaySpeed(h, speed);
		}
		return h;
	}

	SoLoud::handle Sound::PlayBGM(SoLoud::Wav* w, float volume, float pan, float speed, bool loop) {
		soloud->stop(bgm);
		bgm = PlayDirect(w, volume, pan, speed);
		if (loop) {
			soloud->setLooping(bgm, true);
		}
		return bgm;
	}

	void Sound::StopBGM() {
		soloud->stop(bgm);
		bgm = {};
	}

	void Sound::Stop(SoLoud::handle const& h) {
		soloud->stop(h);
	}

	void Sound::StopAll() {
		soloud->stopAll();
		playingWavs.clear();
	}

	void Sound::SetPauseAll(bool b) {
		soloud->setPauseAll(b);
	}

	unsigned int Sound::GetActiveVoiceCount() {
		if (GameBase::instance->masterVolume == 0.f) return 0;
		return soloud->getActiveVoiceCount();
	}

}
