#pragma once
#include "xx_ui_scale9.h"

namespace xx {

	struct InteractionNode : MouseEventHandlerNode {
		std::function<void()> onFocus = [] {
			auto g = GameBase_shader::GetInstance();
			g->Play(g->embed.ss_ui_focus);
		};

		Ref<Scale9Config> cfgNormal, cfgHighlight;

		Ref<Scale9Config> const& GetCfg() const {
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

		std::function<void()> onClicked = [] { CoutN("Button clicked."); };

		Button& Init(int z_, XY position_, XY anchor_, XY size_
			, Ref<Scale9Config> cfgNormal_ = GameBase_shader::GetInstance()->embed.cfg_s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_shader::GetInstance()->embed.cfg_s9bH) {
			assert(typeId == cTypeId);
			z = z_;
			position = position_;
			anchor = anchor_;
			size = size_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			FillTrans();
			auto& cfg = GetCfg();
			Make<Scale9>()->Init(z + 1, 0, 0, size, cfg);
			return *this;
		}

		virtual void ApplyCfg() override {
			assert(children.len);
			auto& cfg = GetCfg();
			auto bg = (Scale9*)children[0].pointer;
			bg->Init(z + 1, 0, 0, size, cfg);
		}

		// todo: enable disable

		virtual int32_t OnMouseDown(int32_t btnId_) override {
			if (btnId_ != GLFW_MOUSE_BUTTON_LEFT) return 0;
			if (!enabled) return 0;
			LostFocus();
			onClicked();
			return 1;
		}

		virtual int32_t OnMouseMove() override {
			if (!enabled) return 0;
			if (!focused) {
				SetFocus();
				GameBase::instance->delayFuncs.Emplace([w = WeakFromThis(this)] {
					auto p = w.TryGetPointer();
					if (!p) return 1;
					if (GameBase::instance->uiHandler.TryGetPointer() != p
						|| !p->MousePosInArea()) {
						p->LostFocus();
						return 1;
					}
					return 0;
				});
			}
			return 1;
		}

	};

}
