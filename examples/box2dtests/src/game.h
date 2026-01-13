#pragma once
#include "pch.h"
#include "_phys.h"
#include "pics.h"

struct Game : xx::GameBase {
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };
	static constexpr float cMaxDelta{ 0.1f };
	bool isShowInfo{ true }, isLimitFPS{ true };
	std::string fpsVal;
	xx::Shared<xx::Node> ui;
	xx::Weak<xx::Label> uiFPS;
	xx::Weak<xx::Label> uiText;
	bool uiColorFlag{};

	xx::Shared<xx::SceneBase> scene, oldScene;	// oldScene: delay remove( after draw )
	template<typename T>
	xx::Shared<T>& MakeScene() {
		oldScene = std::move(scene);
		scene = xx::MakeShared<T>();
		return (xx::Shared<T>&)scene;
	}

	// runtime vars
	xx::Rnd rnd;
	xx::SpaceGridRingDiffuseData sgrdd;
	// ...

	pics pics;

	xx::List<std::pair<xx::Frame*, _phys::InitFunc>> fis;


	void Init() override;
	void GLInit() override;
	void Update() override;
	void Delay() override;
	void Stat() override;
	void OnResize(bool modeChanged_) override;
	void OnFocus(bool focused_) override;
};
extern Game gg;
