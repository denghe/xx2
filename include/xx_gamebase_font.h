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
			this->wnd->display();
		}

		// todo: forground auto pause?
		void HandleEvent(sf::Event& e) {
			switch (e.type) {
			case sf::Event::LostFocus: {
				focused = false;
				break;
			}
			case sf::Event::GainedFocus: {
				focused = true;
				break;
			}
			case sf::Event::TextEntered: {
				// todo: use timeout queue store ?
				break;
			}
			case sf::Event::KeyPressed: {
				if (e.key.code == sf::Keyboard::Unknown) break;
				keyboard[e.key.code].Press(time);
				break;
			}
			case sf::Event::KeyReleased: {
				if (e.key.code == sf::Keyboard::Unknown) break;
				keyboard[e.key.code].Release();
				break;
			}
			case sf::Event::MouseWheelMoved:
				// deprecated
				break;
			case sf::Event::MouseWheelScrolled: {
				if (e.mouseWheelScroll.wheel == 0) {
					if (e.mouseWheelScroll.delta >= 0) {
						mouse[5].Press(time);
						wheelTimeouts[5] = time + 0.05f;
					}
					else {
						mouse[6].Press(time);
						wheelTimeouts[6] = time + 0.05f;
					}
				}
				else {
					if (e.mouseWheelScroll.delta >= 0) {
						mouse[7].Press(time);
						wheelTimeouts[7] = time + 0.05f;
					}
					else {
						mouse[8].Press(time);
						wheelTimeouts[8] = time + 0.05f;
					}
				}
				break;
			}
			case sf::Event::MouseButtonPressed: {
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
					mouse[e.mouseButton.button].Press(time);
				}
				break;
			}
			case sf::Event::MouseButtonReleased: {
				mouse[e.mouseButton.button].Release();
				break;
			}
			case sf::Event::MouseMoved: {
				// left-top: 0,0 to center 0,0 gl pos
				mousePos = { e.mouseMove.x - worldMaxXY.x, worldMaxXY.y - e.mouseMove.y };

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
				break;
			}
			case sf::Event::MouseEntered: {
				mouseInWindow = true;
				break;
			}
			case sf::Event::MouseLeft: {
				mouseInWindow = false;
				break;
			}
			case sf::Event::JoystickButtonPressed: {
				//xx::CoutN("joy id = ", e.joystickButton.joystickId, " pressed btn = ", e.joystickButton.button);

				// btn : AX  BO  X4  Y3   L1   R1   Map   Menu   AxisLeft   AxisRight   PAD   PS
				// xbox: 0   1   2   3    4    5    6     7      8          9
				// ps  : 1   2   0   3    4    5    8     9      10         11          13    12
				// axis: AxisLeft   AxisRight   L2      R2      4way
				// xbox: h0 v1      h4 v5       2+      2-      h6 v7
				// ps  : h0 v1      h2 v3       4       5       h6 v7
				// h0 h6: left -100, right 100   v1: up -100 down 100   h7: up 100 down -100

				// xbox:
				// some btn id disable: 8, 9
				// some axis map to btn: 0 -> 8, 9   1-> 10, 11   6 -> 12, 13   7 -> 14, 15

				if (e.joystickButton.button >= 8) break;
				auto jid = (int32_t)e.joystickButton.joystickId;
				if (joys.len <= jid) {
					joys.Resize(jid + 1);
				}
				joys[jid][e.joystickButton.button].Press(time);
				joy[e.joystickButton.button].Press(time);
				break;
			}
			case sf::Event::JoystickButtonReleased: {
				//xx::CoutN("joy id = ", e.joystickButton.joystickId, " released btn = ", e.joystickButton.button);
				if (e.joystickButton.button >= 8) break;
				auto jid = (int32_t)e.joystickButton.joystickId;
				if (joys.len <= jid) {
					joys.Resize(jid + 1);
				}
				joys[jid][e.joystickButton.button].Release();
				joy[e.joystickButton.button].Release();
				break;
			}
			case sf::Event::JoystickMoved: {
				//xx::CoutN("joy id = ", e.joystickButton.joystickId, " axis = ", e.joystickMove.axis, " pos = ", e.joystickMove.position);
				if (e.joystickMove.axis >= 8) break;
				auto jid = (int32_t)e.joystickMove.joystickId;
				if (joys.len <= jid) {
					joys.Resize(jid + 1);
				}
				if (joyas.len <= jid) {
					joyas.Resize(jid + 1);
				}
				joyas[jid][e.joystickButton.button] = e.joystickMove.position;
				joya[e.joystickButton.button] = e.joystickMove.position;

				switch (e.joystickMove.axis) {
				case 0:
					if (e.joystickMove.position < -20.f) {
						joys[jid][8].Press(time);	// left
						joy[8].Press(time);
					}
					else if (e.joystickMove.position > 20.f) {
						joys[jid][9].Press(time);	// right
						joy[9].Press(time);
					}
					else {
						joys[jid][8].Release();
						joy[8].Release();
						joys[jid][9].Release();
						joy[9].Release();
					}
					break;
				case 1:
					if (e.joystickMove.position < -20.f) {
						joys[jid][10].Press(time);	// up
						joy[10].Press(time);
					}
					else if (e.joystickMove.position > 20.f) {
						joys[jid][11].Press(time);	// down
						joy[11].Press(time);
					}
					else {
						joys[jid][10].Release();
						joy[10].Release();
						joys[jid][11].Release();
						joy[11].Release();
					}
					break;
				case 6:
					if (e.joystickMove.position < -20.f) {
						joys[jid][12].Press(time);	// left
						joy[12].Press(time);
					}
					else if (e.joystickMove.position > 20.f) {
						joys[jid][13].Press(time);	// right
						joy[13].Press(time);
					}
					else {
						joys[jid][12].Release();
						joy[12].Release();
						joys[jid][13].Release();
						joy[13].Release();
					}
					break;
				case 7:
					if (e.joystickMove.position > 20.f) {
						joys[jid][14].Press(time);	// up
						joy[14].Press(time);
					}
					else if (e.joystickMove.position < -20.f) {
						joys[jid][15].Press(time);	// down
						joy[15].Press(time);
					}
					else {
						joys[jid][14].Release();
						joy[14].Release();
						joys[jid][15].Release();
						joy[15].Release();
					}
					break;
				default:;
				}
				break;
			}
			case sf::Event::JoystickConnected:
			case sf::Event::JoystickDisconnected: {
				auto jid = (int32_t)e.joystickConnect.joystickId;
				if (joys.len <= jid) {
					joys.Resize(jid + 1);
				}
				if (joyas.len <= jid) {
					joyas.Resize(jid + 1);
				}
				memset(&joys[jid], 0, sizeof(joys[0]));
				memset(&joyas[jid], 0, sizeof(joyas[0]));
				memset(&joy, 0, sizeof(joy));
				memset(&joya, 0, sizeof(joya));
				break;
			}
			default:
				CoutN("unhandled event: ", e.type);
				break;
			}
		}

		void BaseUpdate() {
			// event timeout handles
			for (int32_t i = 5; i < 9; ++i) {
				if (time < wheelTimeouts[i]) {
					mouse[i].Press(time);
				}
				else {
					mouse[i].Release();
				}
			}
		}

	};
}
