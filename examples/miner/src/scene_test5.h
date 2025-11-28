#pragma once
#include "game.h"

namespace Test5 {

	// talent: config + data
	// config: node ( id/index, parentId, [children], type, level, levelup condition, pos, info panel )
	// data: [id+level]...
	// colors: 
	// can't levelup: red
	// can levelup: green
	// levelup max: blue

	struct Scene;

	enum class TalentTypes {
		A, B, C
	};

	struct TalentBase {
		TalentTypes type{};
		int32_t id{}, parentId{};
		int32_t maxLevel{};
		XY pos{};
		// todo: grid for mouse pos check? joystick focus?
		int32_t level{};				// runtime / player settings?
		bool canLevelUp{};				// runtime
		bool visible{};					// runtime
		TalentBase* parent{};			// runtime
		Scene* scene{};					// runtime
		//xx::List<TalentBase*> children;	// runtime
		virtual void LevelUp() {};					// ++level && deducte resources ?
		virtual xx::Shared<xx::Node> GetInfo() { return {}; };	// gen ui panel by info + levelup condition
		virtual void Update() {};								// set canLevelUp?
		virtual void Draw();						// draw icon by type. draw line
		virtual ~TalentBase() {}
	};

	struct TalentA : TalentBase {
		static constexpr TalentTypes cType{ TalentTypes::A };
		void LevelUp() override;
		xx::Shared<xx::Node> GetInfo() override;
		void Update() override;
	};

	struct TalentB : TalentBase {
		static constexpr TalentTypes cType{ TalentTypes::B };
		void LevelUp() override;
		xx::Shared<xx::Node> GetInfo() override;
		void Update() override;
	};

	struct TalentC : TalentBase {
		static constexpr TalentTypes cType{ TalentTypes::C };
		void LevelUp() override;
		xx::Shared<xx::Node> GetInfo() override;
		void Update() override;
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };
		float timer{};

		xx::Weak<xx::Node> uiInfo;
		int32_t currencyA{}, currencyB{}, currencyC{};
		float talentScale{};
		XY talentBasePos{};
		XY lastMousePos{};
		bool lastMBPressed{};	// for first press check
		bool talentDragging{};
		bool clicking{};
		xx::List<xx::Shared<TalentBase>> talents;
		void SetTalentLevel(int32_t id_, int32_t level_);
		TalentBase* FindTalent(XY pos_);

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
