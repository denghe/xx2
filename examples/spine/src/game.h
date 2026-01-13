#pragma once
#include "pch.h"
#include "shader_grass.h"
#include <xx_grid2d_circle.h>

struct SpineRes1 {
	xx::Shared<xx::GLTexture> tex;
	spine::SkeletonData* skel{};
	spine::Animation* idle{};
};

struct Game : xx::GameBase {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };
	static constexpr float cMaxDelta{ 0.1f };

	xx::Shader_Grass shaderGrass;
	XX_INLINE xx::Shader_Grass& Grass() { return ShaderBegin(shaderGrass); }

	xx::Shared<xx::SceneBase> scene, oldScene;	// oldScene: delay remove( after draw )
	template<typename T>
	xx::Shared<T>& MakeScene() {
		oldScene = std::move(scene);
		scene = xx::MakeShared<T>();
		return (xx::Shared<T>&)scene;
	}

	struct {
		std::array<xx::TinyFrame, 14> brush_;
		struct {
			xx::Shared<xx::GLTexture> tex;
			spine::SkeletonData* skel{};
			spine::Animation* idle{};
			spine::Animation* blink{};
			spine::Animation* up{};
			spine::Animation* right{};
			spine::Animation* down{};
			spine::Animation* left{};
		} owl;
		struct {
			xx::Shared<xx::GLTexture> tex;
			spine::SkeletonData* skel{};
			spine::Animation* walk{};
			spine::Animation* jump{};
			spine::Animation* aim{};
			spine::Animation* shoot{};
		} spineBoy;
		struct {
			xx::Shared<xx::GLTexture> tex;
			spine::SkeletonData* skel{};
			spine::Animation* idle{};
		} man1;
		struct {
			xx::Shared<xx::GLTexture> tex;
			spine::SkeletonData* skel{};
		} eye;
		xx::List<SpineRes1> flower_;
		xx::List<SpineRes1> grass_;
	} res;

	// runtime vars
	xx::Rnd rnd;
	xx::SpaceGridRingDiffuseData sgrdd;
	// ...

	void Init() override;
	void GLInit() override;
	void Update() override;
	void Delay() override;
	void Stat() override;
	void OnResize(bool modeChanged_) override;
};
extern Game gg;
