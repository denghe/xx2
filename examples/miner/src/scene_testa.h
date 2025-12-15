#pragma once
#include "game.h"

namespace TestA {

	struct Scene;
	struct SceneItem {
		Scene* scene{};
		SceneItem* next{};
		float y{};
		virtual void Draw() {}
		virtual ~SceneItem() {}
	};

	struct Miner : SceneItem {
		xx::Frame* frames{};
		int32_t framesLen{};
		int32_t frameIndex{};
		XY pos{};
		void Init(Scene* scene_, int32_t index_, XY pos_);
		void Update();
		void Draw() override;
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::List<xx::Shared<Miner>> miners;
		void Gen(int32_t count_);

		xx::List<SceneItem*> sortContainer;				// for draw order by Y
		void SortContainerAdd(SceneItem* o);
		void SortContainerDraw();

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
