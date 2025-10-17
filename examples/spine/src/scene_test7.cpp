#include "pch.h"
#include "scene_test7.h"
#include "scene_mainmenu.h"

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
	XY oPos { a->getX(), a->getY() };
	oPos *= scale_;
	XY aPos { size.x / 2 - oPos.x, size.y / 2 - oPos.y };
	anchor = aPos / size;

	// spine frames -> tex
	numFrames = gg.res.grass1.idle->getDuration() / gg.cDelta;

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

	// fill tex
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
	tex->TryGenerateMipmap();
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




void Grass1::Init(Scene_Test7* scene_, XY pos_, xx::FromTo<float> scale_, xx::FromTo<float> colorPlus_) {
	scene = scene_;
	pos = pos_;
	scale = gg.rnd.Next<float>(scale_.from, scale_.to);
	frameIndex = gg.rnd.Next<int32_t>(scene->sfb.numFrames);
	colorPlus = gg.rnd.Next<float>(colorPlus_.from, colorPlus_.to);
	assert(frameIndex <= scene->sfb.numFrames);
}

void Grass1::Update() {
	++frameIndex;
	if (frameIndex == scene->sfb.numFrames) {
		frameIndex = 0;
	}
}

void Grass1::Draw() {
	// cam? scale? logic pos?
	gg.Quad().Draw(*scene->sfb.tex, scene->sfb.GetUvRect(frameIndex)
		, pos * scene->cam.scale, scene->sfb.anchor, scale * scene->cam.scale
	, 0, colorPlus/*, xx::RGBA8_Red*/);
}





void Scene_Test7::Init() {
	cam.Init(gg.scale, 1.f);
	xRange = { -gg.designSize.x / 2, gg.designSize.x / 2 };
	yRange = { -gg.designSize.y / 2, gg.designSize.y / 2 };

	// data init
	// todo: load from disk ?
	cGrassScale = { 0.3f, 1.f };
	cGrassColorPlus = { 0.3f, 1.f };
	cGrassCount = (cGrassCountRange.from + cGrassCountRange.to) / 2;
	cLeafCount = (cLeafCountRange.from + cLeafCountRange.to) / 2;
	cBGTiling = (cBGTilingRange.from + cBGTilingRange.to) / 2;

	// spine prepare
	sfb.Init(gg.res.grass1.skel, gg.res.grass1.idle, 0.1f);

	// ui init
	ui.Emplace()->InitRoot(gg.scale * 0.5f);

	static constexpr float cSliderWidths[]{ 500, 1000, 200 };
	static constexpr float cMargin{ 20 };
	static constexpr float cLineHeight{ 100 };
	static constexpr XY cItemSize{ cSliderWidths[0] + cSliderWidths[1] + cSliderWidths[2], cLineHeight  - cMargin};
	auto fontSize = cItemSize.y - gg.embed.cfg_s9bN->paddings.TopBottom();
	auto anchor = gg.a7;
	auto offset = gg.p7 * 2 + XY{ cMargin, -cMargin };

	/*************************************************************************************************/
	uiGrassScaleFrom = ui->Make<xx::Slider>();
	uiGrassScaleFrom->valueToString = [](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", v);
		return buf;
	};
	uiGrassScaleFrom->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, cGrassScale.from)("grass scale from");
	uiGrassScaleFrom->onChanged = [this](double v) {
		cGrassScale.from = v;
		if (cGrassScale.from > cGrassScale.to) {
			cGrassScale.to = cGrassScale.from + uiGrassScaleTo->value * 5;
			uiGrassScaleTo->ApplyValue();
		}
		GenGrass();
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiGrassScaleTo = ui->Make<xx::Slider>();
	uiGrassScaleTo->valueToString = [this](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", cGrassScale.from + v * 5);
		return buf;
	};
	uiGrassScaleTo->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (cGrassScale.to - cGrassScale.from) / 5)("grass scale to");
	uiGrassScaleTo->onChanged = [this](double v) {
		cGrassScale.to = cGrassScale.from + v * 5;
		GenGrass();
	};


	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiGrassColorPlusFrom = ui->Make<xx::Slider>();
	uiGrassColorPlusFrom->valueToString = [](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", v);
		return buf;
		};
	uiGrassColorPlusFrom->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, cGrassColorPlus.from)("grass colorplus from");
	uiGrassColorPlusFrom->onChanged = [this](double v) {
		cGrassColorPlus.from = v;
		if (cGrassColorPlus.from > cGrassColorPlus.to) {
			cGrassColorPlus.to = cGrassColorPlus.from + uiGrassScaleTo->value * 5;
			uiGrassScaleTo->ApplyValue();
		}
		GenGrass();
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiGrassColorPlusTo = ui->Make<xx::Slider>();
	uiGrassColorPlusTo->valueToString = [this](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", cGrassColorPlus.from + v * 5);
		return buf;
		};
	uiGrassColorPlusTo->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (cGrassColorPlus.to - cGrassColorPlus.from) / 5)("grass colorplus to");
	uiGrassColorPlusTo->onChanged = [this](double v) {
		cGrassColorPlus.to = cGrassColorPlus.from + v * 5;
		GenGrass();
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiGrassCount = ui->Make<xx::Slider>();
	uiGrassCount->valueToString = [](double v)->std::string {
		return xx::ToString(cGrassCountRange.from + (uint32_t(v * (cGrassCountRange.to - cGrassCountRange.from))));
	};
	uiGrassCount->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (double)cGrassCount / (cGrassCountRange.to - cGrassCountRange.from))("grass count");
	uiGrassCount->onChanged = [this](double v) {
		cGrassCount = int32_t(v * (cGrassCountRange.to - cGrassCountRange.from));
		GenGrass();
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiBGTiling = ui->Make<xx::Slider>();
	uiBGTiling->valueToString = [](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", cBGTilingRange.from + v * (cBGTilingRange.to - cBGTilingRange.from));
		return buf;
	};
	uiBGTiling->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (double)cBGTiling / (cBGTilingRange.to - cBGTilingRange.from))("leaf tiling");
	uiBGTiling->onChanged = [this](double v) {
		cBGTiling = v * (cBGTilingRange.to - cBGTilingRange.from);
		GenBG();
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiLeafCount = ui->Make<xx::Slider>();
	uiLeafCount->valueToString = [](double v)->std::string {
		return xx::ToString(cLeafCountRange.from + (uint32_t(v * (cLeafCountRange.to - cLeafCountRange.from))));
	};
	uiLeafCount->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (double)cLeafCount / (cLeafCountRange.to - cLeafCountRange.from))("leaf count");
	uiLeafCount->onChanged = [this](double v) {
		cLeafCount = int32_t(v * (cLeafCountRange.to - cLeafCountRange.from));
		GenLeaf();
	};

	/*************************************************************************************************/
	GenGrass();
	GenLeaf();
	GenBG();
}

void Scene_Test7::GenGrass() {
	grasses.Clear();
	if (cGrassCount) {
		grasses.Reserve(cGrassCount);
		for (size_t i = 0; i < cGrassCount; i++) {
			XY pos{ gg.rnd.Next<float>(xRange.from, xRange.to)
				, gg.rnd.Next<float>(yRange.from, yRange.to) };
			grasses.Emplace().Init(this, pos, cGrassScale, cGrassColorPlus);
		}
		std::sort(grasses.buf, grasses.buf + grasses.len, [](auto& a, auto& b)->bool {
			return a.pos.y > b.pos.y;
		});
	}
}

void Scene_Test7::GenLeaf() {
	texLeaf = xx::FrameBuffer{}.Init().Draw(gg.designSize, true, {}, [&] {
		for (size_t i = 0; i < cLeafCount; i++) {
			auto idx = gg.rnd.Next<int32_t>(gg.res.brush_.size() - 1);
			auto& tf = gg.res.brush_[idx];
			XY pos{ gg.rnd.Next<float>(xRange.from, xRange.to)
				, gg.rnd.Next<float>(yRange.from, yRange.to) };
			auto r = gg.rnd.Next<float>(-M_PI, M_PI);
			auto colorPlus = gg.rnd.Next<float>(0.5f, 1.f);
			gg.Quad().Draw(tf.tex->id, tf.uvRect, pos, 0.5f, 0.05f, r, colorPlus);
		}
	});
	texLeaf->TryGenerateMipmap();
}

void Scene_Test7::GenBG() {
	texBG = xx::FrameBuffer{}.Init().Draw(gg.designSize, true, {}, [&] {
		gg.Grass().Draw({ 0, 0, uint16_t(gg.designSize.x * cBGTiling), uint16_t(gg.designSize.y * cBGTiling) }
		, 0, 0.5f, 1.f / cBGTiling);
	});
	texBG->TryGenerateMipmap();
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
	gg.Quad().Draw(*texBG, texBG->Rect(), 0, 0.5f, cam.scale, 0, 1/*, xx::RGBA8_Red*/);
	gg.Quad().Draw(*texLeaf, texLeaf->Rect(), 0, 0.5f, cam.scale, 0, 1/*, xx::RGBA8_Red*/);
#if 0
	gg.Quad().Draw(*sfb.tex, sfb.tex->Rect());// , 0, 0.5f);// , gg.designSize.y / sfb.tex->size.y);
#else
	for (auto& o : grasses) {
		o.Draw();
	}
#endif
	gg.DrawNode(ui);
}

void Scene_Test7::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale * 0.5f);
	cam.SetBaseScale(gg.scale);
}
