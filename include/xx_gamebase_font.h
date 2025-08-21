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

		void HandleEvent(sf::Event& e) {
			if (e.type == sf::Event::MouseMoved) {

				// left-top: 0,0 to center 0,0 gl pos
				mousePos = { e.mouseMove.x - worldMaxXY.x, worldMaxXY.y - e.mouseMove.y };

				if (uiHandler) {
					uiHandler->OnMouseMove();
				}
				else {
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
						if (!((MouseEventHandlerNode*)zn.n)->PosInScissor(mousePos)) continue;
						((MouseEventHandlerNode*)zn.n)->OnMouseMove();
						if (uiHandler) break;
					}
					tmpZNodes.Clear();
				}
			}
			else if (e.type == sf::Event::MouseButtonPressed) {
				auto btnId = (int)e.mouseButton.button;
				if (mouseBtns[btnId]) return;  // for duplicate message bug
				bool handled{};
				if (uiHandler) {
					uiHandler->OnMouseDown();
					handled = true;
				}
				else {
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

					for (auto& zn : tmpZNodes) {
						if (!((MouseEventHandlerNode*)zn.n)->PosInScissor(mousePos)) continue;
						((MouseEventHandlerNode*)zn.n)->OnMouseDown();
						handled = true;
						if (uiHandler) break;
					}
					tmpZNodes.Clear();
				}
				if (!handled) {
					mouseBtns[btnId] = true;
				}

			}
			else if (e.type == sf::Event::MouseButtonReleased) {
				auto btnId = (int)e.mouseButton.button;
				mouseBtns[btnId] = false;
				if (uiHandler) {
					uiHandler->OnMouseUp();
					//uiHandler.Reset();
				}
			}
		}

	};
}
