#pragma once
#include "xx_ui_dropdownlist.h"
#include "xx_ui_label.h"
#include "xx_ui_image.h"
#include "xx_ui_background.h"
#include "xx_gamebase.h"

namespace xx {

	// init step 1/2
	DropDownList& DropDownList::InitBegin(int32_t z_, XY position_, XY anchor_, XY fixedSize_
		, Shared<Scale9Config> cfgNormal_
		, Shared<Scale9Config> cfgHighlight_
		, Shared<Scale9Config> cfgBG_
		, TinyFrame icon_
		, TinyFrame itemHead_) {
		assert(typeId == cTypeId);
		focused = false;
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
	void DropDownList::InitEnd(int32_t selectedIndex_) {
		selectedIndex = selectedIndex_;
		totalSize = { size.x, size.y * items.len };
		auto& cfg = GetCfg();
		auto fontSize = size.y - cfg->paddings.TopBottom();
		label = Make<Label>();
		label->Init(z + 1, cfg->paddings.LeftBottom(), 0, fontSize)(items[selectedIndex]);
			
		XY imgSize{ size.y - cfg->paddings.TopBottom() };
		XY imgPos{ size.x - cfg->paddings.right, cfg->paddings.bottom };
		Make<Image>()->Init(z + 2, imgPos, {1, 0}, icon, imgSize);

		Make<Scale9>()->Init(z, 0, 0, size, cfg);

		onClicked = [w = WeakFromThis(this)] {
			assert(w);
			w->PopList();
		};
	}

	void DropDownList::SetSelectedIndex(int32_t selectedIndex_) {
		selectedIndex = selectedIndex_;
		label->SetText(items[selectedIndex]);
	}

	void DropDownList::PopList() {
		auto itemsBorder = Make<Scale9>();
		itemsBorder->Init(z + 1000, 0, { 0, 1 }, totalSize, cfgBG);
		itemsBorder->inParentArea = false;

		auto itemsContent = Make<Node>();
		itemsContent->Init(z + 1001, 0, { 0, 1 }, 1, totalSize);
		itemsContent->inParentArea = false;

		for (int32_t i = 0; i < items.len; ++i) {
			itemsContent->Make<DropDownListItem>()->Init(z + 1001
				, WeakFromThis(this), i, false);
		}

		auto itemsBG = Make<Background>();
		itemsBG->Init(z + 999, itemsContent).onOutsideClicked = [this] {
			children.Resize(3);
		};
	}

	void DropDownList::ItemCommit(int32_t idx_) {
		At<Background>(5).onOutsideClicked();	// unsafe
		if (selectedIndex != idx_) {
			selectedIndex = idx_;
			label->SetText(items[idx_]);
			onSelectedIndexChanged(idx_);
		}
	}

	void DropDownList::ApplyCfg() {
		At<Scale9>(2).Init(z, 0, 0, size, GetCfg());
	}





	void DropDownListItem::Init(int32_t z_, Weak<DropDownList> owner_, int32_t idx_, bool highLight_) {
		assert(typeId == cTypeId);
		z = z_;
		owner = std::move(owner_);
		idx = idx_;
		highLight = highLight_;
		size = owner->size;
		position = { 0, owner->totalSize.y - size.y * idx_ };
		anchor = { 0, 1 };
		FillTrans();

		auto& cfg = owner->GetCfg();
		auto imgSize = XY{ size.y - cfg->paddings.TopBottom() };
		auto imgColor = idx_ == owner->selectedIndex ? RGBA8_Green : RGBA8_White;
		Make<Image>()->Init(z, cfg->paddings.LeftBottom(), 0, owner->itemHead, imgSize, true, {}, imgColor);

		Make<Label>()->Init(z, cfg->paddings.LeftBottom() + XY{ imgSize.x, 0 }, 0, size.y - cfg->paddings.TopBottom())(owner->items[idx_]);
	}

	void DropDownListItem::SetHighLight(bool b) {
		if (highLight == b) return;
		highLight = b;
		auto& label = At<Label>(1);
		if (b) label.color = RGBA8{ 200, 100, 100, 255 };
		else label.color = RGBA8_White;
	}

	int32_t DropDownListItem::OnMouseDown(int32_t btnId_) {
		assert(owner);
		owner->ItemCommit(idx);
		return 1;
	}

	int32_t DropDownListItem::OnMouseMove() {
		if (!highLight) {
			SetHighLight(true);
			TryRegisterAutoUpdate();
		}
		return 1;
	}

	int32_t DropDownListItem::Update() {
		if (!MousePosInArea()) {
			SetHighLight(false);
			return 1;
		}
		return 0;
	}
}
