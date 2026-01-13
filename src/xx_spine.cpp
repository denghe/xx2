#include "xx_spine.h"

namespace xx {

	// Frame.userData map to Events.index
	void SpineToFrames(
		List<Frame>& outFrames
		, List<SpineEventData>& outEventDatas
		, spine::SkeletonData* sd_
		, spine::Animation* a_
		, XY size_
		, XY offset_
		, XY drawScale_
		, float frameDelta_
		, float timeBegin_
		, float timeEnd_
	) {
		// bad logic occur check
		assert(frameDelta_ <= 1.f / 30.f);
		assert(frameDelta_ >= 1.f / 360.f);

		auto fsLen = outFrames.len;
		auto size = size_ * drawScale_;
		auto anchor = offset_ / size_;
		offset_ *= drawScale_;

		SpinePlayer sp(sd_);
		sp.SetScale(drawScale_);
		sp.SetAnimation(0, a_, true);
		sp.skeleton.setToSetupPose();
		sp.skeleton.updateWorldTransform();

		SpineListener sl;
		bool hasEventData{};
		SpineEventData eventData;
		sl.SetCallBack([&](spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* e) {
			if (type != spine::EventType::EventType_Event) return;
			auto&& ed = e->getData();
			auto&& s = ed.getStringValue();
			eventData.name = { s.buffer(), s.length() };
			eventData.intValue = ed.getIntValue();
			eventData.floatValue = ed.getFloatValue();
			hasEventData = true;
			});
		sp.animationState.setListener(&sl);


		// spine frames -> tex
		int32_t numFrames{};
		if (timeEnd_ == 0.f) {
			timeEnd_ = a_->getDuration();
		}
		if (timeBegin_ >= timeEnd_) {
			assert(false);
			return;
		}
		numFrames = (int32_t)((timeEnd_ - timeBegin_) / frameDelta_);
		assert(numFrames > 0);

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

		sp.SetPosition(0);
		sp.Update(timeBegin_);

		// fill tex & outFrames
		auto tex = FrameBuffer{}.Init().Draw(texSize, true, {}, [&] {
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

	void LoadSpineToFrames(
		std::string filePrefix
		, std::string animName
		, List<Frame>& outFrames
		, List<SpineEventData>& outEventDatas
		, XY drawScale_
		, float frameDelta_
		, float timeBegin_
		, float timeEnd_
	) {
		Shared<GLTexture> t;
		spine::SkeletonData* s{};
		if (auto&& iter = gSpineEnv.skeletonDatas.find(filePrefix + ".skel"); iter != gSpineEnv.skeletonDatas.end()) {
			s = iter->second.get();
		}
		else {
			gSpineEnv.Load(false, filePrefix, s, t);
		}

		auto&& ed = s->findEvent(animName.c_str());
		assert(ed);

		auto&& str = ed->getStringValue();
		std::string_view sv(str.buffer(), str.length());
		auto left = SvToNumber<int32_t>(SplitOnce(sv, ","), 0);
		auto right = SvToNumber<int32_t>(SplitOnce(sv, ","), 0);
		auto bottom = SvToNumber<int32_t>(SplitOnce(sv, ","), 0);
		auto top = SvToNumber<int32_t>(sv, 0);
		assert(left > 0 && right > 0 && bottom > 0 && top > 0);

		auto a = s->findAnimation(animName.c_str());
		SpineToFrames(outFrames, outEventDatas, s, a
			, { left + right, bottom + top }, { left, bottom }, drawScale_, frameDelta_
			, timeBegin_, timeEnd_
		);
	}

}
