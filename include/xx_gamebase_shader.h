#pragma once
#include "xx_gamebase.h"
#include "xx_shader_quad.h"

namespace xx {

	// shader support
	struct alignas(8) GameBase_shader : GameBase {
		inline static GameBase_shader* GetInstance() {
			return (GameBase_shader*)GameBase::instance;
		}

		Shader_Quad shaderQuad;
		XX_INLINE Shader_Quad& Quad() { return ShaderBegin(shaderQuad); }
		// ...

		void BaseGLInit() {
			GameBase::BaseGLInit();
			shaderQuad.Init();
			// ...
		}
	};

}
