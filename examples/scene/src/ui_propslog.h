#pragma once
#include "game.h"
#include "creature.h"

namespace UI {

	struct PropsLog : xx::Node {
		static constexpr int32_t cTypeId{ 106 };
		xx::Weak<Creature> creature;
		void Init(int32_t z_, xx::Weak<Creature> creature_);
	};

}
