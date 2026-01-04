#pragma once
#include "pch.h"
#include "_phys.h"
#include "_pics.h"
using XY = xx::XY;
using XYi = xx::XYi;

struct MinerConfig {
	float animFPS, resRadius, moveSpeed, attackRange;
	int32_t fsLens[3]; // aIdleLen, aMoveLen, aAtkLen;
	xx::Frame* fss[3];	// aIdle, * aMove, * aAtk;
	char* cd;	// for atk. share aAtkLen
	xx::Weak<xx::SoundSource> ss;
};

struct SpineFrameConfig {
	XY size{}, offset{};
	float scale{}, delta{};
};

struct Game : xx::Game<Game> {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };
	static constexpr float cMaxDelta{ 0.1f };
	bool isShowInfo{ true }, isLimitFPS{ true };
	std::string fpsVal;
	xx::Shared<xx::Node> ui;
	xx::Weak<xx::Label> uiFPS;

	xx::Shared<xx::SceneBase> scene, oldScene;	// oldScene: delay remove( after draw )
	template<typename T>
	xx::Shared<T>& MakeScene() {
		oldScene = std::move(scene);
		scene = xx::MakeShared<T>();
		return (xx::Shared<T>&)scene;
	}

	_pics _pics;
	xx::STBImage _mask_bg_1;
	xx::List<xx::List<xx::Frame>> treeIdles;
	xx::List<xx::List<xx::Frame>> treeTurns;
	xx::List<xx::List<_phys::Circle>> treeCollisionDatas;
	xx::List<_phys::Circle> axeCollisionData;
	xx::List<xx::SpineEventData> spineEventDatas;

	// runtime vars
	xx::Rnd rnd;
	xx::SpaceGridRingDiffuseData sgrdd;
	// ...

	void Init();
	void GLInit();
	void Update();
	void Delay();
	void Stat();
	void OnResize(bool modeChanged_);
	void OnFocus(bool focused_);
};
extern Game gg;
