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

xx::Shared<xx::Node> Equip::GenInfoPanel() {
	//auto r = parent_->Make<xx::Node>();
	//auto pos = gg.mousePos;// +gg.worldMaxXY;
	//r->InitDerived<xx::Node>(r->z, pos, { 0, 1 }, 1, { 200, 300 });
	auto& ui = gg.scene.Cast<Scene_Play>()->ui;
	auto r = ui->Make<xx::Scale9>();
	r->Init(ui->z + 100, gg.mousePos, { 0, 1 }, { 200, 300 });
	// todo: calc size & change anchor ?
	// ...
	return r;
}


void Equip_Blade::Draw(XY pos_, XY anchor_, XY size_) {
	auto& f = gg.res.blade;
	gg.Quad().Draw(f, f, pos_, anchor_, { size_.x / f.textureRect.w, size_.y / f.textureRect.h });
}


void Equip_Blood::Draw(XY pos_, XY anchor_, XY size_) {
	auto& f = gg.embed.shape_heart;
	gg.Quad().Draw(f, f, pos_, anchor_, { size_.x / f.textureRect.w, size_.y / f.textureRect.h });
}


void Equip_Bomb::Draw(XY pos_, XY anchor_, XY size_) {
	auto& f = gg.res.explosion_[0];
	gg.Quad().Draw(f, f, pos_, anchor_, { size_.x / f.textureRect.w, size_.y / f.textureRect.h });
}

// ...
