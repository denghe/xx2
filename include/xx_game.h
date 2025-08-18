#pragma once
#include "xx_gamebase.h"
#include "xx_shader_quad.h"

namespace xx {

	struct GameBaseWithShader : GameBase {

		Shader_Quad shaderQuad;
		XX_INLINE Shader_Quad& Quad() { return ShaderBegin(shaderQuad); }
		// ...

		XX_INLINE void ShaderInit() {
			shaderQuad.Init();
			// ...
		}
	};

	// example: struct Game : xx::Game<Game> { void Init(); int32_t Update(); };
	// static constexpr float fps{ 120 }, frameDelay{ 1.f / fps }, maxFrameDelay{ 0.1f };
	template<typename Derived, typename BaseType = GameBaseWithShader>
	struct Game : BaseType {
		sf::Window* wnd{};
		float tmpDeltaPool{};
		int32_t tmpCounter{};

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

		void GLLoop() {
			glClearColor(this->clearColor.r / 255.f, this->clearColor.g / 255.f
				, this->clearColor.b / 255.f, this->clearColor.a / 255.f);
			glClear(GL_COLOR_BUFFER_BIT);

			this->blend = this->blendDefault;
			glBlendFunc(this->blendDefault[0], this->blendDefault[1]);
			glBlendEquation(this->blendDefault[2]);

			this->delta = NowSteadyEpochSeconds(this->time);
			tmpDeltaPool += this->delta;
			++tmpCounter;
			if (tmpDeltaPool >= 1.f) {
			if (this->showStat) {
				CoutN("fps = ", tmpCounter, " draw call = ", this->drawCall, " draw verts = ", this->drawVerts);
			}
			tmpDeltaPool = 0;
			tmpCounter = 0;
			}
			this->drawVerts = {};
			this->drawCall = {};

			if constexpr (Has_Task<Derived>) {
				this->baseTask();													// lifeCycle 6 ( loop )
			}
			if constexpr (Has_Loop<Derived>) {
				((Derived*)this)->Loop();											// lifeCycle 7 ( loop )
			}

			this->ShaderEnd();
			this->wnd->display();
		}

		int32_t Run() {
			this->BaseInit();														// lifeCycle 1 ( once )

			if constexpr (Has_Init<Derived>) {
				((Derived*)this)->Init();											// lifeCycle 2 ( once )
			}

			sf::ContextSettings contextSettings;
			contextSettings.depthBits = 24;
			contextSettings.majorVersion = 4;
			contextSettings.minorVersion = 1;
			contextSettings.attributeFlags = sf::ContextSettings::Core;

			sf::Window window(sf::VideoMode((uint32_t)this->windowSize.x, (uint32_t)this->windowSize.y)
				, sf::String((sf::Uint32*)this->title.data()), sf::Style::Default, contextSettings);
			this->wnd = &window;

			auto ws = window.getSize();
			this->windowSize.x = (float)ws.x;
			this->windowSize.y = (float)ws.y;

			window.setActive();

			gladLoadGL(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));

			this->GLInit();															// lifeCycle 3 ( once )
			if constexpr (Has_GLInit<Derived>) {
				((Derived*)this)->GLInit();											// lifeCycle 4 ( once )
			}

			if constexpr (Has_Task<Derived>) {
				this->baseTask = ((Derived*)this)->Task();							// lifeCycle 5 ( once )
			}

#ifdef WIN32
			contextSettings.onDraw = [this] { this->GLLoop(); };
#endif
			while (window.isOpen()) {
				sf::Event event;
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed
						|| (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
						window.close();
						goto LabEnd;
					}
					if (event.type == sf::Event::Resized) {
						auto ws = window.getSize();
						this->windowSize.x = (float)ws.x;
						this->windowSize.y = (float)ws.y;
						glViewport(0, 0, (int)this->windowSize.x, (int)this->windowSize.y);
						// todo: call user func
					}
				}
				this->GLLoop();
			}
		LabEnd:
			return EXIT_SUCCESS;
		}
	};
}
