#pragma once
#include "xx_ui_label.h"
#include "xx_ui_scale9.h"
#include "xx_ui_button.h"

namespace xx {

	struct Slider : InteractionNode {
		static constexpr int32_t cTypeId{ 13 };

		Ref<Scale9Config> cfgBar, cfgBlock;
		float height{}, widthTxtLeft{}, widthBar{}, widthTxtRight{};
		double value{}, valueBak{};	// 0 ~ 1
		bool blockMoving{};

		std::function<void(double)> onChanged = [](double v) { CoutN("Slider changed. v = ", v); };
		std::function<std::string(double)> valueToString = [](double v)->std::string {
			return ToString(int32_t(v * 100));
			};

		// InitBegin + set value/ToSting + InitEnd
		template<typename S>
		Slider& Init(int z_, XY position_, XY anchor_
			, float height_, float widthTxtLeft_, float widthBar_, float widthTxtRight_
			, S const& txtLeft_, double value_
			, Ref<Scale9Config> cfgNormal_ = GameBase_ui::GetInstance()->defaultCfg.s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_ui::GetInstance()->defaultCfg.s9bH
			, Ref<Scale9Config> cfgBar_ = GameBase_ui::GetInstance()->defaultCfg.sbar
			, Ref<Scale9Config> cfgBlock_ = GameBase_ui::GetInstance()->defaultCfg.sblock
		)
		{
			assert(children.Empty());
			typeId = cTypeId;
			isFocus = false;
			z = z_;
			position = position_;
			anchor = anchor_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			cfgBar = std::move(cfgBar_);
			cfgBlock = std::move(cfgBlock_);
			height = height_;
			widthTxtLeft = widthTxtLeft_;
			widthBar = widthBar_;
			widthTxtRight = widthTxtRight_;
			blockMoving = false;
			SetValue(value_);
			assert(value >= 0 && value <= 1);

			auto& cfg = isFocus ? *cfgHighlight : *cfgNormal;
			MakeChildren<Label>()->Init(z + 1, { cfg.txtPadding.x * cfg.txtScale, cfg.txtPaddingRightBottom.y * cfg.txtScale }, {}, cfg.txtScale, cfg.txtColor).SetText(txtLeft_);

			size = { widthTxtLeft + widthBar + widthTxtRight, height };

			auto barMinWidth = cfgBar->center.x * 2. * cfgBar->borderScale;
			XY barSize{ std::max(widthBar * value, barMinWidth), height * 0.25f };
			XY barPos{ widthTxtLeft, (height - barSize.y) * 0.5f };
			MakeChildren<Scale9>()->Init(z + 1, barPos, {}, cfgBar->borderScale, barSize / cfgBar->borderScale, *cfgBar);

			XY blockSize{ height * 0.6f, height * 0.6f };
			XY blockPos{ widthTxtLeft + widthBar * value - blockSize.x * 0.5f, (height - blockSize.y) * 0.5f };
			MakeChildren<Scale9>()->Init(z + 2, blockPos, {}, cfgBlock->borderScale, blockSize / cfgBlock->borderScale, *cfgBlock);

			auto txtRight = valueToString(value);
			MakeChildren<Label>()->Init(z + 1, { size.x - cfg.txtPadding.x * cfg.txtScale, cfg.txtPaddingRightBottom.y * cfg.txtScale }, { 1, 0 }, cfg.txtScale, cfg.txtColor).SetText(txtRight);
			MakeChildren<Scale9>()->Init(z, {}, {}, cfg.borderScale, size / cfg.borderScale, cfg);
			FillTransRecursive();

			return *this;
		}

		void ApplyCfg() override {
			assert(children.len == 5);		// lblLeft, bar, block, lblRight, bg
			auto& cfg = isFocus ? *cfgHighlight : *cfgNormal;
			assert(children[4]->typeId == Scale9::cTypeId);
			((Scale9*)children[4].pointer)->Init(z, {}, {}, cfg.borderScale, size / cfg.borderScale, cfg);
		}

		Slider& SetValue(double v) {
			value = valueBak = v;
			return *this;
		}

		void ApplyValue() {
			assert(value >= 0 && value <= 1);

			auto barMinWidth = cfgBar->center.x * 2. * cfgBar->borderScale;
			XY barSize{ std::max(widthBar * value, barMinWidth), height * 0.25f };
			XY barPos{ widthTxtLeft, (height - barSize.y) * 0.5f };
			auto bar = ((Scale9*)children[1].pointer);
			bar->Init(z + 1, barPos, {}, cfgBar->borderScale, barSize / cfgBar->borderScale, *cfgBar);

			XY blockSize{ height * 0.6f, height * 0.6f };
			XY blockPos{ widthTxtLeft + widthBar * value - blockSize.x * 0.5f, (height - blockSize.y) * 0.5f };
			auto block = ((Scale9*)children[2].pointer);
			block->position = blockPos;
			block->FillTrans();

			LabelRight().SetText(valueToString(value));
		}

		Label& LabelLeft() {
			assert(children.len == 5);
			assert(children[0]->typeId == Label::cTypeId);
			return *(Label*)children[0].pointer;
		}

		Label& LabelRight() {
			assert(children.len == 5);
			assert(children[3]->typeId == Label::cTypeId);
			return *(Label*)children[3].pointer;
		}

		void DragEnd() {
			if (!blockMoving) return;
			blockMoving = false;
			if (valueBak != value) {
				onChanged(value);
				valueBak = value;
			}
		}

		// todo: enable disable

		virtual int32_t OnMouseDown(int32_t btnId_) override {
			if (btnId_ != GLFW_MOUSE_BUTTON_LEFT) return 0;
			auto mx = GameBase::instance->mousePos.x;
			auto x1 = worldMinXY.x + widthTxtLeft * worldScale.x;
			auto x2 = worldMinXY.x + (widthTxtLeft + widthBar) * worldScale.x;
			assert(worldMaxXY.x > x1);
			assert(worldMaxXY.x > x2);
			if (mx <= x1) {
				value = 0;
			}
			else if (mx >= x2) {
				value = 1;
			}
			else {
				value = (mx - x1) / (x2 - x1);
			}
			ApplyValue();
			blockMoving = true;
			return 1;
		}

		int32_t OnMouseMove() override {
			if (!isFocus) {
				SetFocus();
				GameBase::instance->delayFuncs.Emplace([w = WeakFromThis(this)] {
					auto p = w.TryGetPointer();
					if (!p) return 1;
					if (GameBase::instance->uiHandler.TryGetPointer() != p || !p->MousePosInArea()) {
						p->LostFocus();
						p->DragEnd();
						return 1;
					}
					if (!GameBase::instance->mouse[GLFW_MOUSE_BUTTON_LEFT]) {
						p->DragEnd();
					}
					return 0;
				});
			}
			if (blockMoving) {
				OnMouseDown(GLFW_MOUSE_BUTTON_LEFT);
			}
			return 1;
		}

	};
}
