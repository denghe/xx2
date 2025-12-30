#pragma once
#include "g2_scene.h"

namespace G2 {

	struct Wall : SceneItem {
		static constexpr int32_t cTypeId{__LINE__};
		void Init(Scene* scene_, XY pos_);
		void Draw() override;
		void Dispose();	// unsafe
		~Wall() override;
	};

}
