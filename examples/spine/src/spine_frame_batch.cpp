#include "pch.h"
#include "spine_frame_batch.h"

void SpineFrameBatch::Init(spine::SkeletonData* sd_, spine::Animation* a_, XY scale_) {
	xx::SpinePlayer sp(sd_);
	sp.SetScale(scale_);
	sp.SetAnimation(0, a_, true);
	sp.skeleton.setToSetupPose();

	/*        h
	+---------+
	|         |
	|         |
	|         |
	|    O    |
	|         |
	|         |
	|         |
  00+------A--+w

	given O's pos & size( w&h ), find A( root/anchor )
	anchor.x = (w/2 - A.x) / w
	anchor.y = (A.y - h/2) / h;
	*/

	// calculate anchor
	// todo: args set size when no slot: size
	auto a = (spine::RegionAttachment*)sp.FindSlot("size")->getAttachment();
	size = { a->getScaleX() * a->getWidth(), a->getScaleY() * a->getHeight() };
	size *= scale_;
	XY oPos{ a->getX(), a->getY() };
	oPos *= scale_;
	XY aPos{ size.x / 2 - oPos.x, size.y / 2 - oPos.y };
	anchor = aPos / size;

	// spine frames -> tex
	numFrames = a_->getDuration() / gg.cDelta;

	// calculate tex max size
	int32_t texSize{ 256 };
LabRetry:
	numCols = texSize / (int32_t)size.x;
	numRows = texSize / (int32_t)size.y;
	if (numCols * numRows < numFrames) {
		texSize *= 2;
		goto LabRetry;
	}
	stepX = texSize / numCols;
	stepY = texSize / numRows;
	XY origin{ -texSize / 2 };

	// fill tex & tfs
	tex = xx::FrameBuffer{}.Init().Draw(texSize, true, {}, [&] {
		int32_t i{};
		for (int32_t y = 0; y < numRows; ++y) {
			for (int32_t x = 0; x < numCols; ++x) {
				XY pos{ origin.x + stepX * x, origin.y + texSize - stepY * (y + 1) };
				pos += aPos;
				sp.SetPosition(pos);
				sp.Update(gg.cDelta);
				sp.Draw();
				auto& r = tfs.Emplace().uvRect;
				r.x = stepX * x;
				r.y = stepY * y;
				r.w = size.x;
				r.h = size.y;
				++i;
				if (i == numFrames) return;
			}
		}
		});
	tex->TryGenerateMipmap();

	// fill tfs's tex
	for (auto& o : tfs) o.tex = tex;
}
