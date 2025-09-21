#include "pch.h"
#include "scene_play.h"
#include "equip.h"

void Equip::EquipInit(Creature* owner_, xx::Ref<EquipConfig> cfg_) {
	owner = owner_;
	scene = owner_->scene;
	cfg = std::move(cfg_);
	location = cfg->location;
	quality = cfg->quality;
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
	| type              |
	| props       value |
	| props       value |
	| props       value |
	| ...         ...   |
	|                   |
	| $         quality |
	+-------------------+
	*/
	// root
	auto& ui = gg.scene.Cast<Scene_Play>()->ui;

	// todo: handle cmp_

	// container
	auto& r = ui->Make<xx::Node>();
	r->Init(ui->z + 100, gg.mousePos, { 0, 1 }, 1, { 200, 300 });

	// bg
	r->Make<xx::Scale9>()->Init(r->z, 0, 0, r->size).cfg.color = EquipQualityColors[(int32_t)quality];

	// icon
	// name
	// ...
	// desc translate to rich node?
	// price
	// quality
	return r;
}

void Equip::InitAllCfgs() {
	{
		auto& c = Equip_amulet_1::_cfg;
		c.Emplace();
		c->icon = gg.res.amulet_1;
		c->location = EquipLocations::Amulet;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_armor_1::_cfg;
		c.Emplace();
		c->icon = gg.res.armor_1;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_armor_2::_cfg;
		c.Emplace();
		c->icon = gg.res.armor_2;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Excellent;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_armor_3::_cfg;
		c.Emplace();
		c->icon = gg.res.armor_3;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Rare;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_boots_1::_cfg;
		c.Emplace();
		c->icon = gg.res.boots_1;
		c->location = EquipLocations::Boots;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_consumable_1::_cfg;
		c.Emplace();
		c->icon = gg.res.consumable_1;
		c->location = EquipLocations::Consumables;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_currency_1::_cfg;
		c.Emplace();
		c->icon = gg.res.currency_1;
		c->location = EquipLocations::Currency;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_currency_2::_cfg;
		c.Emplace();
		c->icon = gg.res.currency_2;
		c->location = EquipLocations::Currency;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_helm_1::_cfg;
		c.Emplace();
		c->icon = gg.res.helm_1;
		c->location = EquipLocations::Helm;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_material_1::_cfg;
		c.Emplace();
		c->icon = gg.res.material_1;
		c->location = EquipLocations::Materials;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_1::_cfg;
		c.Emplace();
		c->icon = gg.res.ring_1;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Rare;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_2::_cfg;
		c.Emplace();
		c->icon = gg.res.ring_2;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Epic;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_3::_cfg;
		c.Emplace();
		c->icon = gg.res.ring_3;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Legendary;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_4::_cfg;
		c.Emplace();
		c->icon = gg.res.ring_4;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Ancient;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon1_1::_cfg;
		c.Emplace();
		c->icon = gg.res.weapon1_1;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon1_2::_cfg;
		c.Emplace();
		c->icon = gg.res.weapon1_2;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Excellent;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon1_3::_cfg;
		c.Emplace();
		c->icon = gg.res.weapon1_3;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Epic;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon2_1::_cfg;
		c.Emplace();
		c->icon = gg.res.weapon2_1;
		c->location = EquipLocations::Weapon2;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
}
