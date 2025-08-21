#pragma once
#include "xx_gamebase_font.h"
#include "xx_ui_ext.h"

namespace xx {
	// derived member checkers
	template<typename T> concept Has_Init = requires(T t) { { t.Init() } -> std::same_as<void>; };
	template<typename T> concept Has_GLInit = requires(T t) { { t.GLInit() } -> std::same_as<void>; };
	template<typename T> concept Has_Task = requires(T t) { { t.Task() } -> std::same_as<xx::Task<>>; };
	template<typename T> concept Has_Update = requires(T t) { { t.Update() } -> std::same_as<void>; };
	template<typename T> concept Has_Delay = requires(T t) { { t.Delay() } -> std::same_as<void>; };
	template<typename T> concept Has_Stat = requires(T t) { { t.Stat() } -> std::same_as<void>; };
	template<typename T> concept Has_OnResize = requires(T t) { { t.OnResize() } -> std::same_as<void>; };


	// for game scene logic
	struct SceneBase {
		virtual ~SceneBase() {}
		virtual void Update() {};
		virtual void Draw() {};
		virtual void OnResize() {};
	};


	// example: struct Game : xx::Game<Game> { ...
	template<typename Derived, typename BaseType = GameBase_font>
	struct Game : BaseType {
		sf::ContextSettings contextSettings;

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

			this->BaseGLInit();														// lifeCycle 3
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
					if (event.type == sf::Event::Closed
						|| (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
						)
					{
						Close();
						goto LabEnd;
					} else if (event.type == sf::Event::Resized) {
						StoreWindowSize();
					}
					else {
						this->HandleEvent(event);
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

		void GLLoop(bool fromEvent) {
			this->GLClear(this->clearColor);
			this->GLBlendFunc();

			this->delta = NowSteadyEpochSeconds(this->time);
			this->drawVerts = {};
			this->drawCall = {};

			if constexpr (Has_Task<Derived>) {
				this->baseTask();													// lifeCycle 6 ( loop )
			}
			if constexpr (Has_Update<Derived>) {
				((Derived*)this)->Update();											// lifeCycle 7 ( loop )
			}

			this->ExecuteDelayFuncs();												// lifeCycle 8 ( loop )

			this->ShaderEnd();

			this->drawFPSTimePool += this->delta;
			++this->drawFPS;
			if (this->drawFPSTimePool >= 1.f) {
				if constexpr (Has_Stat<Derived>) {
					((Derived*)this)->Stat();										// lifeCycle 9 ( loop )
				}
				this->drawFPSTimePool = {};
				this->drawFPS = {};
			}

			this->wnd->display();

			if (!fromEvent) {
				if constexpr (Has_Delay<Derived>) {
					((Derived*)this)->Delay();										// lifeCycle 10 ( loop )
				}
			}
		}

		XX_INLINE void StoreWindowSize() {
			auto ws = this->wnd->getSize();
			this->windowSize.x = (float)ws.x;
			this->windowSize.y = (float)ws.y;
		}

		XX_INLINE void AssignWindowSize() {
			this->windowSizeBackup = this->windowSize;
			this->GLViewport();
			this->ResizeCalc();
		}

		/****************************************************************************/
		// utils

		XX_INLINE void SetVerticalSyncEnabled(bool enabled_) {
			this->wnd->setVerticalSyncEnabled(enabled_);
		}

		XX_INLINE void SetFramerateLimit(uint32_t limit_) {
			this->wnd->setFramerateLimit(limit_);
		}

		XX_INLINE void Close() {
			this->wnd->close();
		}

	};
}
