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
	i18n::Strs::gameName,	// Unknown
	i18n::Strs::EquipLocations_Helm,
	i18n::Strs::EquipLocations_Ring,
	i18n::Strs::EquipLocations_Amulet,
	i18n::Strs::EquipLocations_Armor,
	i18n::Strs::EquipLocations_Weapon1,
	i18n::Strs::EquipLocations_Weapon2,
	i18n::Strs::EquipLocations_Boots,
	i18n::Strs::EquipLocations_Currency,
	i18n::Strs::EquipLocations_Materials,
	i18n::Strs::EquipLocations_Consumables,
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
	i18n::Strs::EquipQualities_Normal,
	i18n::Strs::EquipQualities_Excellent,
	i18n::Strs::EquipQualities_Rare,
	i18n::Strs::EquipQualities_Epic,
	i18n::Strs::EquipQualities_Legendary,
	i18n::Strs::EquipQualities_Ancient,
	i18n::Strs::EquipQualities_Myth,
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
	uint32_t typeId{};
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
	static constexpr uint32_t cTypeId{};	// map to cfgs / makers[ index ]
	xx::Ref<EquipConfig> cfg;
	Scene_Play* scene{};
	Creature* owner{};
	xx::List<EquipProp> props;	// cfg's props calc result
	// todo: quantity?
	// ...

	void Init(Creature* owner_, xx::Ref<EquipConfig> cfg_);
	void Combine(Equip& tar);	// attach tar's props to this

	xx::Shared<xx::Node> GenInfoPanel(Equip* cmp = {});	// parent == Scene_Play::ui
	virtual void Update() {}
	virtual void Cast() {};
	virtual void Draw(XY pos_, XY anchor_, XY size_, float colorplus_);

	// for save data
	void WriteTo(xx::Data& d_);
	static int ReadFrom(xx::Data_r& dr_, Creature* owner_, xx::Shared<Equip>& tar_);

	// cfgs & makers's index == Equip_xxxx::cTypeId
	inline static xx::List<xx::Ref<EquipConfig>> cfgs;
	static void InitCfgs();

	// cfgs & makers's index == Equip_xxxx::cTypeId
	inline static xx::List<std::function<xx::Shared<Equip>()>> makers;
	static xx::Shared<Equip> Make(Creature* owner_, uint32_t typeId_);

	// makers filler func
	template<typename T> requires std::derived_from<T, Equip>
	static xx::Ref<EquipConfig>& MakeMakerAndCfg() {
		assert(cfgs.len == T::cTypeId);
		assert(cfgs.len == makers.len);
		makers.Emplace([] { return xx::MakeShared<T>(); });
		auto& c = cfgs.Emplace().Emplace();
		c->typeId = T::cTypeId;
		return c;
	}
};

struct Equip_amulet_1 : Equip {
	static constexpr uint32_t cTypeId{1};
};
struct Equip_armor_1 : Equip {
	static constexpr uint32_t cTypeId{2};
};
struct Equip_armor_2 : Equip {
	static constexpr uint32_t cTypeId{3};
};
struct Equip_armor_3 : Equip {
	static constexpr uint32_t cTypeId{4};
};
struct Equip_boots_1 : Equip {
	static constexpr uint32_t cTypeId{5};
};
struct Equip_consumable_1 : Equip {
	static constexpr uint32_t cTypeId{6};
};
struct Equip_currency_1 : Equip {
	static constexpr uint32_t cTypeId{7};
};
struct Equip_currency_2 : Equip {
	static constexpr uint32_t cTypeId{8};
};
struct Equip_helm_1 : Equip {
	static constexpr uint32_t cTypeId{9};
};
struct Equip_material_1 : Equip {
	static constexpr uint32_t cTypeId{10};
};
struct Equip_ring_1 : Equip {
	static constexpr uint32_t cTypeId{11};
};
struct Equip_ring_2 : Equip {
	static constexpr uint32_t cTypeId{12};
};
struct Equip_ring_3 : Equip {
	static constexpr uint32_t cTypeId{13};
};
struct Equip_ring_4 : Equip {
	static constexpr uint32_t cTypeId{14};
};
struct Equip_weapon1_1 : Equip {
	static constexpr uint32_t cTypeId{15};
};
struct Equip_weapon1_2 : Equip {
	static constexpr uint32_t cTypeId{16};
};
struct Equip_weapon1_3 : Equip {
	static constexpr uint32_t cTypeId{17};
};
struct Equip_weapon2_1 : Equip {
	static constexpr uint32_t cTypeId{18};
};

// ...

// todo: drop rate table ?
