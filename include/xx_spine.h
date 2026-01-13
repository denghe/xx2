#pragma once
// spine version switcher
#include "xx_spine38.h"

// some spine utils

namespace xx {

	struct SpineEventData {
		std::string name;
		int32_t intValue{};
		float floatValue{};
	};

	// Frame.userData map to Events.index ( only support .skel now)
	void SpineToFrames(
		List<Frame>& outFrames
		, List<SpineEventData>& outEventDatas
		, spine::SkeletonData* sd_
		, spine::Animation* a_
		, XY size_
		, XY offset_
		, XY drawScale_
		, float frameDelta_
		, float timeBegin_ = 0.f
		, float timeEnd_ = 0.f
	);

	void LoadSpineToFrames(
		std::string filePrefix
		, std::string animName
		, List<Frame>& outFrames
		, List<SpineEventData>& outEventDatas
		, XY drawScale_
		, float frameDelta_
		, float timeBegin_ = 0.f
		, float timeEnd_ = 0.f
	);

}
