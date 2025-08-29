#pragma once
#include "xx_ui_background.h"
#include "xx_ui_imagebutton.h"

namespace xx {

	struct DropDownList;
	struct DropDownListItem : MouseEventHandlerNode {
		static constexpr int32_t cTypeId{ 15 };
		Weak<DropDownList> owner;
		int32_t idx{ -1 };
		bool highLight{};

		void Init(int32_t z_, Weak<DropDownList> owner_, int32_t idx_, bool highLight_);

		void SetHighLight(bool enable);

		virtual int32_t OnMouseDown(int32_t btnId_) override;

		virtual int32_t OnMouseMove() override;
	};

	struct DropDownList : Button {
		static constexpr int32_t cTypeId{ 15 };
		Ref<Scale9Config> cfgBG;
		List<std::string> items;
		Weak<Label> lbl;
		Weak<Background> bg;
		Weak<Scale9> content;
		TinyFrame icon, itemHead;
		XY totalSize{};
		int32_t selectedIndex{};
		std::function<void(int32_t)> onSelectedIndexChanged = [](int32_t idx) { CoutN("DropDownList selectedIndex = ", idx); };

		// init step 1/2
		DropDownList& InitBegin(int32_t z_, XY position_, XY anchor_, XY fixedSize_
			, Ref<Scale9Config> cfgNormal_, Ref<Scale9Config> cfgHighlight_, Ref<Scale9Config> cfgBG_
			, TinyFrame icon_, TinyFrame itemHead_) {
			typeId = cTypeId;
			isFocus = false;
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			cfgBG = std::move(cfgBG_);
			icon = std::move(icon_);
			itemHead = std::move(itemHead_);
			FillTrans();
			return *this;
		}

		// init step 2/2
		// need items.Add(...
		void InitEnd(int32_t selectedIndex_) {
			selectedIndex = selectedIndex_;
			totalSize = { size.x, size.y * items.len };
			auto& cfg = GetCfg();

			auto lblLeft = MakeChildren<Label>();
			lblLeft->Init(z + 1, { cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, {}, cfg.txtScale, cfg.txtColor);
			lblLeft->SetText(items[selectedIndex]);
			lbl = lblLeft;
			
			auto imgSize = size.y * 0.8f;
			auto imgSpacing = (size.y - imgSize) * 0.5f;
			XY imgPos{ size.x - imgSpacing, imgSpacing };
			MakeChildren<Image>()->Init(z + 2, imgPos, { 1, 0 }, imgSize, true, icon);

			MakeChildren<Scale9>()->Init(z, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);

			onClicked = [w = WeakFromThis(this)] {
				assert(w);
				w->PopList();
			};
		}

		void PopList() {
			auto c = MakeChildren<Scale9>();
			c->Init(z + 1000, 0, {0, 1}, 1, totalSize, *cfgBG);
			content = c.ToWeak();

			for (int32_t i = 0; i < items.len; ++i) {
				c->MakeChildren<DropDownListItem>()->Init(z + 1000
					, WeakFromThis(this), i, false);
			}

			bg = MakeChildren<Background>();
			bg->Init(z + 999, content).onOutsideClicked = [this] {
				content->SwapRemoveFromParent();
				bg->SwapRemoveFromParent();
			};
		}

		void ItemCommit(int32_t idx_) {
			assert(content);
			assert(bg);
			assert(lbl);
			content->SwapRemoveFromParent();
			bg->SwapRemoveFromParent();
			if (selectedIndex != idx_) {
				selectedIndex = idx_;
				lbl->SetText(items[idx_]);
				onSelectedIndexChanged(idx_);
			}
		}

		void ApplyCfg() override {
			assert(children.len >= 3);
			auto& cfg = GetCfg();
			assert(children[2]->typeId == Scale9::cTypeId);
			auto bg = (Scale9*)children[2].pointer;
			bg->Init(z, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);
		}
	};


	inline void DropDownListItem::Init(int32_t z_, Weak<DropDownList> owner_, int32_t idx_, bool highLight_) {
		typeId = cTypeId;
		z = z_;
		owner = std::move(owner_);
		idx = idx_;
		highLight = highLight_;
		size = owner->size;
		position = { 0, owner->totalSize.y - size.y * idx_ };
		anchor = { 0, 1 };
		FillTrans();

		auto imgSize = size.y * 0.8f;
		auto imgSpacing = (size.y - imgSize) * 0.5f;
		auto imgColor = idx_ == owner->selectedIndex ? RGBA8_Green : RGBA8_White;
		MakeChildren<Image>()->Init(z, imgSpacing, { 0, 0 }, imgSize, true, owner->itemHead, {}, imgColor);

		auto& cfg = *owner->cfgNormal;
		auto lblLeft = MakeChildren<Label>();
		XY lblPos{ imgSpacing + imgSize + imgSpacing, cfg.txtPaddingRightBottom.y };
		lblLeft->Init(z, lblPos, {}, cfg.txtScale, cfg.txtColor);
		lblLeft->SetText(owner->items[idx_]);
	}

	inline void DropDownListItem::SetHighLight(bool enable) {
		if (highLight == enable) return;
		highLight = enable;
		assert(children[1]->typeId == Label::cTypeId);
		auto lbl = (Label*)children[1].pointer;
		lbl->color = enable ? RGBA8{ 200, 100, 100, 255 } : RGBA8_White;
	}

	inline int32_t DropDownListItem::OnMouseDown(int32_t btnId_) {
		assert(owner);
		owner->ItemCommit(idx);
		return 1;
	}

	inline int32_t DropDownListItem::OnMouseMove() {
		if (!highLight) {
			SetHighLight(true);
			GameBase::instance->delayFuncs.Emplace([w = WeakFromThis(this)] {
				auto p = w.TryGetPointer();
				if (!p) return 1;
				if (!p->MousePosInArea()) {
					p->SetHighLight(false);
					return 1;
				}
				return 0;
				});
		}
		return 1;
	}

}
