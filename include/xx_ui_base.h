#pragma once
#include "xx_node.h"

namespace xx {

	struct Label : Node {
		static constexpr int32_t cTypeId{ 1 };

		List<FontFrame> fs;
		RGBA8 color{};

		Label& Init(int z_, XY position_, XY anchor_ = 0, XY scale_ = 1, RGBA8 color_ = RGBA8_White) {
			typeId = cTypeId;
			z = z_;
			position = position_;
			anchor = anchor_;
			scale = scale_;
			color = color_;
			return *this;
		}

		// S : literal string u8/32string [view]
		template<typename S>
		void SetText(S const& txt_) {
			auto len = (int32_t)StrLen(txt_);
			if (!len) {
				fs.Clear();
				return;
			}
			fs.Resize(len);
			auto& fm = GameBase_font::GetInstance()->fontMaker;
			size = { 0, (float)fm.canvasSize.y };
			for (int i = 0; i < len; ++i) {
				fs[i] = fm[txt_[i]];
				size.x += fs[i].textureRect.w;
			}
			FillTrans();
		}

		void SetText() {
			fs.Clear();
		}

		virtual void Draw() override {
			if (fs.Empty()) return;
			auto& q = GameBase_shader::GetInstance()->Quad();
			auto basePos = worldMinXY;
			for (auto& f : fs) {
				q.Draw(f.texId, f.textureRect, basePos, 0, worldScale, 0, 1, color);
				basePos.x += f.textureRect.w * worldScale.x;
			}
		}
	};

	enum class ImageRadians : int32_t {
		Zero = 0,		// 0'
		PiDiv2 = 1,		// 90'
		Pi = 2,			// 180'
		NegPiDiv2 = -1,	// -90'
		NegPi = -2		// -180'
	};

	struct Image : Node {
		static constexpr int32_t cTypeId{ 2 };

		TinyFrame frame;
		XY fixedScale{};
		RGBA8 color{};
		float colorplus{};
		float radians{};

		Image& Init(int z_, XY position_, XY anchor_, XY fixedSize_, bool keepAspect_
			, TinyFrame frame_, ImageRadians radians_ = ImageRadians::Zero, RGBA8 color_ = RGBA8_White, float colorplus_ = 1) {
			typeId = cTypeId;
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_;
			frame = std::move(frame_);
			radians = float(M_PI) * 0.5f * (int32_t)radians_;
			color = color_;
			colorplus = colorplus_;

			if (keepAspect_) {
				auto s = fixedSize_.x / frame.textureRect.w;
				if (frame.textureRect.h * s > fixedSize_.y) {
					s = fixedSize_.y / frame.textureRect.h;
				}
				fixedScale = s;
			}
			else {
				fixedScale.x = fixedSize_.x / frame.textureRect.w;
				fixedScale.y = fixedSize_.y / frame.textureRect.h;
			}

			FillTrans();
			return *this;
		}

		void Draw() override {
			if (!frame.tex) return;
			GameBase_shader::GetInstance()->Quad().Draw(*frame.tex, frame.textureRect, worldMinXY, 0
				, worldScale * fixedScale, radians, colorplus, { color.r, color.g, color.b, (uint8_t)(color.a * alpha) });
		}
	};

	struct Scale9Config {
		TinyFrame frame;
		XY texScale{ 1, 1 };
		UVRect center{};
		RGBA8 color{ RGBA8{ 0x5f, 0x15, 0xd9, 0xff } };
		float borderScale{ 1.f };
		RGBA8 txtColor{ RGBA8_White };
		XY txtPadding{ 20, 5 };
		XY txtPaddingRightBottom{ 20, 10 };
		float txtScale{ 1 };
		float iconPadding{ 5 };

		XX_INLINE XY GetCornerSize() const {
			return { float(frame.textureRect.w - center.w) * borderScale, float(frame.textureRect.h - center.h) * borderScale };
		}

		XX_INLINE XY GetCornerScaledSize() const {
			return GetCornerSize() * texScale;
		}
	};

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

		Scale9& Init(int z_, XY position_, XY anchor_, XY size_, Scale9Config const& cfg_) {
			return Init(z_, position_, cfg_.borderScale, anchor_, size_ / cfg_.borderScale, cfg_);
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
