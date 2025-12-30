#pragma once
#include "g2_scene.h"

namespace G2 {

	struct WoodFactory : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		static XY PivotOffset();	// center - pivot
		XY offset{};
		float cos{}, sin{};
		int32_t i{}, j{};
		int32_t _1{}, _2{};
		float nextEffectTime{};
		bool shaking{};
		void ShakeA();
		void ShakeB();
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
		void Draw() override;
		void Dispose() override;
		~WoodFactory() override;
	};

}
