#pragma once
#include <stb_rect_pack.h>
#include "xx_frame.h"
#include "xx_framebuffer.h"
#include "xx_gamebase_shader.h"

namespace xx {

	struct RectPacker {

		List<TinyFrame*> tfs;		// need fill by user
		List<stbrp_rect> rects;
		List<stbrp_node> nodes;

		// return 0: success
		int32_t Pack(XY texSize_, XY padding_ = 4) {
			assert(tfs.len);
			auto siz = texSize_.As<int32_t>();
			assert(siz.x > 0 && siz.y > 0);

			rects.Clear();
			rects.Reserve(tfs.len);
			auto grow = (padding_ * 2).As<int32_t>();
			for (int32_t i = 0; i < tfs.len; ++i) {
				auto& tf = tfs[i];
				auto& rect = rects.Emplace();
				rect.id = i;
				rect.w = tf->uvRect.w + grow.x;
				rect.h = tf->uvRect.h + grow.y;
			}

			stbrp_context c;
			nodes.Resize(siz.x);
			stbrp_init_target(&c, siz.x, siz.y, nodes.buf, nodes.len);
			auto r = stbrp_pack_rects(&c, rects.buf, rects.len);
			if (r == 0) return __LINE__;

			Ref<GLTexture> t;
			t.Emplace()->Make(texSize_);
			FrameBuffer{}.Init().DrawTo(t, {}, [&] {
				XY basePos{ -texSize_.x / 2, -texSize_.y / 2 };
				for (auto& o : rects) {
					auto& tf = *tfs[o.id];
					o.x += padding_.x;
					o.y += padding_.y;
					auto pos = basePos + XY{ o.x, o.y };
					GameBase_shader::GetInstance()->Quad().Draw(*tf.tex, tf.uvRect, pos, 0);
				}
			});
			for (auto& o : rects) {
				auto& tf = *tfs[o.id];
				tf.tex = t;
				tf.uvRect.x = uint16_t(o.x);
				tf.uvRect.y = uint16_t(texSize_.y - o.y - tf.uvRect.h);
			}

			tfs.Clear();
			return 0;
		}
	};

}
