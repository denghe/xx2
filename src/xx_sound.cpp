#include "xx_sound.h"

namespace xx {

	void Sound::Init() {
		playingWavs.clear();
		// small buffer for low latency. default is 0 ( auto: 4096 )
		// soloud.Emplace()->init(SoLoud::Soloud::CLIP_ROUNDOFF, 0, 0, 1024);
		soloud.Emplace()->init(0, 0, 0, 1024);
		bgm.h = {};
	}

	int32_t Sound::Update() {
		if (!soloud) return 0;
		if (soloud->mReinitFailedCount > 3) {
			auto t = soloud->getStreamPosition(bgm.h);
			Init();
			if (bgm.w && t > 0.f) {
				auto h = PlayBGM(bgm.w, bgm.volume, bgm.pan, bgm.speed, bgm.loop);
				soloud->seek(h, t);
			}
			return 1;
		}
		return 0;
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
		auto h = soloud->play(*w, volume, pan);
		if (speed != 1.f) {
			soloud->setRelativePlaySpeed(h, speed);
		}
		return h;
	}

	SoLoud::handle Sound::PlayBGM(SoLoud::Wav* w, float volume, float pan, float speed, bool loop) {
		soloud->stop(bgm.h);
		bgm.w = WeakFromThis(w);
		bgm.h = PlayDirect(w, volume, pan, speed);
		if (loop) {
			soloud->setLooping(bgm.h, true);
		}
		bgm.volume = volume;
		bgm.pan = pan;
		bgm.speed = speed;
		bgm.loop = loop;
		return bgm.h;
	}

	void Sound::StopBGM() {
		soloud->stop(bgm.h);
		bgm = {};
	}

	void Sound::SetBGMVolume(float v) {
		bgm.volume = v;
		soloud->setVolume(bgm.h, v);
	}

	void Sound::Stop(SoLoud::handle const& h) {
		soloud->stop(h);
	}

	void Sound::StopAll(bool _includeBGM) {
		float t{};
		if (!_includeBGM) {
			t = soloud->getStreamPosition(bgm.h);
		}
		soloud->stopAll();
		playingWavs.clear();
		if (!_includeBGM && bgm.w) {
			auto h = PlayBGM(bgm.w, bgm.volume, bgm.pan, bgm.speed, bgm.loop);
			if (t > 0.f) {
				soloud->seek(h, t);
			}
		}
	}

	void Sound::SetPauseAll(bool b) {
		soloud->setPauseAll(b);
	}

	unsigned int Sound::GetActiveVoiceCount() {
		return soloud->getActiveVoiceCount();
	}

}
