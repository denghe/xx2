#pragma once
#include "xx_prims.h"
#include "xx_rnd.h"

// base & factor
struct CreatureProperties1 {
	// health
	float healthMaxPreset{};	// 0
	float healthMaxFactor{};	// 1
	// health regeneration
	float healthRegenerationPreset{};	// 2
	float healthRegenerationFactor{};	// 3
	// mana
	float manaMaxPreset{};	// 4
	float manaMaxFactor{};	// 5
	// mana regeneration
	float manaRegenerationPreset{};	// 6
	float manaRegenerationFactor{};	// 7
	// defense
	float defensePreset{};	// 8
	float defenseFactor{};	// 9
	// dodge
	float dodgePreset{};	// 10
	float dodgeFactor{};	// 11
	// invincible frame( seconds )
	float invincibleFrameDurationPreset{};	// 12
	float invincibleFrameDurationFactor{};	// 13
	// damage ratio
	float damagePreset{};	// 14
	// critical ratio
	float criticalChancePreset{};	// 15
	float criticalChanceFactor{};	// 16
	// critical damage
	float criticalDamagePreset{};	// 17
	float criticalDamageFactor{};	// 18
	// movement speed( pixel distance per seconds )
	float movementSpeedPreset{};	// 19
	float movementSpeedFactor{};	// 20
	// jump height( pixel distance max val map to 0 ~ 1 )
	float jumpHeightPreset{};	// 21
	float jumpHeightFactor{};	// 22
	// jump extra times
	float jumpExtraNumsPreset{};	// 23
	float jumpExtraNumsFactor{};	// 24
	// lucky
	float luckyPreset{};	// 25
	float luckyFactor{};	// 26
};

// result & runtime
struct CreatureProperties2 {
	// health
	float healthMax{};	// 27
	float health{};	// 28
	// health regeneration
	float healthRegeneration{};	// 29
	// mana
	float manaMax{};	// 30
	float mana{};	// 31
	// mana regeneration
	float manaRegeneration{};	// 32
	// defense( damage reduce percentage
	float defense{};	// 33
	// dodge( miss success percentage
	float dodge{};	// 34
	// invincible frame
	float invincibleFrameDuration{};	// 35
	// damage ratio
	float damage{};	// 36
	// critical ratio
	float criticalChance{};	// 37
	// critical damage ratio
	float criticalDamage{};	// 38
	// movement speed
	float movementSpeed{};	// 39
	// jump height
	float jumpHeight{};	// 40
	// jump extra times
	float jumpExtraNums{};	// 41
	// lucky
	float lucky{};	// 42
	float harvestFactor{};	// 43
	float dropCalcNums{};	// 44
};

struct CreatureProperties : CreatureProperties1, CreatureProperties2 {
	static constexpr int32_t numProps{ (sizeof(CreatureProperties1) 
		+ sizeof(CreatureProperties2)) / sizeof(float) };
	float regenerationTime{};

	// call at player init
	void Init() {
		healthMaxPreset = 100.f;
		healthMaxFactor = 1.f;
		healthRegenerationPreset = 0.f;
		healthRegenerationFactor = 1.f;
		manaMaxPreset = 100.f;
		manaMaxFactor = 1.f;
		manaRegenerationPreset = 10.f;
		manaRegenerationFactor = 1.f;
		defensePreset = 0.f;
		defenseFactor = 1.f;
		dodgePreset = 0.f;
		dodgeFactor = 1.f;
		invincibleFrameDurationPreset = 0.1f;
		invincibleFrameDurationFactor = 1.f;
		damagePreset = 1.f;
		criticalChancePreset = 0.1f;
		criticalChanceFactor = 1.f;
		criticalDamagePreset = 2.f;
		criticalDamageFactor = 1.f;
		movementSpeedPreset = 1.f;
		movementSpeedFactor = 1.f;
		jumpHeightPreset = 1.f;
		jumpHeightFactor = 1.f;
		jumpExtraNumsPreset = 2.f;
		jumpExtraNumsFactor = 1.f;
		luckyPreset = 0.f;
		luckyFactor = 1.f;
	}

	// used by foreach equip & add values
	float& At(int32_t idx_) const {
		assert(idx_ >= 0 && idx_ < numProps);
		return ((float*)this)[idx_];
	}

	// call at player init & equipment changed
	void CalcAll() {
		healthMax = std::max(healthMaxPreset * healthMaxFactor, 1.f);
		health = healthMax;
		healthRegeneration = healthRegenerationPreset * healthRegenerationFactor;	// can < 0
		manaMax = std::max(manaMaxPreset * manaMaxFactor, 0.f);
		mana = manaMax;
		manaRegeneration = manaRegenerationPreset * manaRegenerationFactor;	// can < 0
		defense = 100.f / (100.f + defensePreset * defenseFactor);
		dodge = 1.f - 100.f / (100.f + dodgePreset * dodgeFactor);
		invincibleFrameDuration = std::max(invincibleFrameDurationPreset * invincibleFrameDurationFactor, 0.f);
		damage = damagePreset;
		criticalChance = std::max(std::min(criticalChancePreset * criticalChanceFactor, 1.f), 0.f);
		criticalDamage = std::max(criticalDamagePreset * criticalDamageFactor, 1.f);
		movementSpeed = std::max(std::min(movementSpeedPreset * movementSpeedFactor, 5.f), 0.f);
		jumpHeight = std::max(std::min(jumpHeightPreset * jumpHeightFactor, 5.f), 0.f);
		jumpExtraNums = std::max(jumpExtraNumsPreset * jumpExtraNumsFactor, 0.f);
		lucky = luckyPreset * luckyFactor;
		harvestFactor = std::max(1.f + lucky / 100.f, 0.f);
		dropCalcNums = std::max(int32_t(1.f + lucky / 100.f), 0);
	}

	// return damage point & is critical
	XX_INLINE std::pair<float, bool> CalcDamagePoint(xx::Rnd& rnd, float baseDamagePoint) {
		auto d = baseDamagePoint * damage;
		if (rnd.Next<float>() <= criticalChance) {
			return { d * criticalDamage, true };
		}
		return { d, false };
	}

	// return actual hurt dp, int: 0 normal 1 dodge 2 death
	XX_INLINE std::pair<float, int> Hurt(xx::Rnd& rnd, float targetDamagePoint) {
		if (health <= 0) return { 0.f, 2 };
		if (rnd.Next<float>() <= dodge) return { 0.f, 1 };
		targetDamagePoint *= defense;
		if (health <= targetDamagePoint) {
			auto d = targetDamagePoint - health;
			health = 0;
			return { d, 2 };
		}
		else {
			health -= targetDamagePoint;
			return{ targetDamagePoint, 0 };
		}
	}

	// return regeneration quantity health, mana, success ( call for every frame update )
	XX_INLINE std::tuple<float, float, bool> TryRegeneration(float interval_, float time_) {
		if (regenerationTime > time_) return {};
		auto bakHealth = health;
		auto bakMana = mana;
		health = std::min(health + healthRegeneration, healthMax);
		mana = std::min(mana + manaRegeneration, manaMax);
		regenerationTime += interval_;
		return { health - bakHealth, mana - bakMana, true };
	}
	// ... more funcs
};
