#pragma once
#include "xx_gamebase.h"
#include "xx_shader_quad.h"

namespace xx {
	// derived member checkers
	template<typename T> concept Has_Init = requires(T t) { { t.Init() } -> std::same_as<void>; };
	template<typename T> concept Has_GLInit = requires(T t) { { t.GLInit() } -> std::same_as<void>; };
	template<typename T> concept Has_Task = requires(T t) { { t.Task() } -> std::same_as<xx::Task<>>; };
	template<typename T> concept Has_Update = requires(T t) { { t.Update() } -> std::same_as<void>; };
	template<typename T> concept Has_Delay = requires(T t) { { t.Delay() } -> std::same_as<void>; };
	template<typename T> concept Has_Stat = requires(T t) { { t.Stat() } -> std::same_as<void>; };
	template<typename T> concept Has_OnResize = requires(T t) { { t.OnResize() } -> std::same_as<void>; };


	// add more features
	struct alignas(8) GameBaseEx : GameBase {
		Shader_Quad shaderQuad;
		XX_INLINE Shader_Quad& Quad() { return ShaderBegin(shaderQuad); }
		// ...

		XX_INLINE void BaseGLInit() {
#ifndef __EMSCRIPTEN__
			glEnable(GL_PRIMITIVE_RESTART);
			glPrimitiveRestartIndex(65535);
#endif
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);

			shaderQuad.Init();
			// ...
		}
	};


	// for game scene logic
	struct SceneBase {
		virtual ~SceneBase() {}
		virtual void Update() {};
		virtual void Draw() {};
		virtual void OnResize() {};
	};
}
