#pragma once
#include "game.h"

namespace G {

	struct Scene;
	struct Fish {
		Scene* scene{};
		Fish* next{};
		xx::Frame frame;
		xx::B2Body b2body;
		int32_t indexAtContainer{ -1 };
		float scale{};
		float y{};
		bool isDead{};
		float Init(Scene* scene_, XY pos_, float scale_ = 1);
		bool Update();
		void Draw();
		void Dispose();	// unsafe
	};

}
