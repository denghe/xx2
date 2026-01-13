#pragma once
#include "xx_ui_scale9.h"
#include "xx_gamebase.h"

namespace xx {

	Scale9& Scale9::Init(int32_t z_, XY position_, XY anchor_, XY size_
		, Shared<Scale9Config> cfg_) {
		assert(typeId == cTypeId);
		Node::InitDerived<Scale9>(z_, position_, anchor_, 1, size_);
		cfg = *cfg_;
		return *this;
	}

	Scale9& Scale9::SetConfig(Shared<Scale9Config> cfg_) {
		cfg = *cfg_;
		return *this;
	}

	void Scale9::Draw() {
		// calc
		auto& rect = cfg.frame.uvRect;
		auto& center = cfg.center;
		auto& color = cfg.color;
		auto& frame = cfg.frame;

		uint16_t tx1 = rect.x + 0;
		uint16_t tx2 = rect.x + center.x;
		uint16_t tx3 = rect.x + center.x + center.w;

		uint16_t ty1 = rect.y + 0;
		uint16_t ty2 = rect.y + center.y;
		uint16_t ty3 = rect.y + center.y + center.h;

		uint16_t tw1 = center.x;
		uint16_t tw2 = center.w;
		uint16_t tw3 = rect.w - (center.x + center.w);

		uint16_t th1 = center.y;
		uint16_t th2 = center.h;
		uint16_t th3 = rect.h - (center.y + center.h);

		XY ts{ cfg.textureScale * worldScale };

		float sx = float(worldSize.x - tw1 * ts.x - tw3 * ts.x) / tw2;
		float sy = float(worldSize.y - th1 * ts.y - th3 * ts.y) / th2;
#ifndef NDEBUG
		if (sx < 0 || sy < 0) {
			printf(" sx = %f, sy = %f, ts = {%f, %f}", sx, sy, ts.x, ts.y);
			assert(false);
		}
#endif

		float px1 = 0;
		float px2 = tw1 * ts.x;
		float px3 = worldSize.x - tw3 * ts.x;

		float py1 = worldSize.y;
		float py2 = worldSize.y - (th1 * ts.y);
		float py3 = worldSize.y - (worldSize.y - th3 * ts.y);

		auto& basePos = worldMinXY;

		RGBA8 c = { color.r, color.g, color.b, (uint8_t)(color.a * alpha) };
		float cp;
		if (enabled) cp = 1.f;
		else cp = 0.5f;

		// draw
		auto qs = GameBase::instance->Quad().Alloc(*frame.tex, 9);
		auto q = &qs[0];
		q->pos = basePos + XY{ px1, py1 };
		q->anchor = { 0, 1 };
		q->scale = ts;
		q->radians = {};
		q->colorplus = cp;
		q->color = c;
		q->texRect = { tx1, ty1, tw1, th1 };

		q = &qs[1];
		q->pos = basePos + XY{ px2, py1 };
		q->anchor = { 0, 1 };
		q->scale = { sx, ts.y };
		q->radians = {};
		q->colorplus = cp;
		q->color = c;
		q->texRect = { tx2, ty1, tw2, th1 };

		q = &qs[2];
		q->pos = basePos + XY{ px3, py1 };
		q->anchor = { 0, 1 };
		q->scale = ts;
		q->radians = {};
		q->colorplus = cp;
		q->color = c;
		q->texRect = { tx3, ty1, tw3, th1 };

		q = &qs[3];
		q->pos = basePos + XY{ px1, py2 };
		q->anchor = { 0, 1 };
		q->scale = { ts.x, sy };
		q->radians = {};
		q->colorplus = cp;
		q->color = c;
		q->texRect = { tx1, ty2, tw1, th2 };

		q = &qs[4];
		q->pos = basePos + XY{ px2, py2 };
		q->anchor = { 0, 1 };
		q->scale = { sx, sy };
		q->radians = {};
		q->colorplus = cp;
		q->color = c;
		q->texRect = { tx2, ty2, tw2, th2 };

		q = &qs[5];
		q->pos = basePos + XY{ px3, py2 };
		q->anchor = { 0, 1 };
		q->scale = { ts.x, sy };
		q->radians = {};
		q->colorplus = cp;
		q->color = c;
		q->texRect = { tx3, ty2, tw3, th2 };

		q = &qs[6];
		q->pos = basePos + XY{ px1, py3 };
		q->anchor = { 0, 1 };
		q->scale = ts;
		q->radians = {};
		q->colorplus = cp;
		q->color = c;
		q->texRect = { tx1, ty3, tw1, th3 };

		q = &qs[7];
		q->pos = basePos + XY{ px2, py3 };
		q->anchor = { 0, 1 };
		q->scale = { sx, ts.y };
		q->radians = {};
		q->colorplus = cp;
		q->color = c;
		q->texRect = { tx2, ty3, tw2, th3 };

		q = &qs[8];
		q->pos = basePos + XY{ px3, py3 };
		q->anchor = { 0, 1 };
		q->scale = ts;
		q->radians = {};
		q->colorplus = cp;
		q->color = c;
		q->texRect = { tx3, ty3, tw3, th3 };
	}

}
