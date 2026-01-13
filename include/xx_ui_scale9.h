#pragma once
#include "xx_node.h"
#include "xx_gamebase.h"

namespace xx {

	struct Scale9 : Node {
		static constexpr int32_t cTypeId{ 3 };
		Scale9Config cfg;

		// reepeatable call
		Scale9& Init(int32_t z_, XY position_, XY anchor_, XY size_
			, Shared<Scale9Config> cfg_ = GameBase::instance->embed.cfg_s9bg);

		Scale9& SetConfig(Shared<Scale9Config> cfg_ = GameBase::instance->embed.cfg_s9bg);

		virtual void Draw() override;
	};

}
