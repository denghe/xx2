#include "pch.h"
#include "scene_play.h"
#include "equip.h"

void Equip::Init(Player* owner_, xx::Ref<EquipConfig> cfg_) {
	owner = owner_;
	scene = owner_->scene;
	cfg = std::move(cfg_);
	location = cfg->location;
	quality = cfg->quality;
	auto& cps = cfg->props;
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

xx::Shared<xx::Node> Equip::GetInfoPanel() {
	// todo
	return {};
}
