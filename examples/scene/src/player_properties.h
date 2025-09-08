#pragma once
#include "xx_prims.h"
#include "xx_rnd.h"

// base & factor
struct PlayerProperties1 {
	// health
	float healthMaxPreset{};
	float healthMaxFactor{};
	// health regeneration
	float healthRegenerationPreset{};
	float healthRegenerationFactor{};
	// mana
	float manaMaxPreset{};
	float manaMaxFactor{};
	// mana regeneration
	float manaRegenerationPreset{};
	float manaRegenerationFactor{};
	// defense
	float defensePreset{};
	float defenseFactor{};
	// dodge
	float dodgePreset{};
	float dodgeFactor{};
	// invincible frame( seconds )
	float invincibleFrameDurationPreset{};
	float invincibleFrameDurationFactor{};
	// damage ratio
	float damagePreset{};
	// critical ratio
	float criticalChancePreset{};
	float criticalChanceFactor{};
	// critical damage
	float criticalDamagePreset{};
	float criticalDamageFactor{};
	// movement speed( pixel distance per seconds )
	float movementSpeedPreset{};
	float movementSpeedFactor{};
	// jump height( pixel distance max val map to 0 ~ 1 )
	float jumpHeightPreset{};
	float jumpHeightFactor{};
	// jump extra times
	float jumpExtraNumsPreset{};
	float jumpExtraNumsFactor{};
	// lucky
	float luckyPreset{};
	float luckyFactor{};
};

// result & runtime
struct PlayerProperties2 {
	// health
	float healthMax{};
	float health{};						// runtime
	// health regeneration
	float healthRegeneration{};
	// mana
	float manaMax{};
	float mana{};						// runtime
	// mana regeneration
	float manaRegeneration{};
	// defense
	float defense{};					// damage reduce percentage
	// dodge
	float dodge{};						// miss success percentage
	// invincible frame
	float invincibleFrameDuration{};
	// damage ratio
	float damage{};
	// critical ratio
	float criticalChance{};
	// critical damage ratio
	float criticalDamage{};
	// movement speed
	float movementSpeed{};
	// jump height
	float jumpHeight{};
	// jump extra times
	float jumpExtraNums{};
	// lucky
	float lucky{};
	float harvestFactor{};
	int32_t dropCalcNums{};
};

struct PlayerProperties : PlayerProperties1, PlayerProperties2 {
	int32_t regenerationFrameNumber{};

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
	XX_INLINE std::tuple<float, float, bool> TryRegeneration(int32_t numIntervalFrames, int32_t currentFrameNumber) {
		if (regenerationFrameNumber > currentFrameNumber) return {};
		auto bakHealth = health;
		auto bakMana = mana;
		health = std::min(health + healthRegeneration, healthMax);
		mana = std::min(mana + manaRegeneration, manaMax);
		regenerationFrameNumber += numIntervalFrames;
		return { health - bakHealth, mana - bakMana, true };
	}
	// ... more funcs
};

