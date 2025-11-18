#pragma once
#include "game.h"
#include "game_sceneitem.h"
#include "game_rock.h"

enum class AnimTypes {
	Idle, Move, Atk
};

struct Monster : SceneItem {
	xx::TinyFrame* tfs{};
	char* cds{};
	int32_t monsterTypeId{};
	int32_t tfsLen{};
	XY ap{};
	float tfIndex{};
	float resRadius{};
	float frameDelay{};
	float speedScale{};
	float radius{};
	bool flipX{};
	XY pos{};

	xx::Weak<Rock> target;
	XY targetPos{};
	float stepTime{};
	float attackRange{}, moveSpeed{};
	char hited{};
	int32_t _1{};

	Monster& Init(Scene* scene_, int32_t monsterTypeId_, XY pos_, float radius_);
	void SetAnim(AnimTypes t);
	bool StepAnimOnce();
	void StepAnimLoop();
	bool SearchTarget();
	char GetHitData() const;
	bool Update() override;
	void Draw() override;
};
