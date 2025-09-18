#pragma once
#include "game.h"
#include "equip.h"

namespace UI {

	struct DraggingItem;
	struct EquipBagCell;
	struct CellItem;

	// panel
	struct EquipBag : xx::Node {
		static constexpr int32_t cTypeId{ 103 };
		xx::Weak<Creature> owner;
		xx::Weak<Node> focusedCell;
		float infoPanelPopupTime{};
		xx::Weak<Node> infoPanel;
		xx::Weak<DraggingItem> draggingItem;

		void Init(int32_t z_, xx::Weak<Creature> owner_);
		int32_t Update() override;
	};

	// panel's children cells
	struct EquipBagCell : xx::Button {
		static constexpr int32_t cTypeId{ 104 };
		EquipLocations equipLocation{};
		xx::Shared<Equip>* equipPtr{};
		xx::Weak<CellItem> cellItem;

		EquipBagCell& Init(int32_t z_, XY position_, XY anchor_, XY size_);
		void FillContent(EquipLocations equipLocation_, xx::Shared<Equip>* equipPtr_);
	};

	// cell's children content
	struct CellItem : xx::Node {
		static constexpr int32_t cTypeId{ 105 };
		CellItem& Init(int32_t z_, XY position_, XY anchor_, XY size_);
		void Draw() override;
	};

	// parent == ui root
	struct DraggingItem : xx::Node {
		static constexpr int32_t cTypeId{ 106 };
		xx::Weak<EquipBagCell> cell;
		DraggingItem& Init(xx::Weak<EquipBagCell> cell_, int32_t z_, XY position_, XY anchor_, XY size_);
		void Draw() override;
	};
}
