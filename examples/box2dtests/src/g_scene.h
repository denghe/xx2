#pragma once
#include "game.h"

namespace G {

	struct Fish;
	struct Circle;
	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::B2World b2world;
		xx::List<xx::Shared<Fish>> fishs;
		xx::Shared<Circle> circle;	// mouse

		float genTimer{};
		float lastGenY{};
		void Gen(int32_t num_);

		xx::List<Fish*> tmp;				// for draw order by Y & deleter
		void SortContainerAdd(Fish* o);
		void SortContainerDraw();

		void Init();
		void Update() override;
		virtual void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
