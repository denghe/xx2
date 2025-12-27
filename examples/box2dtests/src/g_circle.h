#pragma once
#include "game.h"

namespace G {

	struct Scene;
	struct Circle {
		Scene* scene{};
		float radius{};
		xx::B2Body b2body;
		void Init(Scene* scene_, XY pos_, float radius_);
		bool Update();
		void Draw();
	};

}
