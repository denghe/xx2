#pragma once
#include "g2_scene.h"

namespace G2 {

	struct Box;
	struct FlyingWood : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		Box* owner{};
		XY inc{};
		int32_t numSteps{};
		int32_t _1{};
		void Init(Box* owner_, SceneItem* tar_);
		bool Update() override;
		void Draw() override;
		void Dispose() override;
	};

}
