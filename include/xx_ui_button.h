#pragma once
#include "xx_ui_scale9.h"

namespace xx {

	struct InteractionNode : MouseEventHandlerNode {
		std::function<void()> onFocus = [] {
			auto g = GameBase::instance;
			g->PlayAudio(g->embed.ss_ui_focus);
		};

		Shared<Scale9Config> cfgNormal, cfgHighlight;

		Shared<Scale9Config> const& GetCfg() const;

		// need override
		virtual void ApplyCfg() {
			//At<Scale9>(children.len - 1).Init(.....
		}

		virtual void SetFocus();
		virtual void LostFocus();

		// todo: focus navigate by joystick? ASDW? prev next?
	};

	struct Button : InteractionNode {
		static constexpr int32_t cTypeId{ 10 };

		std::function<void()> onClicked = [] { printf("Button1 clicked.\n"); };
		std::function<void()> onClicked2 = [] { printf("Button2 clicked.\n"); };

		Button& Init(int32_t z_, XY position_, XY anchor_, XY size_
			, Shared<Scale9Config> cfgNormal_ = GameBase::instance->embed.cfg_s9bN
			, Shared<Scale9Config> cfgHighlight_ = GameBase::instance->embed.cfg_s9bH);

		virtual void ApplyCfg() override;
		// todo: enable disable
		virtual int32_t OnMouseDown(int32_t btnId_) override;
		virtual int32_t OnMouseMove() override;
		virtual int32_t Update() override;

	};

}
