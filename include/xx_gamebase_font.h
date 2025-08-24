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
				keyboardBtns[e.key.code].Press(time);
				break;
			}
			case sf::Event::KeyReleased: {
				if (e.key.code == sf::Keyboard::Unknown) break;
				keyboardBtns[e.key.code].Release();
				break;
			}
			case sf::Event::MouseWheelMoved:
				// deprecated
				break;
			case sf::Event::MouseWheelScrolled: {
				if (e.mouseWheelScroll.wheel == 0) {
					if (e.mouseWheelScroll.delta >= 0) {
						mouseBtns[5].Press(time);
						wheelTimeouts[5] = time + 0.05f;
					}
					else {
						mouseBtns[6].Press(time);
						wheelTimeouts[6] = time + 0.05f;
					}
				}
				else {
					if (e.mouseWheelScroll.delta >= 0) {
						mouseBtns[7].Press(time);
						wheelTimeouts[7] = time + 0.05f;
					}
					else {
						mouseBtns[8].Press(time);
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
					mouseBtns[e.mouseButton.button].Press(time);
				}
				break;
			}
			case sf::Event::MouseButtonReleased: {
				mouseBtns[e.mouseButton.button].Release();
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
				xx::CoutN("joy id = ", e.joystickButton.joystickId, " pressed btn = ", e.joystickButton.button);
				// todo
				break;
			}
			case sf::Event::JoystickButtonReleased: {
				xx::CoutN("joy id = ", e.joystickButton.joystickId, " released btn = ", e.joystickButton.button);
				// todo
				break;
			}
			case sf::Event::JoystickMoved: {
				xx::CoutN("joy id = ", e.joystickButton.joystickId, " axis = ", e.joystickMove.axis, " pos = ", e.joystickMove.position);
				// todo
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
					mouseBtns[i].Press(time);
				}
				else {
					mouseBtns[i].Release();
				}
			}
		}

	};
}
