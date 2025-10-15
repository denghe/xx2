#include "pch.h"
#include "scene_test7.h"
#include "scene_mainmenu.h"

void SpineFrameBatch::Init(spine::SkeletonData* sd_, spine::Animation* a_) {

	xx::SpinePlayer sp(sd_);
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
	auto a = (spine::RegionAttachment*)sp.FindSlot("size")->getAttachment();
	size = { a->getScaleX() * a->getWidth(), a->getScaleY() * a->getHeight() };
	XY oPos { a->getX(), a->getY() };
	XY aPos { size.x / 2 - oPos.x, size.y / 2 - oPos.y };
	anchor = aPos / size;

	auto d = gg.res.grass1.idle->getDuration();
	numFrames = d / gg.cDelta;

	// spine frames -> tex
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

	tex = xx::FrameBuffer{}.Init().Draw(texSize, true, {}, [&] {
		int32_t i{};
		for (int32_t y = 0; y < numRows; ++y) {
			for (int32_t x = 0; x < numCols; ++x) {
				XY pos{ origin.x + stepX * x, origin.y + texSize - stepY * (y + 1) };
				pos += aPos;
				sp.SetPosition(pos);
				sp.Update(gg.cDelta);
				sp.Draw();
				++i;
				if (i == numFrames) return;
			}
		}
	});
}

xx::UVRect SpineFrameBatch::GetUvRect(int32_t frameIndex_) const {
	auto y = frameIndex_ / numCols;
	auto x = frameIndex_ - numCols * y;
	xx::UVRect r;
	r.x = stepX * x;
	r.y = stepY * y;
	r.w = size.x;
	r.h = size.y;
	return r;
}




void Grass1::Init(Scene_Test7* scene_) {
	scene = scene_;
}

void Grass1::Update() {
	++frameIndex;
	assert(frameIndex <= scene->sfb.numFrames);
	if (frameIndex == scene->sfb.numFrames) {
		frameIndex = 0;
	}
}

void Grass1::Draw() {
	// cam? scale? logic pos?
	gg.Quad().Draw(*scene->sfb.tex, scene->sfb.GetUvRect(frameIndex), 0, scene->sfb.anchor);
}





void Scene_Test7::Init() {
	cam.Init(gg.scale, 0.25f);
	ui.Emplace()->InitRoot(gg.scale);

	sfb.Init(gg.res.grass1.skel, gg.res.grass1.idle);
	grasses.Reserve(10000);
	for (size_t i = 0; i < 10000; i++) {
		grasses.Emplace().Init(this);
	}
}

void Scene_Test7::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}

	// fixed update
	auto d = float(std::min((float)gg.delta, gg.cMaxDelta) * timeScale);
	time += d;
	timePool += d;
	while (timePool >= gg.cDelta) {
		timePool -= gg.cDelta;
		FixedUpdate();
	}
}

void Scene_Test7::FixedUpdate() {
	for (auto& o : grasses) {
		o.Update();
	}
}

void Scene_Test7::Draw() {
#if 0
	gg.Quad().Draw(*sfb.tex, sfb.tex->Rect(), 0, 0.5f, gg.designSize.y / sfb.tex->size.y);
#else
	for (auto& o : grasses) {
		o.Draw();
	}
#endif
	gg.DrawNode(ui);
}

void Scene_Test7::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
