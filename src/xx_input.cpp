#include "xx_input.h"

namespace xx {

    BtnState::operator bool() const {
        return pressed;
    }

    void BtnState::Press() {
        pressed = true;
        nextActiveTime = {};
    }

    void BtnState::Release() {
        pressed = false;
        nextActiveTime = {};
    }

    bool BtnState::Once() {
        if (pressed) {
            Release();
            return true;
        }
        return false;
    }

    bool BtnState::operator()(float interval_) {
        if (pressed) {
            if (auto t = float(*globalTime); nextActiveTime <= t) {
                nextActiveTime = t + interval_;
                return true;
            }
        }
        return false;
    }





    void JoyState::Init(float* globalTime_) {
        for (auto& o : btns) {
            o.globalTime = globalTime_;
        }
    }

    void JoyState::ClearValues() {
        for (auto& btn : btns) {
            btn.pressed = 0;
            //btn.lastPressedTime = 0;
        }
        memset(&axes, 0, sizeof(float) * GLFW_GAMEPAD_AXIS_LAST - 1);
        axes[GLFW_GAMEPAD_AXIS_LAST - 1] = -1.f;
        axes[GLFW_GAMEPAD_AXIS_LAST] = -1.f;
    }
    void JoyState::Cleanup() {
        jid = -1;
        name.clear();
        ClearValues();
    }


}
