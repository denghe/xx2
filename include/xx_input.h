#pragma once
#include "xx_prims.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>
#define GLFW_MOUSE_BUTTON_WHEEL_UP (GLFW_MOUSE_BUTTON_LAST + 1)
#define GLFW_MOUSE_BUTTON_WHEEL_DOWN (GLFW_MOUSE_BUTTON_LAST + 2)
#define GLFW_MOUSE_BUTTON_WHEEL_LEFT (GLFW_MOUSE_BUTTON_LAST + 3)
#define GLFW_MOUSE_BUTTON_WHEEL_RIGHT (GLFW_MOUSE_BUTTON_LAST + 4)

namespace xx {

	// for mouse, keyboard, joystick
	struct BtnState {
		int32_t pressed{};
		float lastPressedTime{};
		float lastActivedTime{};

		XX_INLINE operator bool() const {
			return pressed;
		}

		XX_INLINE void Press(float t) {
			pressed = true;
			lastPressedTime = t;
			// lastActivedTime not set
		}

		XX_INLINE void Release() {
			pressed = {};
			lastPressedTime = {};
			lastActivedTime = {};
		}

		// auto release. return pressed
		XX_INLINE bool Once() {
			if (pressed) {
				Release();
				return true;
			}
			return false;
		}

		// handle lastActivedTime
		XX_INLINE bool operator()(float interval_) {
			if (pressed && lastPressedTime >= lastActivedTime + interval_) {
				lastActivedTime = lastPressedTime + interval_;
				return true;
			}
			return false;
		}
	};

	struct JoyState {
		int32_t jid{ -1 };
		std::string name;

		//	A, B, X, Y, L1, R1, Back, Start, Home, L2, R2, Up, Right, Down, Left
		std::array<BtnState, GLFW_GAMEPAD_BUTTON_LAST + 1> btns{};

		// AxisLeft H V, AxisRight H V, Trigger L R
		std::array<float, GLFW_GAMEPAD_AXIS_LAST + 1> axes{};

		void ClearValues() {
			for (auto& btn : btns) {
				btn.pressed = 0;
				btn.lastPressedTime = 0;
			}
			memset(&axes, 0, sizeof(float) * GLFW_GAMEPAD_AXIS_LAST - 1);
			axes[GLFW_GAMEPAD_AXIS_LAST - 1] = -1.f;
			axes[GLFW_GAMEPAD_AXIS_LAST] = -1.f;
		}
		void Cleanup() {
			jid = -1;
			name.clear();
			ClearValues();
		}
	};

}
