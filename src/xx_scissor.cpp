#pragma once
#include "xx_scissor.h"
#include "xx_gamebase.h"

namespace xx {

	void Scissor::DirectBegin(float x, float y, float w, float h) {
		auto& eb = *GameBase::instance;
		eb.ShaderEnd();
		bakBlend = eb.blend;
		eb.GLBlendFunc(eb.blendDefault);
		X_Y<GLsizei> wp{ GLsizei(eb.worldMaxXY.x + x), GLsizei(eb.worldMaxXY.y + y) };
		glScissor(wp.x, wp.y, (GLsizei)w, (GLsizei)h);
		glEnable(GL_SCISSOR_TEST);
	}

	void Scissor::DirectEnd() {
		auto& eb = *GameBase::instance;
		eb.ShaderEnd();
		glDisable(GL_SCISSOR_TEST);
		eb.GLBlendFunc(bakBlend);
	}


	void Scissor::OffsetBegin(float x, float y, float w, float h) {
		auto& eb = *GameBase::instance;
		eb.ShaderEnd();
		bakWndSiz = eb.windowSize;
		bakBlend = eb.blend;
		X_Y<GLsizei> wp{ GLsizei(eb.worldMaxXY.x + x), GLsizei(eb.worldMaxXY.y + y) };
		eb.SetWindowSize({ w, h });
		eb.GLBlendFunc(eb.blendDefault);
		glViewport(wp.x, wp.y, (GLsizei)w, (GLsizei)h);
		glScissor(wp.x, wp.y, (GLsizei)w, (GLsizei)h);
		glEnable(GL_SCISSOR_TEST);
	}

	void Scissor::OffsetEnd() {
		auto& eb = *GameBase::instance;
		eb.ShaderEnd();
		glDisable(GL_SCISSOR_TEST);
		eb.SetWindowSize(bakWndSiz);
		eb.GLViewport();
		eb.GLBlendFunc(bakBlend);
	}

}
