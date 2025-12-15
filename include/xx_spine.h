#pragma once
// spine version switcher
#include "xx_spine38.h"

#include "xx_frame.h"
#include "xx_framebuffer.h"

// some spine utils

namespace xx {

	struct SpineEventData {
		std::string name;
		int32_t intValue{};
		float floatValue{};
	};

	// Frame.userData map to Events.index
	inline void SpineToFrames(
		xx::List<xx::Frame>& outFrames
		, xx::List<SpineEventData>& outEventDatas
		, spine::SkeletonData* sd_
		, spine::Animation* a_
		, XY size_
		, XY offset_
		, XY drawScale_
		, float frameDelta_
	) {
		auto fsLen = outFrames.len;
		auto size = size_ * drawScale_;
		auto anchor = offset_ / size_;
		offset_ *= drawScale_;

		xx::SpinePlayer sp(sd_);
		sp.SetScale(drawScale_);
		sp.SetAnimation(0, a_, true);
		sp.skeleton.setToSetupPose();

		xx::SpineListener sl;
		bool hasEventData{};
		SpineEventData eventData;
		sl.SetCallBack([&](spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* e) {
			assert(type == spine::EventType::EventType_Event);
			auto&& ed = e->getData();// .getIntValue());
			auto&& s = ed.getStringValue();
			eventData.name = { s.buffer(), s.length() };
			eventData.intValue = ed.getIntValue();
			eventData.floatValue = ed.getFloatValue();
			hasEventData = true;
		});
		sp.animationState.setListener(&sl);

		// spine frames -> tex
		auto numFrames = (int32_t)(a_->getDuration() / frameDelta_);

		// calculate tex max size
		int32_t texSize{ 256 };
	LabRetry:
		auto numCols = texSize / (int32_t)size.x;
		auto numRows = texSize / (int32_t)size.y;
		if (numCols * numRows < numFrames) {
			texSize *= 2;
			goto LabRetry;
		}
		auto stepX = texSize / numCols;
		auto stepY = texSize / numRows;
		XY origin{ -texSize / 2 };

		// fill tex & outFrames
		auto tex = xx::FrameBuffer{}.Init().Draw(texSize, true, {}, [&] {
			int32_t i{};
			for (int32_t y = 0; y < numRows; ++y) {
				for (int32_t x = 0; x < numCols; ++x) {
					XY pos{ origin.x + stepX * x, origin.y + texSize - stepY * (y + 1) };
					pos += offset_;

					sp.SetPosition(pos);
					sp.Update(frameDelta_);
					sp.Draw();

					auto& f = outFrames.Emplace();
					f.anchor = anchor;
					auto& r = f.uvRect;
					r.x = stepX * x;
					r.y = stepY * y;
					r.w = size.x;
					r.h = size.y;

					if (hasEventData) {
						hasEventData = {};
						f.userData = outEventDatas.len;
						outEventDatas.Emplace(std::move(eventData));
					}
					else {
						f.userData = -1;
					}

					++i;
					if (i == numFrames) return;
				}
			}
		});
		tex->TryGenerateMipmap();

		// fill outFrames's tex
		for (int32_t i = fsLen; i < outFrames.len; ++i) {
			outFrames[i].tex = tex;
		}
	}

}
