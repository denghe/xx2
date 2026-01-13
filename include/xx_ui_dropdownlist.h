#pragma once
#include "xx_ui_label.h"
#include "xx_ui_button.h"

namespace xx {

	struct DropDownList;
	struct DropDownListItem : MouseEventHandlerNode {
		static constexpr int32_t cTypeId{ 16 };
		Weak<DropDownList> owner;
		int32_t idx{ -1 };
		bool highLight{};

		void Init(int32_t z_, Weak<DropDownList> owner_, int32_t idx_, bool highLight_);

		void SetHighLight(bool enable);

		virtual int32_t OnMouseDown(int32_t btnId_) override;
		virtual int32_t OnMouseMove() override;
		virtual int32_t Update() override;
	};


	struct DropDownList : Button {
		static constexpr int32_t cTypeId{ 15 };
		Weak<Label> label;
		TinyFrame icon, itemHead;
		XY totalSize{};
		Shared<Scale9Config> cfgBG;	// for popup
		List<std::u32string> items;
		int32_t selectedIndex{};
		std::function<void(int32_t)> onSelectedIndexChanged = [](int32_t idx) { printf("DropDownList selectedIndex = %d\n", idx); };

		// init step 1/2
		DropDownList& InitBegin(int32_t z_, XY position_, XY anchor_, XY fixedSize_
			, Shared<Scale9Config> cfgNormal_ = GameBase::instance->embed.cfg_s9bN
			, Shared<Scale9Config> cfgHighlight_ = GameBase::instance->embed.cfg_s9bH
			, Shared<Scale9Config> cfgBG_ = GameBase::instance->embed.cfg_s9bg
			, TinyFrame icon_ = GameBase::instance->embed.ui_dropdownlist_icon
			, TinyFrame itemHead_ = GameBase::instance->embed.ui_dropdownlist_head);
		// init step 2/2
		// need items.Add(...
		void InitEnd(int32_t selectedIndex_ = 0);

		void SetSelectedIndex(int32_t selectedIndex_);
		void PopList();
		void ItemCommit(int32_t idx_);
		void ApplyCfg() override;
	};

}
