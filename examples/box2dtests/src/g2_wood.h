#pragma once
#include "g2_scene.h"

namespace G2 {

	struct Wood : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		XY offset{}, inc{};
		float cos{}, sin{};
		int32_t i{};
		int32_t _1{};
		bool ready{};
		void Anim();
		void Init(Scene* scene_, XY pos_);
		void PreUpdate();
		bool Update() override;
		void Draw() override;
		void Dispose() override;
		~Wood() override;
	};

}
