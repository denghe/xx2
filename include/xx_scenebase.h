#pragma once
#include "xx_gamebase.h"

namespace xx {

	struct alignas(8) SceneBase {
		virtual ~SceneBase() {}
		virtual void Update() {};
		virtual void Draw() {};
		virtual void OnResize() {};

		XY size{};								// actual design size
		XY size_2{};							// actual design size / 2
		float scale{};							// base scale. actual design size * scale = physics size

		/*
                 screen anchor points
		   ┌───────────────────────────────┐
		   │ 7             8             9 │
		   │                               │
		   │                               │
		   │ 4             5             6 │
		   │                               │
		   │                               │
		   │ 1             2             3 │
		   └───────────────────────────────┘
		*/
		XY pos7{}, pos8{}, pos9{};
		XY pos4{}, pos5{}, pos6{};
		XY pos1{}, pos2{}, pos3{};

		static constexpr XY anchor7{ 0, 1 }, anchor8{ 0.5, 1 }, anchor9{ 1, 1 };
		static constexpr XY anchor4{ 0, 0.5f }, anchor5{ 0.5, 0.5f }, anchor6{ 1, 0.5f };
		static constexpr XY anchor1{ 0, 0 }, anchor2{ 0.5, 0 }, anchor3{ 1, 0 };

		void ResizeCalc() {
			auto& ds = GameBase::instance->designSize;
			auto& ws = GameBase::instance->windowSize;
			if (ws.x / ds.x > ws.y / ds.y) {
				scale = ws.y / GameBase::instance->designSize.y;
				size.y = GameBase::instance->designSize.y;
				size.x = ws.x / scale;
			}
			else {
				scale = ws.x / GameBase::instance->designSize.x;
				size.x = GameBase::instance->designSize.x;
				size.y = ws.y / scale;
			}
			size_2 = size * 0.5f;
			pos7 = { -size_2.x, +size_2.y }; pos8 = { 0, +size_2.y }; pos9 = { +size_2.x, +size_2.y };
			pos4 = { -size_2.x, 0 }; pos5 = { 0, 0 }; pos6 = { +size_2.x, 0 };
			pos1 = { -size_2.x, -size_2.y }; pos2 = { 0, -size_2.y }; pos3 = { +size_2.x, -size_2.y };
		};
	};
}
