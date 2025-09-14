#pragma once
#include "game.h"
#include "monster.h"
#include "player.h"
#include "equip.h"
#include "ui_equipbag.h"
#include "ui_propslog.h"

struct Scene_Play : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Weak<UI::EquipBag> uiEquipBag;
	xx::Weak<UI::PropsLog> uiPropsLog;

	xx::Camera cam;

	float time{}, timePool{}, timeScale{ 1 };

	xx::Shared<Player> player;
	xx::List<xx::Shared<Monster>> monsters;

	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize() override;
};
