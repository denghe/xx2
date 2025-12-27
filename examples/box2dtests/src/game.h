#pragma once
#include "pch.h"
using XY = xx::XY;
using XYi = xx::XYi;
#include "_phys.h"

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

	// runtime vars
	xx::Rnd rnd;
	xx::SpaceGridRingDiffuseData sgrdd;
	// ...

	struct {
		xx::Frame circle256;
		xx::Frame bg_fish;
		xx::Frame wood1;
		xx::Frame wood2;

		xx::Frame _10;
		xx::Frame _110;
		xx::Frame _123;
		xx::Frame _128;
		xx::Frame _138;
		xx::Frame _14;
		xx::Frame _147;
		xx::Frame _168;
		xx::Frame _17;
		xx::Frame _18;
		xx::Frame _192;
		xx::Frame _206;
		xx::Frame _243;
		xx::Frame _244;
		xx::Frame _26;
		xx::Frame _283;
		xx::Frame _285;
		xx::Frame _288;
		xx::Frame _296;
		xx::Frame _303;
		xx::Frame _309;
		xx::Frame _402;
		xx::Frame _407;
		xx::Frame _408;
		xx::Frame _415;
		xx::Frame _421;
		xx::Frame _422;
		xx::Frame _443;
		xx::Frame _449;
		xx::Frame _458;
		xx::Frame _461;
		xx::Frame _462;
		xx::Frame _463;
		xx::Frame _464;
		xx::Frame _467;
		xx::Frame _470;
		xx::Frame _472;
		xx::Frame _51;
		xx::Frame _75;
		xx::Frame _98;
	} fs;

	xx::List<std::pair<xx::Frame*, _phys::InitFunc>> fis;


	void Init();
	void GLInit();
	void Update();
	void Delay();
	void Stat();
	void OnResize(bool modeChanged_);
	void OnFocus(bool focused_);
};
extern Game gg;
