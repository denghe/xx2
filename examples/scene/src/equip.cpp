#include "pch.h"
#include "scene_play.h"
#include "equip.h"

void Equip::Init(Creature* owner_, xx::Shared<EquipConfig> cfg_) {
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
	gg.Quad().Draw(f, f, pos_, anchor_, { size_.x / f.uvRect.w, size_.y / f.uvRect.h }, 0, colorplus_);
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
	| desc desc desc de |
	| ...         ...   |
	| $         quality |
	+-------------------+
	*/
	auto& ui = gg.scene.Cast<Scene_Play>()->ui;	// root
	// todo: handle cmp_

	static constexpr float fontSize{ 32 }, lineHeight{ 48 }, margin{ 12 };

	// container
	auto& r = ui->Make<xx::Node>();

	// content
	xx::Layouter L;
	L.InitBegin(r, ui->z + 100, 0, { 0, 1 }, fontSize * 20 + margin * 2);
	L.Image(cfg->icon, lineHeight * 2);
	L.EndLine(false, lineHeight * 2 + margin);
	L.Text(gg.lang(cfg->name), fontSize, lineHeight);
	L.EndLine(true, lineHeight * 2 + margin);
	L.Text(gg.lang(EquipLocationsStrs[(int32_t)cfg->location]), fontSize, lineHeight);
	L.EndLine();
	assert(cfg->props.len == props.len);
	for (int32_t i = 0; i < props.len; ++i) {
		auto& o = props[i];
		auto& c = cfg->props[i];
		L.Text(gg.lang(CreaturePropertiesStrs[o.index]), fontSize, lineHeight);
		L.EndLine(false);
		L.Text(xx::ToString(o.value, " (", c.value.from, " ~ ", c.value.to, ")"), fontSize, lineHeight);
		L.EndLine(xx::HAligns::Right);
	}
	// ...
	// todo: desc. translate to rich node?
	// price
	// quality
	L.InitEnd();

	// bg / border
	r->Make<xx::Scale9>()->Init(r->z - 1, -margin, 0, r->size + margin * 2)
		.cfg.color = EquipQualityColors[(int32_t)cfg->quality];
	return r;
}

void Equip::WriteTo(xx::Data& d) {
	d.Write(cfg->typeId);
	for (int32_t i = 0; i < props.len; ++i) {
		d.Write(props[i].value);
	}
}

int Equip::ReadFrom(xx::Data_r& dr_, Creature* owner_, xx::Shared<Equip>& tar_) {
	uint32_t typeId{};
	if (auto r = dr_.Read(typeId)) return r;
	if (typeId == 0) return 0;
	if (typeId >= cfgs.len) return __LINE__;
	tar_ = makers[typeId]();
	tar_->owner = owner_;
	tar_->scene = owner_->scene;
	tar_->cfg = cfgs[typeId];
	auto& cps = tar_->cfg->props;
	if (!cps.len) return 0;
	tar_->props.Reserve(cps.len);
	for (int32_t i = 0; i < cps.len; ++i) {
		auto& p = cps[i];
		float v{};
		if (auto r = dr_.Read(v)) return {};
		if (v < p.value.from || v > p.value.to) return {};
		tar_->props.Emplace(EquipProp{ p.target, p.index, v });
	}
	return 0;
}

xx::Shared<Equip> Equip::Make(Creature* owner_, uint32_t typeId_) {
	auto r = makers[typeId_]();
	r->Init(owner_, cfgs[typeId_]);
	return r;
}

void Equip::InitCfgs() {
	assert(cfgs.Empty());
	assert(makers.Empty());
	cfgs.Emplace();
	makers.Emplace();
	{
		auto& c = MakeMakerAndCfg<Equip_amulet_1>();
		c->name = i18n::Strs::Equip_amulet_1;
		c->icon = gg.fs.amulet_1;
		c->location = EquipLocations::Amulet;
		c->quality = EquipQualities::Normal;
		c->props.Emplace(EquipConfigProp{ EquipTargets::Creature, 0, {10, 10} });
		c->props.Emplace(EquipConfigProp{ EquipTargets::Creature, 25, {3, 5} });
	}
	{
		auto& c = MakeMakerAndCfg<Equip_armor_1>();
		c->name = i18n::Strs::Equip_armor_1;
		c->icon = gg.fs.armor_1;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_armor_2>();
		c->name = i18n::Strs::Equip_armor_2;
		c->icon = gg.fs.armor_2;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Excellent;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_armor_3>();
		c->name = i18n::Strs::Equip_armor_3;
		c->icon = gg.fs.armor_3;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Rare;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_boots_1>();
		c->name = i18n::Strs::Equip_boots_1;
		c->icon = gg.fs.boots_1;
		c->location = EquipLocations::Boots;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_consumable_1>();
		c->name = i18n::Strs::Equip_consumable_1;
		c->icon = gg.fs.consumable_1;
		c->location = EquipLocations::Consumables;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_currency_1>();
		c->name = i18n::Strs::Equip_currency_1;
		c->icon = gg.fs.currency_1;
		c->location = EquipLocations::Currency;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_currency_2>();
		c->name = i18n::Strs::Equip_currency_2;
		c->icon = gg.fs.currency_2;
		c->location = EquipLocations::Currency;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_helm_1>();
		c->name = i18n::Strs::Equip_helm_1;
		c->icon = gg.fs.helm_1;
		c->location = EquipLocations::Helm;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_material_1>();
		c->name = i18n::Strs::Equip_material_1;
		c->icon = gg.fs.material_1;
		c->location = EquipLocations::Materials;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_ring_1>();
		c->name = i18n::Strs::Equip_ring_1;
		c->icon = gg.fs.ring_1;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Rare;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_ring_2>();
		c->name = i18n::Strs::Equip_ring_2;
		c->icon = gg.fs.ring_2;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Epic;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_ring_3>();
		c->name = i18n::Strs::Equip_ring_3;
		c->icon = gg.fs.ring_3;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Legendary;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_ring_4>();
		c->name = i18n::Strs::Equip_ring_4;
		c->icon = gg.fs.ring_4;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Ancient;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_weapon1_1>();
		c->name = i18n::Strs::Equip_weapon1_1;
		c->icon = gg.fs.weapon1_1;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_weapon1_2>();
		c->name = i18n::Strs::Equip_weapon1_2;
		c->icon = gg.fs.weapon1_2;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Excellent;
		//c->props.Emplace(......)
	}
	{
		auto& c = MakeMakerAndCfg<Equip_weapon1_3>();
		c->name = i18n::Strs::Equip_weapon1_3;
		c->icon = gg.fs.weapon1_3;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Epic;
		c->props.Emplace(EquipConfigProp{ EquipTargets::Creature, 4, {30, 50} });
		c->props.Emplace(EquipConfigProp{ EquipTargets::Creature, 6, {2, 2} });
	}
	{
		auto& c = MakeMakerAndCfg<Equip_weapon2_1>();
		c->name = i18n::Strs::Equip_weapon2_1;
		c->icon = gg.fs.weapon2_1;
		c->location = EquipLocations::Weapon2;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
}
