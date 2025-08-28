#pragma once
#include "xx_ui_scale9.h"

namespace xx {

	// todo: ScrollView Richxxxx

	struct InteractionNode : MouseEventHandlerNode {
		static constexpr FromTo<int32_t> typeIdRange { 10, 20 };	// is InteractionNode check

		std::function<void()> onFocus = [] {};	// play sound?

		Ref<Scale9Config> cfgNormal, cfgHighlight;
		bool isFocus{}, alwaysHighlight{};

		virtual void SetFocus() { assert(!isFocus); isFocus = true; };
		virtual void LostFocus() { assert(isFocus); isFocus = false; };

		// todo: focus navigate? prev next?

		Scale9Config& GetCfg() {
			return alwaysHighlight ? *cfgHighlight : (isFocus ? *cfgHighlight : *cfgNormal);
		}
	};

	struct Button : InteractionNode {
		static constexpr int32_t cTypeId{ 10 };

		std::function<void()> onClicked = [] { CoutN("Button clicked."); };

		Button& Init(int z_, XY position_, XY anchor_, XY size_
			, Ref<Scale9Config> cfgNormal_, Ref<Scale9Config> cfgHighlight_) {
			typeId = cTypeId;
			z = z_;
			position = position_;
			anchor = anchor_;
			size = size_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			FillTrans();
			auto& cfg = GetCfg();
			MakeChildren<Scale9>()->Init(z + 1, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);
			return *this;
		}

		virtual void ApplyCfg() {
			assert(children.len);
			auto& cfg = GetCfg();
			//CoutN(cfg.frame.tex->FileName());
			auto bg = (Scale9*)children[0].pointer;
			bg->Init(z + 1, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);
		}

		void SetFocus() override {
			assert(!isFocus);
			isFocus = true;
			GameBase::instance->uiHandler = WeakFromThis(this);
			ApplyCfg();
			onFocus();
			//CoutN("SetFocus");
		}

		void LostFocus() override {
			if (!isFocus) return;
			if (GameBase::instance->uiHandler.TryGetPointer() == this) {
				GameBase::instance->uiHandler.Reset();
			}
			isFocus = false;
			ApplyCfg();
			//CoutN("LostFocus");
		}

		// todo: enable disable

		virtual int32_t OnMouseDown(int32_t btnId_) override {
			if (btnId_ != GLFW_MOUSE_BUTTON_LEFT) return 0;
			LostFocus();
			onClicked();
			return 1;
		}

		virtual int32_t OnMouseMove() override {
			if (!isFocus) {
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
