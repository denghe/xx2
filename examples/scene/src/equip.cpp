#include "pch.h"
#include "scene_play.h"
#include "equip.h"

void Equip::EquipInit(Creature* owner_, xx::Ref<EquipConfig> cfg_) {
	owner = owner_;
	scene = owner_->scene;
	cfg = std::move(cfg_);
	auto& cps = cfg->props;
	if (!cps.len) return;
	props.Reserve(cps.len);
	for (int32_t i = 0; i < cps.len; ++i) {
		auto& p = cps[i];
		float v;
		if (p.value.from == p.value.to) v = p.value.from;
		else v = gg.rnd.Next<float>(p.value.from, p.value.to);
		props.Emplace(EquipProp{ p.target, p.index, v });
	}
}

void Equip::Combine(Equip& tar) {
	assert(cfg == tar.cfg);
	assert(props.len == tar.props.len);
	for (int32_t i = 0; i < props.len; ++i) {
		auto& p1 = props[i];
		auto& p2 = tar.props[i];
		assert(p1.target == p2.target && p1.index == p2.index);
		p1.value += p2.value;
	}
}

void Equip::Draw(XY pos_, XY anchor_, XY size_, float colorplus_) {
	auto& f = cfg->icon;
	gg.Quad().Draw(f, f, pos_, anchor_, { size_.x / f.textureRect.w, size_.y / f.textureRect.h }, 0, colorplus_);
}

xx::Shared<xx::Node> Equip::GenInfoPanel(Equip* cmp_) {
	/*
	+-------------------+
	| [icon] name       |
	| [icon] location   |
	| props       value |
	| props       value |
	| props       value |
	| ...         ...   |
	|                   |
	| $         quality |
	+-------------------+
	*/
	auto& ui = gg.scene.Cast<Scene_Play>()->ui;	// root
	// todo: handle cmp_

	// container
	auto& r = ui->Make<xx::Node>();
	xx::Layouter L;
	L.InitBegin(r, ui->z + 100, 0, { 0, 1 }, 600);
	L.Image(cfg->icon, 96);
	L.EndLine(false, 96 + 10);
	L.Text(gg.lang(cfg->name), 32, 48);
	L.EndLine(true, 96 + 10);
	L.Text(gg.lang(EquipLocationsStrs[(int32_t)cfg->location]), 32, 48);
	L.EndLine();
	assert(cfg->props.len == props.len);
	for (int32_t i = 0; i < props.len; ++i) {
		auto& o = props[i];
		auto& c = cfg->props[i];
		L.Text(gg.lang(CreaturePropertiesStrs[o.index]), 32, 48);
		L.EndLine(false);
		L.Text(xx::ToString(o.value, " (", c.value.from, " ~ ", c.value.to, ")"), 32, 48);
		L.EndLine(xx::HAligns::Right);
	}
	// ...
	// todo: desc. translate to rich node?
	// price
	// quality
	L.InitEnd();

	// bg
	r->Make<xx::Scale9>()->Init(r->z - 1, -10, 0, r->size + 20).cfg.color = EquipQualityColors[(int32_t)cfg->quality];
	return r;
}

void Equip::InitAllCfgs() {
	{
		auto& c = Equip_amulet_1::_cfg.Emplace();
		c->name = i18n::Strs::Equip_amulet_1;
		c->icon = gg.res.amulet_1;
		c->location = EquipLocations::Amulet;
		c->quality = EquipQualities::Normal;
		c->props.Emplace(EquipConfigProp{ EquipTargets::Creature, 0, {10, 10} });
		c->props.Emplace(EquipConfigProp{ EquipTargets::Creature, 25, {3, 5} });
	}
	{
		auto& c = Equip_armor_1::_cfg.Emplace();
		c->name = i18n::Strs::Equip_armor_1;
		c->icon = gg.res.armor_1;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_armor_2::_cfg.Emplace();
		c->name = i18n::Strs::Equip_armor_2;
		c->icon = gg.res.armor_2;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Excellent;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_armor_3::_cfg.Emplace();
		c->name = i18n::Strs::Equip_armor_3;
		c->icon = gg.res.armor_3;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Rare;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_boots_1::_cfg.Emplace();
		c->name = i18n::Strs::Equip_boots_1;
		c->icon = gg.res.boots_1;
		c->location = EquipLocations::Boots;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_consumable_1::_cfg.Emplace();
		c->name = i18n::Strs::Equip_consumable_1;
		c->icon = gg.res.consumable_1;
		c->location = EquipLocations::Consumables;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_currency_1::_cfg.Emplace();
		c->name = i18n::Strs::Equip_currency_1;
		c->icon = gg.res.currency_1;
		c->location = EquipLocations::Currency;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_currency_2::_cfg.Emplace();
		c->name = i18n::Strs::Equip_currency_2;
		c->icon = gg.res.currency_2;
		c->location = EquipLocations::Currency;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_helm_1::_cfg.Emplace();
		c->name = i18n::Strs::Equip_helm_1;
		c->icon = gg.res.helm_1;
		c->location = EquipLocations::Helm;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_material_1::_cfg.Emplace();
		c->name = i18n::Strs::Equip_material_1;
		c->icon = gg.res.material_1;
		c->location = EquipLocations::Materials;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_1::_cfg.Emplace();
		c->name = i18n::Strs::Equip_ring_1;
		c->icon = gg.res.ring_1;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Rare;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_2::_cfg.Emplace();
		c->name = i18n::Strs::Equip_ring_2;
		c->icon = gg.res.ring_2;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Epic;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_3::_cfg.Emplace();
		c->name = i18n::Strs::Equip_ring_3;
		c->icon = gg.res.ring_3;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Legendary;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_4::_cfg.Emplace();
		c->name = i18n::Strs::Equip_ring_4;
		c->icon = gg.res.ring_4;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Ancient;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon1_1::_cfg.Emplace();
		c->name = i18n::Strs::Equip_weapon1_1;
		c->icon = gg.res.weapon1_1;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon1_2::_cfg.Emplace();
		c->name = i18n::Strs::Equip_weapon1_2;
		c->icon = gg.res.weapon1_2;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Excellent;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon1_3::_cfg.Emplace();
		c->name = i18n::Strs::Equip_weapon1_3;
		c->icon = gg.res.weapon1_3;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Epic;
		c->props.Emplace(EquipConfigProp{ EquipTargets::Creature, 4, {30, 50} });
		c->props.Emplace(EquipConfigProp{ EquipTargets::Creature, 6, {2, 2} });
	}
	{
		auto& c = Equip_weapon2_1::_cfg.Emplace();
		c->name = i18n::Strs::Equip_weapon2_1;
		c->icon = gg.res.weapon2_1;
		c->location = EquipLocations::Weapon2;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
}
