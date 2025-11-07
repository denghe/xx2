#pragma once
#include "game.h"

struct SpineFrameBatch {
	xx::Shared<xx::GLTexture> tex;
	XY size, anchor;	// , oPos, aPos
	int32_t numFrames{};
	int32_t numCols{}, numRows{};
	int32_t stepX{}, stepY{};
	xx::List<xx::TinyFrame> tfs;	// frame results
	void Init(spine::SkeletonData* sd_, spine::Animation* a_, XY scale_);
};
