#pragma once
#include "xx_gamebase_ex.h"

namespace xx {

	// example: struct Game : xx::Game<Game> { ...
	template<typename Derived, typename BaseType = GameBaseEx>
	struct Game : BaseType {
		sf::Window* wnd{};
		sf::ContextSettings contextSettings;

		void GLInit() {
			this->ShaderInit();

#ifndef __EMSCRIPTEN__
			glEnable(GL_PRIMITIVE_RESTART);
			glPrimitiveRestartIndex(65535);
#endif
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glViewport(0, 0, (int)this->windowSize.x, (int)this->windowSize.y);
		}

		void GLLoop(bool fromEvent) {
			glClearColor(this->clearColor.r / 255.f, this->clearColor.g / 255.f
				, this->clearColor.b / 255.f, this->clearColor.a / 255.f);
			glClear(GL_COLOR_BUFFER_BIT);

			this->blend = this->blendDefault;
			glBlendFunc(this->blendDefault[0], this->blendDefault[1]);
			glBlendEquation(this->blendDefault[2]);

			this->delta = NowSteadyEpochSeconds(this->time);
			this->drawVerts = {};
			this->drawCall = {};

			if constexpr (Has_Task<Derived>) {
				this->baseTask();													// lifeCycle 6 ( loop )
			}
			if constexpr (Has_Update<Derived>) {
				((Derived*)this)->Update();											// lifeCycle 7 ( loop )
			}

			this->ShaderEnd();

			this->drawFPSTimePool += this->delta;
			++this->drawFPS;
			if (this->drawFPSTimePool >= 1.f) {
				if constexpr (Has_Stat<Derived>) {
					((Derived*)this)->Stat();
				}
				this->drawFPSTimePool = {};
				this->drawFPS = {};
			}

			this->wnd->display();

			if (!fromEvent) {
				if constexpr (Has_Delay<Derived>) {
					((Derived*)this)->Delay();										// lifeCycle 8 ( loop )
				}
			}
		}

		XX_INLINE void StoreWindowSize() {
			auto ws = wnd->getSize();
			this->windowSize.x = (float)ws.x;
			this->windowSize.y = (float)ws.y;
		}

		XX_INLINE void AssignWindowSize() {
			this->windowSizeBackup = this->windowSize;
			glViewport(0, 0, (GLsizei)this->windowSize.x, (GLsizei)this->windowSize.y);
			this->ResizeCalc();
		}

		int32_t Run() {
			this->BaseInit();														// lifeCycle 1

			if constexpr (Has_Init<Derived>) {
				((Derived*)this)->Init();											// lifeCycle 2
			}

			contextSettings.depthBits = 24;
			contextSettings.majorVersion = 4;
			contextSettings.minorVersion = 1;
			contextSettings.attributeFlags = sf::ContextSettings::Core;

			sf::Window window(sf::VideoMode((uint32_t)this->windowSize.x, (uint32_t)this->windowSize.y)
				, sf::String((sf::Uint32*)this->title.data()), sf::Style::Default, contextSettings);
			if (!window.setActive()) return -1;

			this->wnd = &window;
			gladLoadGL(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));

			StoreWindowSize();
			AssignWindowSize();

			this->GLInit();															// lifeCycle 3
			if constexpr (Has_GLInit<Derived>) {
				((Derived*)this)->GLInit();											// lifeCycle 4
			}
			this->time = NowSteadyEpochSeconds();
			this->delta = 0.00001;

			if constexpr (Has_Task<Derived>) {
				this->baseTask = ((Derived*)this)->Task();							// lifeCycle 5
			}

#ifdef WIN32
			// to solve the stuck when dragging/resizing windows
			contextSettings.onDraw = [this] { this->GLLoop(true); };
#endif

			while (window.isOpen()) {
				sf::Event event;
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed ||
						(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
						Close();
						goto LabEnd;
					} else if (event.type == sf::Event::Resized) {
						StoreWindowSize();
					}
					else {
						// todo: keyboard, mouse? event ?
					}
				}
				if (this->windowSize != this->windowSizeBackup) {
					AssignWindowSize();
					if constexpr (Has_OnResize<Derived>) {
						((Derived*)this)->OnResize();
					}
				}
				this->GLLoop(false);
			}

		LabEnd:
			return EXIT_SUCCESS;
		}

		/****************************************************************************/
		// utils

		XX_INLINE void SetVerticalSyncEnabled(bool enabled_) {
			wnd->setVerticalSyncEnabled(enabled_);
		}

		XX_INLINE void SetFramerateLimit(uint32_t limit_) {
			wnd->setFramerateLimit(limit_);
		}

		XX_INLINE void Close() {
#ifdef WIN32
			// to solve the stuck when dragging/resizing windows
			contextSettings.onDrawHolder.reset();
#endif
			wnd->close();
		}

	};
}
