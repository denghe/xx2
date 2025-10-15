#pragma once
#include "pch.h"
using XY = xx::XY;

struct Game : xx::Game<Game> {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };
	static constexpr float cMaxDelta{ 0.1f };

	xx::Shared<xx::SceneBase> scene, oldScene;	// oldScene: delay remove( after draw )
	template<typename T>
	xx::Shared<T>& MakeScene() {
		oldScene = std::move(scene);
		scene = xx::MakeShared<T>();
		return (xx::Shared<T>&)scene;
	}

	struct {
		struct {
			xx::Ref<xx::GLTexture> tex;
			spine::SkeletonData* skel{};
			spine::Animation* idle{};
			spine::Animation* blink{};
			spine::Animation* up{};
			spine::Animation* right{};
			spine::Animation* down{};
			spine::Animation* left{};
		} owl;
		struct {
			xx::Ref<xx::GLTexture> tex;
			spine::SkeletonData* skel{};
			spine::Animation* walk{};
			spine::Animation* jump{};
			spine::Animation* aim{};
			spine::Animation* shoot{};
		} spineBoy;
		struct {
			xx::Ref<xx::GLTexture> tex;
			spine::SkeletonData* skel{};
			spine::Animation* idle{};
		} man1;
		struct {
			xx::Ref<xx::GLTexture> tex;
			spine::SkeletonData* skel{};
		} eye;
		struct {
			xx::Ref<xx::GLTexture> tex;
			spine::SkeletonData* skel{};
			spine::Animation* idle{};
		} grass1;
	} res;

	// runtime vars
	xx::Rnd rnd;
	// ...

	void Init();
	void GLInit();
	void Update();
	void Delay();
	void Stat();
	void OnResize(bool modeChanged_);
};
extern Game gg;
