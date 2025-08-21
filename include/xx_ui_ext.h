#pragma once
#include "xx_ui_base.h"

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
			assert(isFocus);
			if (GameBase::instance->uiHandler.TryGetPointer() == this) {
				GameBase::instance->uiHandler.Reset();
			}
			isFocus = false;
			ApplyCfg();
			//CoutN("LostFocus");
		}

		// todo: enable disable

		virtual int32_t OnMouseDown() override {
			onClicked();
			return 1;
		}

		virtual int32_t OnMouseMove() override {
			if (!isFocus) {
				SetFocus();
				GameBase::instance->delayFuncs.Emplace([w = WeakFromThis(this)] {
					if (!w) return 1;
					auto p = w.TryGetPointer();
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

	struct LabelButton : Button {
		static constexpr int32_t cTypeId{ 11 };

		XY fixedSize{};

		template<typename S1, typename S2 = char const*>
		LabelButton& Init(int z_, XY position_, XY anchor_
			, Ref<Scale9Config> cfgNormal_, Ref<Scale9Config> cfgHighlight_
			, S1 const& txtLeft_ = {}, S2 const& txtRight_ = {}, XY fixedSize_ = {}) {
			assert(children.Empty());
			typeId = cTypeId;
			isFocus = false;
			z = z_;
			position = position_;
			anchor = anchor_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			fixedSize = fixedSize_;

			auto& cfg = GetCfg();
			auto lblLeft = MakeChildren<Label>();
			lblLeft->Init(z + 1, { cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, {}, cfg.txtScale, cfg.txtColor).SetText(txtLeft_);
			if (fixedSize.x > 0) {
				size = fixedSize;
			}
			else {
				size = lblLeft->GetScaledSize() + cfg.txtPadding + cfg.txtPaddingRightBottom;
			}
			if (StrLen(txtRight_)) {
				MakeChildren<Label>()->Init(z + 1, { size.x - cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, { 1, 0 }, cfg.txtScale, cfg.txtColor).SetText(txtRight_);
			}
			MakeChildren<Scale9>()->Init(z, {}, {}, cfg.borderScale, size / cfg.borderScale, cfg);
			FillTransRecursive();
			return *this;
		}

		void ApplyCfg() override {
			assert(children.len == 2 || children.len == 3);		// lblLeft [, lblRight], bg
			auto& cfg = GetCfg();
			//CoutN(cfg.frame.tex->FileName());
			auto lblLeft = (Label*)children[0].pointer;
			lblLeft->Init(z + 1, { cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, {}, cfg.txtScale, cfg.txtColor);
			if (fixedSize.x > 0) {
				size = fixedSize;
			}
			else {
				size = lblLeft->GetScaledSize() + cfg.txtPadding + cfg.txtPaddingRightBottom;
			}
			Scale9* bg;
			if (children.len == 3) {
				((Label*)children[1].pointer)->Init(z + 1, { size.x - cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, { 1, 0 }, cfg.txtScale, cfg.txtColor);
				bg = (Scale9*)children[2].pointer;
			}
			else {
				bg = (Scale9*)children[1].pointer;
			}
			bg->Init(z, {}, {}, cfg.borderScale, size / cfg.borderScale, cfg);
			FillTransRecursive();
		}

		Label& LabelLeft() {
			assert(children.len == 2 || children.len == 3);
			return *(Label*)children[0].pointer;
		}

		Label& LabelRight() {
			assert(children.len == 3);
			return *(Label*)children[1].pointer;
		}
	};

	struct ImageButton : Button {
		static constexpr int32_t cTypeId{ 12 };

		ImageButton& Init(int z_, XY position_, XY anchor_, XY fixedSize_, bool keepAspect_, XY spacing_
			, Ref<Scale9Config> cfgNormal_, Ref<Scale9Config> cfgHighlight_
			, TinyFrame frame_, ImageRadians radians_ = ImageRadians::Zero, RGBA8 color_ = RGBA8_White, float colorplus_ = 1) {
			typeId = cTypeId;
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_ + spacing_ * 2;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			FillTrans();
			MakeChildren<Image>()->Init(z, spacing_, {}, fixedSize_, keepAspect_, std::move(frame_), radians_, color_, colorplus_);
			auto& cfg = GetCfg();
			MakeChildren<Scale9>()->Init(z + 1, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);
			return *this;
		}

		void ApplyCfg() override {
			assert(children.len == 2);
			auto& cfg = GetCfg();
			auto bg = (Scale9*)children[1].pointer;
			bg->Init(z + 1, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);
		}
	};

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
		Slider& InitBegin(int z_, XY position_, XY anchor_
			, Ref<Scale9Config> cfgNormal_, Ref<Scale9Config> cfgHighlight_
			, Ref<Scale9Config> cfgBar_, Ref<Scale9Config> cfgBlock_
			, float height_, float widthTxtLeft_, float widthBar_, float widthTxtRight_
			, S const& txtLeft_)
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

			auto& cfg = isFocus ? *cfgHighlight : *cfgNormal;
			MakeChildren<Label>()->Init(z + 1, { cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, {}, cfg.txtScale, cfg.txtColor, txtLeft_);
			return *this;
		}

		Slider& InitEnd() {
			assert(value >= 0 && value <= 1);
			auto& cfg = isFocus ? *cfgHighlight : *cfgNormal;
			size = { widthTxtLeft + widthBar + widthTxtRight, height };
			// todo: bar, block

			auto barMinWidth = cfgBar->center.x * 2. * cfgBar->borderScale;
			XY barSize{ std::max(widthBar * value, barMinWidth), height * 0.25f };
			XY barPos{ widthTxtLeft, (height - barSize.y) * 0.5f };
			MakeChildren<Scale9>()->Init(z + 1, barPos, {}, cfgBar->borderScale, barSize / cfgBar->borderScale, *cfgBar);

			XY blockSize{ height * 0.6f, height * 0.6f };
			XY blockPos{ widthTxtLeft + widthBar * value - blockSize.x * 0.5f, (height - blockSize.y) * 0.5f };
			MakeChildren<Scale9>()->Init(z + 2, blockPos, {}, cfgBlock->borderScale, blockSize / cfgBlock->borderScale, *cfgBlock);

			auto txtRight = valueToString(value);
			MakeChildren<Label>()->Init(z + 1, { size.x - cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, cfg.txtScale, { 1, 0 }, cfg.txtColor).SetText(txtRight);
			MakeChildren<Scale9>()->Init(z, {}, {}, cfg.borderScale, size / cfg.borderScale, cfg);
			FillTransRecursive();
			return *this;
		}

		template<typename S>
		Slider& Init(int z_, XY position_, XY anchor_
			, Ref<Scale9Config> cfgNormal_, Ref<Scale9Config> cfgHighlight_
			, Ref<Scale9Config> cfgBar_, Ref<Scale9Config> cfgBlock_
			, float height_, float widthTxtLeft_, float widthBar_, float widthTxtRight_
			, S const& txtLeft_, double value_) {
			InitBegin(z_, position_, anchor_
				, std::move(cfgNormal_), std::move(cfgHighlight_), std::move(cfgBar_), std::move(cfgBlock_)
				, height_, widthTxtLeft_, widthBar_, widthTxtRight_, txtLeft_);
			SetValue(value_);
			InitEnd();
			return *this;
		}

		void ApplyCfg() {
			assert(children.len == 5);		// lblLeft, bar, block, lblRight, bg
			auto& cfg = isFocus ? *cfgHighlight : *cfgNormal;
			//auto lblLeft = (Label*)children[0].pointer;
			//lblLeft->Init(z + 1, { cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, cfg.txtScale, {}, cfg.txtColor);
			((Scale9*)children[4].pointer)->Init(z, {}, {}, cfg.borderScale, size / cfg.borderScale, cfg);
			//FillTransRecursive();
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
			return *(Label*)children[0].pointer;
		}

		Label& LabelRight() {
			assert(children.len == 5);
			return *(Label*)children[3].pointer;
		}

		void SetFocus() override {
			assert(!isFocus);
			isFocus = true;
			ApplyCfg();
			GameBase::instance->uiHandler = WeakFromThis(this);
			onFocus();
			//CoutN("SetFocus");
		}

		void LostFocus() override {
			assert(isFocus);
			isFocus = false;
			ApplyCfg();
			GameBase::instance->uiHandler.Reset();
			//CoutN("LostFocus");
		}

		// todo: enable disable

		int32_t OnMouseDown() override {
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
			return true;
		}

		int32_t OnMouseMove() override {
			if (!isFocus) {
				SetFocus();
			}
			if (blockMoving) {
				OnMouseDown();
			}
			//if (!isFocus) return;
			//LostFocus();
			//if (blockMoving) {
			//	blockMoving = false;
			//	if (valueBak != value) {
			//		onChanged(value);
			//		valueBak = value;
			//	}
			//}
			//GameBase::instance->uiHandler.Reset();
			return true;
		}

		void OnMouseUp() override {
			blockMoving = false;
			if (valueBak != value) {
				onChanged(value);
				valueBak = value;
			}
		};

	};
}
