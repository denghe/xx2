#include "pch.h"
#include "game.h"
#include "scene_mainmenu.h"
#include "xx_rectpacker.h"
#include "equip.h"
Game gg;

int32_t main() {
	// load settings from player data store file
	if (int r = gg.SavInit(std::filesystem::current_path())) return r;
	if (int r = gg.Load()) return r;

	// enter game loop
	return gg.Run();
}

void Game::Init() {
	title = "examples_scene";
}

void Game::GLInit() {
	// sync lang settings
	lang.Set((i18n::Languages)settings.language);

	// sync video settings
	if (settings.isFullScreen) {
		if (settings.isBorderless) {
			SetBorderlessMode();
		}
		else {
			SetFullScreenMode(Settings::cResolutions[settings.resolutionsIndex]);
		}
	}
	else {
		SetWindowMode(designSize);
	}

	// sync sound settings
	mute = settings.mute;
	masterVolume = settings.masterVolume;
	audioVolume = settings.audioVolume;
	musicVolume = settings.musicVolume;
	sound.SetGlobalVolume(masterVolume);
}

xx::Task<> Game::Task() {
	// begin load res

	// ogg
	res.explosion = LoadSoundSource("res/explosion.ogg");

	// png
	res.damage_numbers = LoadTexture("res/damage_numbers.png");	// do not pack
	res.hpbar = LoadTexture("res/hpbar.png");					// do not pack

	res.blade = LoadTexture("res/blade.png");
	res.monster = LoadTexture("res/monster.png");
	res.player = LoadTexture("res/player.png");
	res.cell_bg = LoadTexture("res/cell_bg.png");

	res.helm_1 = LoadTexture("res/helm_1.png");
	res.amulet_1 = LoadTexture("res/amulet_1.png");
	res.ring_1 = LoadTexture("res/ring_1.png");
	res.ring_2 = LoadTexture("res/ring_2.png");
	res.ring_3 = LoadTexture("res/ring_3.png");
	res.ring_4 = LoadTexture("res/ring_4.png");
	res.armor_1 = LoadTexture("res/armor_1.png");
	res.armor_2 = LoadTexture("res/armor_2.png");
	res.armor_3 = LoadTexture("res/armor_3.png");
	res.weapon1_1 = LoadTexture("res/weapon1_1.png");
	res.weapon1_2 = LoadTexture("res/weapon1_2.png");
	res.weapon1_3 = LoadTexture("res/weapon1_3.png");
	res.weapon2_1 = LoadTexture("res/weapon2_1.png");
	res.boots_1 = LoadTexture("res/boots_1.png");
	res.currency_2 = LoadTexture("res/currency_2.png");
	res.currency_1 = LoadTexture("res/currency_1.png");
	res.material_1 = LoadTexture("res/material_1.png");
	res.consumable_1 = LoadTexture("res/consumable_1.png");

	res.explosion_.Add(
		LoadTexture("res/explosion_0.png"),
		LoadTexture("res/explosion_1.png"),
		LoadTexture("res/explosion_2.png"),
		LoadTexture("res/explosion_3.png"),
		LoadTexture("res/explosion_4.png"),
		LoadTexture("res/explosion_5.png"),
		LoadTexture("res/explosion_6.png")
	);

	// combine some pngs into single texture
	xx::RectPacker tp;
	for (auto tf = &res.blade; tf != &res.consumable_1; ++tf) {		// first to last
		tp.tfs.Add(tf);
	}
	for (auto& tf : res.explosion_) {
		tp.tfs.Add(&tf);
	}
	if (auto r = tp.Pack(1024); r) {
		xx::CoutN("pack failed");
	}

	// init equip configs
	{
		auto& c = Equip_amulet_1::_cfg;
		c.Emplace();
		c->icon = res.amulet_1;
		c->location = EquipLocations::Amulet;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_armor_1::_cfg;
		c.Emplace();
		c->icon = res.armor_1;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_armor_2::_cfg;
		c.Emplace();
		c->icon = res.armor_2;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Excellent;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_armor_3::_cfg;
		c.Emplace();
		c->icon = res.armor_3;
		c->location = EquipLocations::Armor;
		c->quality = EquipQualities::Rare;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_boots_1::_cfg;
		c.Emplace();
		c->icon = res.boots_1;
		c->location = EquipLocations::Boots;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_consumable_1::_cfg;
		c.Emplace();
		c->icon = res.consumable_1;
		c->location = EquipLocations::Consumables;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_currency_1::_cfg;
		c.Emplace();
		c->icon = res.currency_1;
		c->location = EquipLocations::Currency;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_currency_2::_cfg;
		c.Emplace();
		c->icon = res.currency_2;
		c->location = EquipLocations::Currency;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_helm_1::_cfg;
		c.Emplace();
		c->icon = res.helm_1;
		c->location = EquipLocations::Helm;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_material_1::_cfg;
		c.Emplace();
		c->icon = res.material_1;
		c->location = EquipLocations::Materials;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_1::_cfg;
		c.Emplace();
		c->icon = res.ring_1;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Rare;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_2::_cfg;
		c.Emplace();
		c->icon = res.ring_2;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Epic;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_3::_cfg;
		c.Emplace();
		c->icon = res.ring_3;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Legendary;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_ring_4::_cfg;
		c.Emplace();
		c->icon = res.ring_4;
		c->location = EquipLocations::Ring;
		c->quality = EquipQualities::Ancient;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon1_1::_cfg;
		c.Emplace();
		c->icon = res.weapon1_1;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon1_2::_cfg;
		c.Emplace();
		c->icon = res.weapon1_2;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Excellent;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon1_3::_cfg;
		c.Emplace();
		c->icon = res.weapon1_3;
		c->location = EquipLocations::Weapon1;
		c->quality = EquipQualities::Epic;
		//c->props.Emplace(......)
	}
	{
		auto& c = Equip_weapon2_1::_cfg;
		c.Emplace();
		c->icon = res.weapon2_1;
		c->location = EquipLocations::Weapon2;
		c->quality = EquipQualities::Normal;
		//c->props.Emplace(......)
	}








	// init first scene
	scene.Emplace<Scene_MainMenu>()->Init();

	// game loop
	while (true) {
		scene->Update();
		scene->Draw();
		if (oldScene) oldScene.Reset();
		co_yield 0;
	}
}

void Game::Delay() {
#if 1
	// for power saving, fps limit
	SleepSecs(cDelta - (glfwGetTime() - time));	
#endif
}

void Game::OnResize(bool modeChanged_) {
	if (scene) {
		scene->OnResize(modeChanged_);
	}
}

void Game::Stat() {
#if 0
	xx::CoutN("drawFPS = ", drawFPS, " drawCall = "
		, drawCall, " drawVerts = ", drawVerts
		, " uiAutoUpdates.len = ", uiAutoUpdates.len
		, " delayUpdates.len = ", delayUpdates.len
	);
#endif
}
