#pragma once
#include "xx_includes.h"
#include "xx_glfw.h"

namespace xx {

    // for mouse, keyboard, joystick
    struct BtnState {
        int32_t pressed{};
        float nextActiveTime{};
        float* globalTime{};		// == &GameBase.time

        operator bool() const;  // return pressed
        void Press();   // pressed = true
        void Release(); // pressed = false
        bool Once(); // return pressed & release
        bool operator()(float interval_); // handle lastActivedTime
    };

    struct JoyState {
        int32_t jid{ -1 };
        std::string name;

        //	A, B, X, Y, L1, R1, Back, Start, Home, L2, R2, Up, Right, Down, Left
        std::array<BtnState, GLFW_GAMEPAD_BUTTON_LAST + 1> btns{};

        // AxisLeft H V, AxisRight H V, Trigger L R
        std::array<float, GLFW_GAMEPAD_AXIS_LAST + 1> axes{};

        void Init(float* globalTime_);
        void ClearValues();
        void Cleanup();
    };

}
