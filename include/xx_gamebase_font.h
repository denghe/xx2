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

			// refresh all joystick data
			GLFWgamepadstate gs;
			int32_t maxI{}, maxRank{};
			for (int32_t i = 0; i < joys.len; ++i) {
				auto& joy = joys[i];
				auto jid = joy.jid;
				joy.rank = 0;
				if (glfwGetGamepadState(jid, &gs)) {
					for (auto i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; i++) {
						if (gs.buttons[i]) {
							joy.btns[i].Press(time);
							joy.rank++;
						}
						else {
							joy.btns[i].Release();
						}
					}
					for (auto i = 0; i < GLFW_GAMEPAD_AXIS_LAST - 1; i++) {
						auto v = gs.axes[i];
						if (std::abs(v) < 0.08f) joy.axes[i] = 0.f;		// 0.08f: death zone
						else {
							joy.axes[i] = v;
							joy.rank++;
						}
					}
					for (auto i = GLFW_GAMEPAD_AXIS_LAST - 1; i <= GLFW_GAMEPAD_AXIS_LAST; i++) {
						auto v = gs.axes[i];
						if (v < -0.9f) joy.axes[i] = -1.f;		// 0.9f: death zone
						else {
							joy.axes[i] = v;
							joy.rank++;
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
				else {
					joy.ClearValues();
				}
				if (joy.rank > maxRank) {
					maxRank = joy.rank;
					maxI = i;
				}
			}

			// choose one copy to joy( max rank or current )
			if (joys.len) {
				if (maxRank == 0) {
					if (lastJoyIdx == -1 || lastJoyIdx >= joys.len) {
						lastJoyIdx = 0;
					}
				}
				else {
					lastJoyIdx = maxI;
				}
			}
			else {
				lastJoyIdx = -1;
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
