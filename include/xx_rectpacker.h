#pragma once
#include "xx_list.h"
#include "xx_prims.h"
#include "xx_gl.h"
#include "xx_frame.h"
#include <stb_rect_pack.h>

namespace xx {
	
    struct RectPacker {
        List<TinyFrame*> tfs;		// need fill by user
        List<stbrp_rect> rects;
        List<stbrp_node> nodes;

        // return 0: success
        int32_t Pack(XY texSize_, XY padding_ = 4);
        // try resize & auto generate mipmap
        int32_t AutoPack(int32_t minPackSize_ = 1024, XY padding_ = 8);
        // after pack, visit tex
        GLTexture& Tex();
    };

}
