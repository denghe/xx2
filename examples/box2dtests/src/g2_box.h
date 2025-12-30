#pragma once
#include "g2_woodfactory.h"

namespace G2 {

	struct FlyingWood;
	struct Box : WoodFactory {
		static constexpr int32_t cTypeId{ __LINE__ };
		static XY PivotOffset();	// center - pivot
		xx::List<xx::Shared<FlyingWood>> flyingWoods;
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
		void Draw() override;
	};

}
