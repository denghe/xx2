#pragma once
#include "game.h"

namespace Test5 {

	struct Scene;

	enum class TalentTypes {
		A, B, C
	};

	enum class CurrencyTypes {
		A, B, C
		, __MAX_VALUE__
	};

	static constexpr auto cNumCurrencyTypes{ std::to_underlying(CurrencyTypes::__MAX_VALUE__) };

	struct TalentPrice {
		CurrencyTypes currencyType{};
		int32_t value{};
	};

	struct TalentBase {
		TalentTypes type{};
		int32_t id{}, parentId{};
		int32_t maxLevel{};
		xx::List<xx::List<TalentPrice>> levelPricess;
		XY pos{};
		// todo: joystick focus?
		int32_t level{};							// runtime / player settings?
		bool canLevelUp{};							// runtime
		bool visible{};								// runtime
		TalentBase* parent{};						// runtime
		Scene* scene{};								// runtime
		virtual void LevelUp();						// ++level && deducte resources?
		virtual xx::Shared<xx::Node> GetInfo();		// generate info panel ui
		virtual void Update();						// set canLevelUp? visible?
		virtual void Draw();						// draw icon by type. draw line
		virtual ~TalentBase() {}
		virtual void FillInfo(xx::Layouter& L_);
	};

	struct TalentA : TalentBase {
		static constexpr TalentTypes cType{ TalentTypes::A };
		void FillInfo(xx::Layouter& L_) override;
	};

	struct TalentB : TalentBase {
		static constexpr TalentTypes cType{ TalentTypes::B };
		void FillInfo(xx::Layouter& L_) override;
	};

	struct TalentC : TalentBase {
		static constexpr TalentTypes cType{ TalentTypes::C };
		void FillInfo(xx::Layouter& L_) override;
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };
		float timer{};

		xx::Weak<xx::Node> uiInfo;
		std::array<int32_t, cNumCurrencyTypes> currency;
		std::array<xx::Weak<xx::Label>, cNumCurrencyTypes> currencyUI;

		TalentBase* currTalent{};
		float talentScale{};
		XY talentBasePos{};
		XY lastMousePos{};
		bool lastMBPressed{};	// for first press check
		bool talentDragging{};
		bool clicking{};
		xx::Grid2dCircle<TalentBase*> talentsGrid;
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
