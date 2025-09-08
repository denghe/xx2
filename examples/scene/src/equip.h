#pragma once
#include "game.h"
struct Scene_Play;
struct Player;

enum class EquipLocations : uint8_t {
	Unknown,
	/*********************/
	// equiped
	/*********************/
	Helm,
	Ring,
	Amulet,
	Armor,
	Weapon1,
	Weapon2,
	Boots,
	// ...
	/*********************/
	// in the bag
	/*********************/
	Currency,
	Materials,
	Consumables,
	// ...
};

enum class EquipQualities : uint8_t {
	Normal,			// white
	Excellent,		// green
	Rare,			// blue
	Epic,			// purple
	Legendary,		// golden
	Ancient,		// red
	Myth			// iridescent
	// ...
};

enum class EquipTargets : uint8_t {
	Player,
	Scene
	// ...
};

struct EquipConfigProp {
	EquipTargets target{};
	uint8_t index{};
	xx::FromTo<float> value{};
};

struct EquipConfig {
	EquipLocations location;
	EquipQualities quality;
	xx::List<EquipConfigProp> props;
};

struct EquipProp {
	EquipTargets target{};
	uint8_t index{};
	float value{};
};

// instance
struct Equip {
	xx::Ref<EquipConfig> cfg;
	Scene_Play* scene{};
	Player* owner{};
	EquipLocations location;
	EquipQualities quality;
	xx::List<EquipProp> props;
	// ... more flags
	void Init(Player* owner_, xx::Ref<EquipConfig> cfg_);
	void Combine(Equip& tar);	// attach tar's props to this
	xx::Shared<xx::Node> GetInfoPanel();
	virtual void Update() {}
	virtual void Cast() {};
	virtual void Draw(XY pos, XY scale) {}
};

// todo: equip config manager ?
// todo: drop rate table ?
