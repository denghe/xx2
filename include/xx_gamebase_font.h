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
			else if (e.type == sf::Event::MouseButtonPressed) {
				auto btnId = (int)e.mouseButton.button;
				if (mouseBtns[btnId]) return;  // for duplicate message bug

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
					mouseBtns[btnId] = true;
				}

			}
			else if (e.type == sf::Event::MouseButtonReleased) {
				auto btnId = (int)e.mouseButton.button;
				mouseBtns[btnId] = false;
			}
		}

	};
}
