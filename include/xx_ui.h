#pragma once
// included by xx_gamebase_font.h
#include "xx_node.h"

namespace xx {

	struct Scale9SpriteConfig {
		TinyFrame frame;
		XY texScale{ 1, 1 };
		UVRect center{};
		RGBA8 color{ xx::RGBA8{ 0x5f, 0x15, 0xd9, 0xff } };
		float borderScale{ 1.f };
		RGBA8 txtColor{ xx::RGBA8_White };
		XY txtPadding{ 20, 5 };
		XY txtPaddingRightBottom{ 20, 10 };
		float txtScale{ 1 };
		float iconPadding{ 5 };

		XX_INLINE XY GetCornerSize() const {
			return { float(frame.textureRect.w - center.w) * borderScale, float(frame.textureRect.h - center.h) * borderScale };
		}

		XX_INLINE XY GetCornerScaledSize() const {
			return GetCornerSize() * texScale;
		}
	};

	struct Scale9Sprite : Node {
		TinyFrame frame;
		UVRect center;
		RGBA8 color;
		XY texScale;
		float colorplus;

		Scale9Sprite& Init(int z_, XY const& position_, XY const& scale_, XY const& anchor_, XY const& size_, Scale9SpriteConfig const& cfg_, float colorplus_ = 1) {
			Node::Init(z_, position_, scale_, anchor_, size_);
			texScale = cfg_.texScale;
			frame = cfg_.frame;
			center = cfg_.center;
			color = cfg_.color;
			colorplus = colorplus_;
			return *this;
		}

		Scale9Sprite& Init(int z_, XY position_, XY anchor_, XY size_, Scale9SpriteConfig const& cfg_) {
			return Init(z_, position_, cfg_.borderScale, anchor_, size_ / cfg_.borderScale, cfg_);
		}

		virtual void Draw() override {
			// calc
			auto& r = frame.textureRect;

			uint16_t tx1 = r.x + 0;
			uint16_t tx2 = r.x + center.x;
			uint16_t tx3 = r.x + center.x + center.w;

			uint16_t ty1 = r.y + 0;
			uint16_t ty2 = r.y + center.y;
			uint16_t ty3 = r.y + center.y + center.h;

			uint16_t tw1 = center.x;
			uint16_t tw2 = center.w;
			uint16_t tw3 = r.w - (center.x + center.w);

			uint16_t th1 = center.y;
			uint16_t th2 = center.h;
			uint16_t th3 = r.h - (center.y + center.h);

			XY ts{ texScale * worldScale };

			float sx = float(worldSize.x - tw1 * ts.x - tw3 * ts.x) / tw2;
			float sy = float(worldSize.y - th1 * ts.y - th3 * ts.y) / th2;
#ifndef NDEBUG
			if (sx < 0 || sy < 0) {
				CoutN(" sx = ", sx, " sy = ", sy, " ts = ", ts);
				assert(false);
			}
#endif

			float px1 = 0;
			float px2 = tw1 * ts.x;
			float px3 = worldSize.x - tw3 * ts.x;

			float py1 = worldSize.y;
			float py2 = worldSize.y - (th1 * ts.y);
			float py3 = worldSize.y - (worldSize.y - th3 * ts.y);

			auto& basePos = worldMinXY;

			RGBA8 c = { color.r, color.g, color.b, (uint8_t)(color.a * alpha) };

			// draw
			auto qs = GameBase_shader::GetInstance()->Quad().Alloc(*frame.tex, 9);
			auto q = &qs[0];
			q->pos = basePos + XY{ px1, py1 };
			q->anchor = { 0, 1 };
			q->scale = ts;
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx1, ty1, tw1, th1 };

			q = &qs[1];
			q->pos = basePos + XY{ px2, py1 };
			q->anchor = { 0, 1 };
			q->scale = { sx, ts.y };
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx2, ty1, tw2, th1 };

			q = &qs[2];
			q->pos = basePos + XY{ px3, py1 };
			q->anchor = { 0, 1 };
			q->scale = ts;
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx3, ty1, tw3, th1 };

			q = &qs[3];
			q->pos = basePos + XY{ px1, py2 };
			q->anchor = { 0, 1 };
			q->scale = { ts.x, sy };
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx1, ty2, tw1, th2 };

			q = &qs[4];
			q->pos = basePos + XY{ px2, py2 };
			q->anchor = { 0, 1 };
			q->scale = { sx, sy };
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx2, ty2, tw2, th2 };

			q = &qs[5];
			q->pos = basePos + XY{ px3, py2 };
			q->anchor = { 0, 1 };
			q->scale = { ts.x, sy };
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx3, ty2, tw3, th2 };

			q = &qs[6];
			q->pos = basePos + XY{ px1, py3 };
			q->anchor = { 0, 1 };
			q->scale = ts;
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx1, ty3, tw1, th3 };

			q = &qs[7];
			q->pos = basePos + XY{ px2, py3 };
			q->anchor = { 0, 1 };
			q->scale = { sx, ts.y };
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx2, ty3, tw2, th3 };

			q = &qs[8];
			q->pos = basePos + XY{ px3, py3 };
			q->anchor = { 0, 1 };
			q->scale = ts;
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx3, ty3, tw3, th3 };
		}
	};

	struct Label : Node {
		List<FontFrame> fs;
		RGBA8 color;

		template<typename S>
		Label& Init(int z_, XY const& position_, XY const& scale_, XY const& anchor_, RGBA8 color_, S const& txt_) {
			Init(z_, position_, scale_, anchor_, color_);
			SetText(txt_);
			return *this;
		}

		Label& Init(int z_, XY const& position_, XY const& scale_, XY const& anchor_, RGBA8 color_) {
			z = z_;
			position = position_;
			anchor = anchor_;
			scale = scale_;
			color = color_;
			return *this;
		}

		void Resize(XY const& position_, XY const& scale_) {
			position = position_;
			scale = scale_;
			FillTrans();
		}

		// S : literal string u8/32string [view]
		template<typename S>
		void SetText(S const& txt_) {
			auto len = (int32_t)StrLen(txt_);
			if (!len) {
				fs.Clear();
				return;
			}
			fs.Resize(len);
			auto& fm = GameBase_font::GetInstance()->fontMaker;
			size = { 0, (float)fm.canvasSize.y };
			for (int i = 0; i < len; ++i) {
				fs[i] = fm[txt_[i]];
				size.x += fs[i].textureRect.w;
			}
			FillTrans();
		}

		void SetText() {
			fs.Clear();
		}

		virtual void Draw() override {
			if (fs.Empty()) return;
			auto& q = GameBase_shader::GetInstance()->Quad();
			auto basePos = worldMinXY;
			for (auto& f : fs) {
				q.Draw(f.texId, f.textureRect, basePos, 0, worldScale, 0, 1, color);
				basePos.x += f.textureRect.w * worldScale.x;
			}
		}
	};

	enum class ImageRadians : int32_t {
		Zero = 0,		// 0'
		PiDiv2 = 1,		// 90'
		Pi = 2,			// 180'
		NegPiDiv2 = -1,	// -90'
		NegPi = -2		// -180'
	};

	struct Image : Node {
		TinyFrame frame;
		XY fixedScale{};
		RGBA8 color{};
		float colorplus{};
		float radians{};

		Image& Init(int z_, XY position_, XY anchor_, XY fixedSize_, bool keepAspect_
			, TinyFrame frame_, ImageRadians radians_ = ImageRadians::Zero, RGBA8 color_ = RGBA8_White, float colorplus_ = 1) {
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_;
			frame = std::move(frame_);
			radians = float(M_PI) * 0.5f * (int32_t)radians_;
			color = color_;
			colorplus = colorplus_;

			if (keepAspect_) {
				auto s = fixedSize_.x / frame.textureRect.w;
				if (frame.textureRect.h * s > fixedSize_.y) {
					s = fixedSize_.y / frame.textureRect.h;
				}
				fixedScale = s;
			}
			else {
				fixedScale.x = fixedSize_.x / frame.textureRect.w;
				fixedScale.y = fixedSize_.y / frame.textureRect.h;
			}

			FillTrans();
			return *this;
		}

		void Draw() override {
			if (!frame.tex) return;
			GameBase_shader::GetInstance()->Quad().Draw(*frame.tex, frame.textureRect, worldMinXY, 0
				, worldScale * fixedScale, radians, colorplus, { color.r, color.g, color.b, (uint8_t)(color.a * alpha) });
		}
	};

	struct EventNode : Node {
		virtual void OnMouseDown() {};
		virtual void OnMouseMove() {};
		virtual void OnMouseUp() {};

		XX_INLINE bool MousePosInArea() const {		// virtual ?
			return PosInArea(GameBase::instance->mousePos);
		}

		//virtual void TransUpdate() override {
		//	auto& g = GameBase::instance->uiEventGrid;
		//	auto pos = g.max_2 + worldMinXY + worldSize * 0.5f;
		//	if (Calc::Intersects::BoxPoint(XY{}, g.max, pos)) {
		//		SGABAddOrUpdate(g, pos, worldSize);
		//	}
		//	else {
		//		SGABRemove();
		//	}
		//}

		//~MouseEventHandlerNode() {
		//	SGABRemove();
		//}

		static constexpr uint64_t cFocusBaseTypeId{ 0xFBFBFBFBFBFBFBFB };	// unique. need store into ud
		std::function<void()> onFocus = [] {};	// play sound?
		xx::Ref<Scale9SpriteConfig> cfgNormal, cfgHighlight;
		bool isFocus{}, alwaysHighlight{};
		virtual void SetFocus() { assert(!isFocus); isFocus = true; };
		virtual void LostFocus() { assert(isFocus); isFocus = false; };
		// todo: focus navigate? prev next?
		Scale9SpriteConfig& GetCfg() {
			return alwaysHighlight ? *cfgHighlight : (isFocus ? *cfgHighlight : *cfgNormal);
		}
	};
	/*
	struct FocusButton : EventNode {
		std::function<void()> onClicked = [] { CoutN("FocusButton clicked."); };

		FocusButton& Init(int z_, XY position_, XY anchor_, XY size_
			, xx::Ref<Scale9SpriteConfig> cfgNormal_, xx::Ref<Scale9SpriteConfig> cfgHighlight_) {
			z = z_;
			position = position_;
			anchor = anchor_;
			size = size_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			FillTrans();
			auto& cfg = GetCfg();
			MakeChildren<Scale9Sprite>()->Init(z + 1, 0, cfg.borderScale, {}, size / cfg.borderScale, cfg);
			return *this;
		}

		virtual void ApplyCfg() {
			assert(children.len);
			auto& cfg = GetCfg();
			auto bg = (Scale9Sprite*)children[0].pointer;
			bg->Init(z + 1, 0, cfg.borderScale, {}, size / cfg.borderScale, cfg);
		}

		void SetFocus() override {
			assert(!isFocus);
			isFocus = true;
			ApplyCfg();
			gEngine->mouseEventHandler = xx::WeakFromThis(this);
			onFocus();
			//xx::CoutN("SetFocus");
		}

		void LostFocus() override {
			assert(isFocus);
			isFocus = false;
			ApplyCfg();
			gEngine->mouseEventHandler.Reset();
			//xx::CoutN("LostFocus");
		}

		// todo: enable disable

		virtual void OnMouseDown() override {
			if (MousePosInArea()) {
				onClicked();
			}
		}

		virtual void OnMouseMove() override {
			if (MousePosInArea()) {
				if (isFocus) return;
				SetFocus();
			}
			else {
				if (!isFocus) return;
				LostFocus();
				gEngine->mouseEventHandler.Reset();
			}
		}

	};

	struct FocusLabelButton : FocusButton {
		XY fixedSize{};

		template<typename S1, typename S2 = char const*>
		FocusLabelButton& Init(int z_, XY position_, XY anchor_
			, xx::Ref<Scale9SpriteConfig> cfgNormal_, xx::Ref<Scale9SpriteConfig> cfgHighlight_
			, S1 const& txtLeft_ = {}, S2 const& txtRight_ = {}, XY fixedSize_ = {}) {
			assert(children.Empty());
			ud = cFocusBaseTypeId;
			isFocus = false;
			z = z_;
			position = position_;
			anchor = anchor_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			fixedSize = fixedSize_;

			auto& cfg = GetCfg();
			auto lblLeft = MakeChildren<Label>();
			lblLeft->Init(z + 1, { cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, cfg.txtScale, {}, cfg.txtColor, txtLeft_);
			if (fixedSize.x > 0) {
				size = fixedSize;
			}
			else {
				size = lblLeft->GetScaledSize() + cfg.txtPadding + cfg.txtPaddingRightBottom;
			}
			if (StrLen(txtRight_)) {
				MakeChildren<Label>()->Init(z + 1, { size.x - cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, cfg.txtScale, { 1, 0 }, cfg.txtColor, txtRight_);
			}
			MakeChildren<Scale9Sprite>()->Init(z, {}, cfg.borderScale, {}, size / cfg.borderScale, cfg);
			FillTransRecursive();
			return *this;
		}

		void ApplyCfg() override {
			assert(children.len == 2 || children.len == 3);		// lblLeft [, lblRight], bg
			auto& cfg = GetCfg();
			auto lblLeft = (Label*)children[0].pointer;
			lblLeft->Init(z + 1, { cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, cfg.txtScale, {}, cfg.txtColor);
			if (fixedSize.x > 0) {
				size = fixedSize;
			}
			else {
				size = lblLeft->GetScaledSize() + cfg.txtPadding + cfg.txtPaddingRightBottom;
			}
			Scale9Sprite* bg;
			if (children.len == 3) {
				((Label*)children[1].pointer)->Init(z + 1, { size.x - cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, cfg.txtScale, { 1, 0 }, cfg.txtColor);
				bg = (Scale9Sprite*)children[2].pointer;
			}
			else {
				bg = (Scale9Sprite*)children[1].pointer;
			}
			bg->Init(z, {}, cfg.borderScale, {}, size / cfg.borderScale, cfg);
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

	struct FocusImageButton : FocusButton {
		FocusImageButton& Init(int z_, XY position_, XY anchor_, XY fixedSize_, bool keepAspect_, XY spacing_
			, xx::Ref<Scale9SpriteConfig> cfgNormal_, xx::Ref<Scale9SpriteConfig> cfgHighlight_
			, xx::Ref<xx::Frame> frame_, ImageRadians radians_ = ImageRadians::Zero, RGBA8 color_ = RGBA8_White, float colorplus_ = 1) {
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_ + spacing_ * 2;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			FillTrans();
			MakeChildren<Image2>()->Init(z, spacing_, {}, fixedSize_, keepAspect_, std::move(frame_), radians_, color_, colorplus_);
			auto& cfg = GetCfg();
			MakeChildren<Scale9Sprite>()->Init(z + 1, 0, cfg.borderScale, {}, size / cfg.borderScale, cfg);
			return *this;
		}

		void ApplyCfg() override {
			assert(children.len == 2);
			auto& cfg = GetCfg();
			auto bg = (Scale9Sprite*)children[1].pointer;
			bg->Init(z + 1, 0, cfg.borderScale, {}, size / cfg.borderScale, cfg);
		}
	};

	struct FocusSlider : EventNode {
		xx::Ref<Scale9SpriteConfig> cfgBar, cfgBlock;
		float height{}, widthTxtLeft{}, widthBar{}, widthTxtRight{};
		double value{}, valueBak{};	// 0 ~ 1
		bool blockMoving{};

		std::function<void(double)> onChanged = [](double v) { CoutN("FocusSlider changed. v = ", v); };
		std::function<std::string(double)> valueToString = [](double v)->std::string {
			return xx::ToString(int32_t(v * 100));
			};

		// InitBegin + set value/ToSting + InitEnd
		template<typename S>
		FocusSlider& InitBegin(int z_, XY position_, XY anchor_
			, xx::Ref<Scale9SpriteConfig> cfgNormal_, xx::Ref<Scale9SpriteConfig> cfgHighlight_
			, xx::Ref<Scale9SpriteConfig> cfgBar_, xx::Ref<Scale9SpriteConfig> cfgBlock_
			, float height_, float widthTxtLeft_, float widthBar_, float widthTxtRight_
			, S const& txtLeft_)
		{
			assert(children.Empty());
			ud = cFocusBaseTypeId;
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
			MakeChildren<Label>()->Init(z + 1, { cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, cfg.txtScale, {}, cfg.txtColor, txtLeft_);
			return *this;
		}

		FocusSlider& InitEnd() {
			assert(value >= 0 && value <= 1);
			auto& cfg = isFocus ? *cfgHighlight : *cfgNormal;
			size = { widthTxtLeft + widthBar + widthTxtRight, height };
			// todo: bar, block

			auto barMinWidth = cfgBar->center.x * 2. * cfgBar->borderScale;
			XY barSize{ std::max(widthBar * value, barMinWidth), height * 0.25f };
			XY barPos{ widthTxtLeft, (height - barSize.y) * 0.5f };
			MakeChildren<Scale9Sprite>()->Init(z + 1, barPos, cfgBar->borderScale, {}, barSize / cfgBar->borderScale, *cfgBar);

			XY blockSize{ height * 0.6f, height * 0.6f };
			XY blockPos{ widthTxtLeft + widthBar * value - blockSize.x * 0.5f, (height - blockSize.y) * 0.5f };
			MakeChildren<Scale9Sprite>()->Init(z + 2, blockPos, cfgBlock->borderScale, {}, blockSize / cfgBlock->borderScale, *cfgBlock);

			auto txtRight = valueToString(value);
			MakeChildren<Label>()->Init(z + 1, { size.x - cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, cfg.txtScale, { 1, 0 }, cfg.txtColor, txtRight);
			MakeChildren<Scale9Sprite>()->Init(z, {}, cfg.borderScale, {}, size / cfg.borderScale, cfg);
			FillTransRecursive();
			return *this;
		}

		template<typename S>
		FocusSlider& Init(int z_, XY position_, XY anchor_
			, xx::Ref<Scale9SpriteConfig> cfgNormal_, xx::Ref<Scale9SpriteConfig> cfgHighlight_
			, xx::Ref<Scale9SpriteConfig> cfgBar_, xx::Ref<Scale9SpriteConfig> cfgBlock_
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
			((Scale9Sprite*)children[4].pointer)->Init(z, {}, cfg.borderScale, {}, size / cfg.borderScale, cfg);
			//FillTransRecursive();
		}

		FocusSlider& SetValue(double v) {
			value = valueBak = v;
			return *this;
		}

		void ApplyValue() {
			assert(value >= 0 && value <= 1);

			auto barMinWidth = cfgBar->center.x * 2. * cfgBar->borderScale;
			XY barSize{ std::max(widthBar * value, barMinWidth), height * 0.25f };
			XY barPos{ widthTxtLeft, (height - barSize.y) * 0.5f };
			auto bar = ((Scale9Sprite*)children[1].pointer);
			bar->Init(z + 1, barPos, cfgBar->borderScale, {}, barSize / cfgBar->borderScale, *cfgBar);

			XY blockSize{ height * 0.6f, height * 0.6f };
			XY blockPos{ widthTxtLeft + widthBar * value - blockSize.x * 0.5f, (height - blockSize.y) * 0.5f };
			auto block = ((Scale9Sprite*)children[2].pointer);
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
			gEngine->mouseEventHandler = xx::WeakFromThis(this);
			onFocus();
			//xx::CoutN("SetFocus");
		}

		void LostFocus() override {
			assert(isFocus);
			isFocus = false;
			ApplyCfg();
			gEngine->mouseEventHandler.Reset();
			//xx::CoutN("LostFocus");
		}

		// todo: enable disable

		void OnMouseDown() override {
			if (MousePosInArea()) {
				auto mx = gEngine->mouse.pos.x;
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
			}
		}

		void OnMouseMove() override {
			if (MousePosInArea()) {
				if (!isFocus) {
					SetFocus();
				}
				if (blockMoving) {
					OnMouseDown();
				}
			}
			else {
				if (!isFocus) return;
				LostFocus();
				if (blockMoving) {
					blockMoving = false;
					if (valueBak != value) {
						onChanged(value);
						valueBak = value;
					}
				}
				gEngine->mouseEventHandler.Reset();
			}
		}

		void OnMouseUp() override {
			blockMoving = false;
			if (valueBak != value) {
				onChanged(value);
				valueBak = value;
			}
		};

	};
	*/
}
