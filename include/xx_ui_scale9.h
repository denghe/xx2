#pragma once
#include "xx_gamebase_ui.h"

namespace xx {

	struct Scale9 : Node {
		static constexpr int32_t cTypeId{ 3 };

		TinyFrame frame;
		UVRect center;
		RGBA8 color;
		XY texScale;
		float colorplus;

		Scale9& Init(int z_, XY position_, XY anchor_, XY scale_, XY size_, Scale9Config const& cfg_, float colorplus_ = 1) {
			Node::Init(z_, position_, anchor_, scale_, size_);
			typeId = cTypeId;
			frame = cfg_.frame;
			center = cfg_.center;
			color = cfg_.color;
			texScale = cfg_.texScale;
			colorplus = colorplus_;
			return *this;
		}

		Scale9& Init(int z_, XY position_, XY anchor_, XY size_) {
			auto& cfg = *GameBase_ui::GetInstance()->defaultCfg.s9bg;
			return Init(z_, position_, cfg.borderScale, anchor_, size_ / cfg.borderScale, cfg);
		}

		virtual void Draw() override {
			// calc
			auto& r = frame.textureRect;

			uint16_t tx1 = r.x + 0;
			uint16_t tx2 = r.x + center.x;
			uint16_t tx3 = r.x + center.x + center.w;

			uint16_t ty1 = r.y + 0;
			uint16_t ty2 = r.y + center.y;
			uint16_t ty3 = r.y + center.y + center.h;

			uint16_t tw1 = center.x;
			uint16_t tw2 = center.w;
			uint16_t tw3 = r.w - (center.x + center.w);

			uint16_t th1 = center.y;
			uint16_t th2 = center.h;
			uint16_t th3 = r.h - (center.y + center.h);

			XY ts{ texScale * worldScale };

			float sx = float(worldSize.x - tw1 * ts.x - tw3 * ts.x) / tw2;
			float sy = float(worldSize.y - th1 * ts.y - th3 * ts.y) / th2;
#ifndef NDEBUG
			if (sx < 0 || sy < 0) {
				CoutN(" sx = ", sx, " sy = ", sy, " ts = ", ts);
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

			// draw
			auto qs = GameBase_shader::GetInstance()->Quad().Alloc(*frame.tex, 9);
			auto q = &qs[0];
			q->pos = basePos + XY{ px1, py1 };
			q->anchor = { 0, 1 };
			q->scale = ts;
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx1, ty1, tw1, th1 };

			q = &qs[1];
			q->pos = basePos + XY{ px2, py1 };
			q->anchor = { 0, 1 };
			q->scale = { sx, ts.y };
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx2, ty1, tw2, th1 };

			q = &qs[2];
			q->pos = basePos + XY{ px3, py1 };
			q->anchor = { 0, 1 };
			q->scale = ts;
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx3, ty1, tw3, th1 };

			q = &qs[3];
			q->pos = basePos + XY{ px1, py2 };
			q->anchor = { 0, 1 };
			q->scale = { ts.x, sy };
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx1, ty2, tw1, th2 };

			q = &qs[4];
			q->pos = basePos + XY{ px2, py2 };
			q->anchor = { 0, 1 };
			q->scale = { sx, sy };
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx2, ty2, tw2, th2 };

			q = &qs[5];
			q->pos = basePos + XY{ px3, py2 };
			q->anchor = { 0, 1 };
			q->scale = { ts.x, sy };
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx3, ty2, tw3, th2 };

			q = &qs[6];
			q->pos = basePos + XY{ px1, py3 };
			q->anchor = { 0, 1 };
			q->scale = ts;
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx1, ty3, tw1, th3 };

			q = &qs[7];
			q->pos = basePos + XY{ px2, py3 };
			q->anchor = { 0, 1 };
			q->scale = { sx, ts.y };
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx2, ty3, tw2, th3 };

			q = &qs[8];
			q->pos = basePos + XY{ px3, py3 };
			q->anchor = { 0, 1 };
			q->scale = ts;
			q->radians = {};
			q->colorplus = colorplus;
			q->color = c;
			q->texRect = { tx3, ty3, tw3, th3 };
		}
	};

}
