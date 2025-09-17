#pragma once
#include "game.h"
#include "equip.h"

namespace UI {

	// panel
	struct EquipBag : xx::Node {
		static constexpr int32_t cTypeId{ 103 };
		xx::Weak<Creature> owner;
		xx::Weak<Node> focusedCell;
		float infoPanelPopupTime{};
		xx::Weak<Node> infoPanel;

		void Init(int32_t z_, xx::Weak<Creature> owner_);
		int32_t Update() override;
	};

	// panel's children cells
	struct EquipBagCell : xx::Button {
		static constexpr int32_t cTypeId{ 104 };
		EquipLocations equipLocation{};
		xx::Shared<Equip>* equipPtr{};

		EquipBagCell& Init(int32_t z_, XY position_, XY anchor_, XY size_);
		void operator()(EquipLocations equipLocation_, xx::Shared<Equip>* equipPtr_);
	};

	// cell's children content
	struct EquipItem : xx::Node {
		static constexpr int32_t cTypeId{ 105 };
		EquipItem& Init(int32_t z_, XY position_, XY anchor_, XY size_);
		int32_t Update() override;
		void Draw() override;
	};
}
