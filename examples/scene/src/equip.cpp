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


void Equip::Draw(XY pos_, XY anchor_, XY size_, float colorplus_) {
	auto& f = cfg->icon;
	gg.Quad().Draw(f, f, pos_, anchor_, { size_.x / f.textureRect.w, size_.y / f.textureRect.h }, 0, colorplus_);
}

// ...
