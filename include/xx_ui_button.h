#pragma once
#include "xx_ui_scale9.h"

namespace xx {

	struct InteractionNode : MouseEventHandlerNode {
		std::function<void()> onFocus = [] {
			auto g = GameBase_shader::GetInstance();
			g->PlayAudio(g->embed.ss_ui_focus);
		};

		Shared<Scale9Config> cfgNormal, cfgHighlight;

		Shared<Scale9Config> const& GetCfg() const {
			if (selected || focused) {
				return cfgHighlight;
			}
			return cfgNormal;
		}
		// need override
		virtual void ApplyCfg() {
			//At<Scale9>(children.len - 1).Init(.....
		}

		virtual void SetFocus() {
			assert(!focused);
			focused = true;
			SetToUIHandler(true);
			ApplyCfg();
			onFocus();
			//CoutN("SetFocus");
		}

		virtual void LostFocus() {
			if (!focused) return;
			SetToUIHandler(false);
			focused = false;
			ApplyCfg();
			//CoutN("LostFocus");
		}

		// todo: focus navigate by joystick? ASDW? prev next?
	};

	struct Button : InteractionNode {
		static constexpr int32_t cTypeId{ 10 };

		std::function<void()> onClicked = [] { CoutN("Button1 clicked."); };
		std::function<void()> onClicked2 = [] { CoutN("Button2 clicked."); };

		Button& Init(int32_t z_, XY position_, XY anchor_, XY size_
			, Shared<Scale9Config> cfgNormal_ = GameBase_shader::GetInstance()->embed.cfg_s9bN
			, Shared<Scale9Config> cfgHighlight_ = GameBase_shader::GetInstance()->embed.cfg_s9bH) {
			//assert(typeId == cTypeId);
			z = z_;
			position = position_;
			anchor = anchor_;
			size = size_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			FillTrans();
			auto& cfg = GetCfg();
			Make<Scale9>()->Init(z, 0, 0, size, cfg);
			return *this;
		}

		virtual void ApplyCfg() override {
			At<Scale9>(0).Init(z, 0, 0, size, GetCfg());
		}

		// todo: enable disable

		virtual int32_t OnMouseDown(int32_t btnId_) override {
			if (btnId_ != GLFW_MOUSE_BUTTON_LEFT
				&& btnId_ != GLFW_MOUSE_BUTTON_RIGHT) return 0;
			if (!enabled) return 0;
			LostFocus();
			if (btnId_ == GLFW_MOUSE_BUTTON_LEFT) {
				onClicked();
			}
			else {
				onClicked2();
			}
			return 1;
		}

		virtual int32_t OnMouseMove() override {
			if (!enabled) return 0;
			if (!focused) {
				SetFocus();
				TryRegisterAutoUpdate();
			}
			return 1;
		}

		virtual int32_t Update() override {
			if (GameBase::instance->uiHandler.TryGetPointer() != this || !MousePosInArea()) {
				LostFocus();
				return 1;
			}
			return 0;
		}

	};

}
