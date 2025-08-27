#pragma once
#include "xx_gamebase_shader.h"
#include "xx_fontmaker.h"
#include "xx_node.h"

namespace xx {
	// font support
	struct GameBase_font : GameBase_shader {
		inline static GameBase_font* GetInstance() {
			return (GameBase_font*)GameBase::instance;
		}

		FontMaker fontMaker;	// for Label
		List<ZNode> tmpZNodes;	// for node tree sort

		// for easy draw root node only
		void DrawNode(Node* tar) {
			FillZNodes(tmpZNodes, tar);
			OrderByZDrawAndClear(tmpZNodes);
		}

		void BaseGLInit() {
			GameBase_shader::BaseGLInit();
			fontMaker.Init();
			// search exists joys
			for (auto jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++) {
				if (glfwJoystickPresent(jid)) {
					this->HandleJoystickConnected(jid);
				}
			}
		}

		void HandleMouseMove(XY mp_) {
			mousePos = mp_;

			// search
			uiGrid.ForeachPoint(uiGrid.worldSize * 0.5f + mousePos);

			// search results -> tmpZNodes
			for (auto& i : uiGrid.results) {
				auto o = uiGrid.NodeAt(i).value;
				assert(o->typeId >= 10);
				tmpZNodes.Emplace(o->z, o);
			}

			// sort order by z
			std::sort(tmpZNodes.buf, tmpZNodes.buf + tmpZNodes.len
				, ZNode::GreaterThanComparer);

			// try dispatch
			for (auto& zn : tmpZNodes) {
				auto n = ((MouseEventHandlerNode*)zn.n);
				if (!n->PosInScissor(mousePos)) continue;
				if (n->OnMouseMove()) break;
			}
			tmpZNodes.Clear();
		}

		void HandleMouseButtonPressed(int32_t idx) {
			assert(!mouse[idx]);
			// search
			uiGrid.ForeachPoint(uiGrid.worldSize * 0.5f + mousePos);

			// search results -> tmpZNodes
			for (auto& i : uiGrid.results) {
				auto o = uiGrid.NodeAt(i).value;
				assert(o->typeId >= 10);
				tmpZNodes.Emplace(o->z, o);
			}

			// sort order by z
			std::sort(tmpZNodes.buf, tmpZNodes.buf + tmpZNodes.len
				, ZNode::GreaterThanComparer);

			bool handled{};
			for (auto& zn : tmpZNodes) {
				auto n = ((MouseEventHandlerNode*)zn.n);
				if (!n->PosInScissor(mousePos)) continue;
				if (n->OnMouseDown()) {
					handled = true;
					break;
				}
			}
			tmpZNodes.Clear();
			if (!handled) {
				mouse[idx].Press(time);
			}
		}

		void HandleMouseButtonReleased(int32_t idx) {
			mouse[idx].Release();
		}

		void HandleMouseWheel(double xoffset, double yoffset) {
			if (yoffset > 0) {
				mouse[GLFW_MOUSE_BUTTON_LAST + 1].Press(time);
				wheelTimeouts[0] = time + 0.05f;
			}
			else if (yoffset < 0) {
				mouse[GLFW_MOUSE_BUTTON_LAST + 2].Press(time);
				wheelTimeouts[1] = time + 0.05f;
			}
			if (xoffset > 0) {
				mouse[GLFW_MOUSE_BUTTON_LAST + 3].Press(time);
				wheelTimeouts[2] = time + 0.05f;
			}
			else if (xoffset < 0) {
				mouse[GLFW_MOUSE_BUTTON_LAST + 4].Press(time);
				wheelTimeouts[3] = time + 0.05f;
			}
		}

		void HandleKeyboardPressed(int32_t idx) {
			keyboard[idx].Press(time);
		}

		void HandleKeyboardReleased(int32_t idx) {
			keyboard[idx].Release();
		}

		void HandleJoystickConnected(int jid) {
			auto& joy = joys.Emplace();
			joy.jid = jid;
			joy.name = glfwGetGamepadName(jid);
		}

		void HandleJoystickDisconnected(int jid) {
			for (int i = 0; i < joys.len; ++i) {
				if (joys[i].jid == jid) {
					joys.RemoveAt(i);
					break;
				}
			}
		}

		void BaseUpdate() {

			// fill all joystick data
			joy.ClearValues();
			if (joys.len) {
				GLFWgamepadstate gs;
				for (int32_t i = 0; i < joys.len; ++i) {
					auto& j = joys[i];
					auto jid = j.jid;
					if (glfwGetGamepadState(jid, &gs)) {
						for (auto i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; i++) {
							if (gs.buttons[i]) {
								j.btns[i].Press(time);
								joy.btns[i].Press(time);
							}
							else {
								j.btns[i].Release();
							}
						}
						for (auto i = 0; i < GLFW_GAMEPAD_AXIS_LAST - 1; i++) {
							auto v = gs.axes[i];
							if (std::abs(v) < 0.08f) j.axes[i] = 0.f;	// 0.08f: death zone
							else {
								j.axes[i] = v;
								joy.axes[i] = v;
							}
						}
						for (auto i = GLFW_GAMEPAD_AXIS_LAST - 1; i <= GLFW_GAMEPAD_AXIS_LAST; i++) {
							auto v = gs.axes[i];
							if (v < -0.9f) j.axes[i] = -1.f;	// 0.9f: death zone
							else {
								j.axes[i] = v;
								joy.axes[i] = v;
							}
						}

						// LT -> L2, RT -> R2
						if (j.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > -0.5f) {
							j.btns[GLFW_GAMEPAD_BUTTON_LEFT_THUMB].Press(time);
						}
						else {
							j.btns[GLFW_GAMEPAD_BUTTON_LEFT_THUMB].Release();
						}
						if (j.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > -0.5f) {
							j.btns[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB].Press(time);
						}
						else {
							j.btns[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB].Release();
						}
					}
					else {
						j.ClearValues();
					}
				}

				// LT -> L2, RT -> R2
				if (joy.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > -0.5f) {
					joy.btns[GLFW_GAMEPAD_BUTTON_LEFT_THUMB].Press(time);
				}
				else {
					joy.btns[GLFW_GAMEPAD_BUTTON_LEFT_THUMB].Release();
				}
				if (joy.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > -0.5f) {
					joy.btns[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB].Press(time);
				}
				else {
					joy.btns[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB].Release();
				}
			}

			// timeout
			for (int32_t i = 0; i < 4; ++i) {
				if (time < wheelTimeouts[i]) {
					mouse[GLFW_MOUSE_BUTTON_LAST + 1 + i].Press(time);
				}
				else {
					mouse[GLFW_MOUSE_BUTTON_LAST + 1 + i].Release();
				}
			}
		}

	};
}
