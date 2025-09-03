#pragma once
#include "xx_node.h"
#include "xx_frame.h"

namespace xx {

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
		float radians{};

		Image& Init(int z_, XY position_, XY anchor_, XY fixedSize_, bool keepAspect_
			, TinyFrame frame_, ImageRadians radians_ = ImageRadians::Zero, RGBA8 color_ = RGBA8_White) {
			assert(typeId == cTypeId);
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_;
			frame = std::move(frame_);
			radians = float(M_PI) * 0.5f * (int32_t)radians_;
			color = color_;

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
			float cp;
			if (enabled) cp = 1.f;
			else cp = 0.5f;
			GameBase_shader::GetInstance()->Quad().Draw(*frame.tex, frame.textureRect, worldMinXY, 0
				, worldScale * fixedScale, radians, cp, { color.r, color.g, color.b, (uint8_t)(color.a * alpha) });
		}
	};

}
