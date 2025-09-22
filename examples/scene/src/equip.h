#pragma once
#include "game.h"
struct Scene_Play;
struct Creature;

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
	__EQUIPED_MAX__ = Boots,
	/*********************/
	// in the bag
	/*********************/
	Currency,
	Materials,
	Consumables,
	// ...
	__MAX__
};

static constexpr i18n::Strs EquipLocationsStrs[] = {
		i18n::Strs::gameName,	// map to Unknown
		i18n::Strs::helm,
		i18n::Strs::ring,
		i18n::Strs::amulet,
		i18n::Strs::armor,
		i18n::Strs::weapon1,
		i18n::Strs::weapon2,
		i18n::Strs::boots,
		i18n::Strs::currency,
		i18n::Strs::materials,
		i18n::Strs::consumables,
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

static constexpr i18n::Strs EquipQualitiesStrs[] = {
	i18n::Strs::normal,
	i18n::Strs::excellent,
	i18n::Strs::rare,
	i18n::Strs::epic,
	i18n::Strs::legendary,
	i18n::Strs::ancient,
	i18n::Strs::myth,
};

static constexpr xx::RGBA8 EquipQualityColors[] = {
	{127, 127, 127, 255},
	{0, 255, 0, 255},
	{0, 127, 255, 255},
	{255, 0, 255, 255},
	{255, 127, 0, 255},
	{255, 0, 0, 255},
	{12, 12, 12, 255},
};

enum class EquipTargets : uint8_t {
	Creature,
	Scene
	// ...
};

struct EquipConfigProp {
	EquipTargets target{};
	uint8_t index{};
	xx::FromTo<float> value{};
};

struct EquipConfig {
	int32_t typeId{};
	EquipLocations location{};
	EquipQualities quality{};
	i18n::Strs name{};
	xx::TinyFrame icon;
	xx::List<EquipConfigProp> props;
};

struct EquipProp {
	EquipTargets target{};
	uint8_t index{};
	float value{};
};

// instance
struct Equip {
	static constexpr int32_t cTypeId{};
	xx::Ref<EquipConfig> cfg;
	Scene_Play* scene{};
	Creature* owner{};
	xx::List<EquipProp> props;	// cfg's props calc result
	// ...

	void EquipInit(Creature* owner_, xx::Ref<EquipConfig> cfg_);
	void Combine(Equip& tar);	// attach tar's props to this

	xx::Shared<xx::Node> GenInfoPanel(Equip* cmp = {});	// parent == Scene_Play::ui
	virtual void Update() {}
	virtual void Cast() {};
	virtual void Draw(XY pos_, XY anchor_, XY size_, float colorplus_);

	template<typename T> requires std::is_base_of_v<Equip, T>
	static xx::Shared<T> Make(Creature* owner_) {
		auto r = xx::MakeShared<T>();
		r->EquipInit(owner_, T::_cfg);
		return r;
	}

	static void InitAllCfgs();	// Equip_xxxxx::_cfg = .......
};

// todo: quantity?
// todo: equip config manager ?
// todo: drop rate table ?

struct Equip_amulet_1 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_armor_1 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_armor_2 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_armor_3 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_boots_1 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_consumable_1 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_currency_1 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_currency_2 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_helm_1 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_material_1 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_ring_1 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_ring_2 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_ring_3 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_ring_4 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_weapon1_1 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_weapon1_2 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_weapon1_3 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

struct Equip_weapon2_1 : Equip {
	static constexpr int32_t cTypeId{ __LINE__ };
	inline static xx::Ref<EquipConfig> _cfg;
};

// ...
