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




void Grass1::FillColorplus() {
	colorPlus = gg.rnd.Next<float>(scene->cGrassColorPlus.from, scene->cGrassColorPlus.to);
}
void Grass1::FillScale() {
	scale = gg.rnd.Next<float>(scene->cGrassScale.from, scene->cGrassScale.to);
}

void Grass1::Init(Scene_Test7* scene_, SpineFrameBatch* sfb_, XY pos_) {
	scene = scene_;
	sfb = sfb_;
	pos = pos_;
	FillColorplus();
	FillScale();
	frameIndex = gg.rnd.Next<int32_t>(sfb->numFrames);
	assert(frameIndex <= sfb->numFrames);
}

void Grass1::Update() {
	++frameIndex;
	if (frameIndex == sfb->numFrames) {
		frameIndex = 0;
	}
}

void Grass1::Draw() {
	auto& f = sfb->tfs[frameIndex];
	gg.Quad().Draw(*f.tex, f.uvRect, pos * scene->cam.scale, sfb->anchor, scale * scene->cam.scale
	, 0, colorPlus/*, xx::RGBA8_Red*/);
}





void Scene_Test7::Init() {
	cam.Init(gg.scale, 1.f);
	xRange = { -gg.designSize.x / 2, gg.designSize.x / 2 };
	yRange = { -gg.designSize.y / 2, gg.designSize.y / 2 };

	// todo: load from disk ?
	// data init
	cGrassScale = { 0.3f, 1.f };
	cGrassColorPlus = { 0.3f, 1.f };
	cGrassCount = (cGrassCountRange.from + cGrassCountRange.to) / 10;
	cLeafScale = (cLeafScaleRange.from + cLeafScaleRange.to) / 10;
	cLeafColorplus = 1.f;
	cLeafCount = (cLeafCountRange.from + cLeafCountRange.to) / 10;
	cBGColorplus = 1.f;
	cBGTiling = (cBGTilingRange.from + cBGTilingRange.to) / 10;

	// spine prepare
	for (auto& f : gg.res.flower_) {
		sfbsFlower.Emplace().Init(f.skel, f.idle, 0.1f);
	}
	for (auto& f : gg.res.grass_) {
		sfbsGrass.Emplace().Init(f.skel, f.idle, 0.1f);
	}

	// sfb batch combine
	xx::RectPacker rp;
	for (auto& o : sfbsFlower) {
		for (auto& tf : o.tfs) {
			rp.tfs.Add(&tf);
		}
	}
	for (auto& o : sfbsGrass) {
		for (auto& tf : o.tfs) {
			rp.tfs.Add(&tf);
		}
	}
	if (auto r = rp.Pack(4096, 8); r) {
		xx::CoutN("pack failed");
	}
	else {
		sfbsFlower[0].tfs[0].tex->TryGenerateMipmap();
		for (auto& o : sfbsFlower) o.tex.Reset();
		for (auto& o : sfbsGrass) o.tex.Reset();
	}


	// ui init
	ui.Emplace()->InitRoot(gg.scale * 0.5f);

	static constexpr float cSliderWidths[]{ 600, 1000, 240 };
	static constexpr float cMargin{ 5 };
	static constexpr float cLineHeight{ 100 };
	static constexpr XY cItemSize{ cSliderWidths[0] + cSliderWidths[1] + cSliderWidths[2], cLineHeight  - cMargin};
	auto fontSize = cItemSize.y - gg.embed.cfg_s9bN->paddings.TopBottom();
	auto anchor = gg.a7;
	auto offset = gg.p7 * 2 + XY{ cMargin, -cMargin };

	/*************************************************************************************************/
	uiGrassScaleFrom = ui->Make<xx::Slider>();
	uiGrassScaleFrom->callbackWhenBlockMoving = true;
	uiGrassScaleFrom->valueToString = [](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", v);
		return buf;
	};
	uiGrassScaleFrom->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, cGrassScale.from)("grass scale from");
	uiGrassScaleFrom->onChanged = [this](double v) {
		cGrassScale.from = v;
		if (cGrassScale.to < cGrassScale.from) {
			cGrassScale.to = cGrassScale.from;
		}
		uiGrassScaleTo->SetValue((cGrassScale.to - cGrassScale.from) / 5);
		uiGrassScaleTo->ApplyValue();
		ChangeGrassScale();
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiGrassScaleTo = ui->Make<xx::Slider>();
	uiGrassScaleTo->callbackWhenBlockMoving = true;
	uiGrassScaleTo->valueToString = [this](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", cGrassScale.from + v * 5);
		return buf;
	};
	uiGrassScaleTo->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (cGrassScale.to - cGrassScale.from) / 5)("grass scale to");
	uiGrassScaleTo->onChanged = [this](double v) {
		cGrassScale.to = cGrassScale.from + v * 5;
		ChangeGrassScale();
	};


	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiGrassColorPlusFrom = ui->Make<xx::Slider>();
	uiGrassColorPlusFrom->callbackWhenBlockMoving = true;
	uiGrassColorPlusFrom->valueToString = [](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", v);
		return buf;
	};
	uiGrassColorPlusFrom->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, cGrassColorPlus.from)("grass colorplus from");
	uiGrassColorPlusFrom->onChanged = [this](double v) {
		cGrassColorPlus.from = v;
		if (cGrassColorPlus.to < cGrassColorPlus.from) {
			cGrassColorPlus.to = cGrassColorPlus.from;
		}
		uiGrassColorPlusTo->SetValue((cGrassColorPlus.to - cGrassColorPlus.from) / 5);
		uiGrassColorPlusTo->ApplyValue();
		ChangeGrassColorplus();
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiGrassColorPlusTo = ui->Make<xx::Slider>();
	uiGrassColorPlusTo->callbackWhenBlockMoving = true;
	uiGrassColorPlusTo->valueToString = [this](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", cGrassColorPlus.from + v * 5);
		return buf;
	};
	uiGrassColorPlusTo->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (cGrassColorPlus.to - cGrassColorPlus.from) / 5)("grass colorplus to");
	uiGrassColorPlusTo->onChanged = [this](double v) {
		cGrassColorPlus.to = cGrassColorPlus.from + v * 5;
		ChangeGrassColorplus();
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiGrassCount = ui->Make<xx::Slider>();
	uiGrassCount->callbackWhenBlockMoving = true;
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
	uiBGColorplus = ui->Make<xx::Slider>();
	uiBGColorplus->callbackWhenBlockMoving = true;
	uiBGColorplus->valueToString = [](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", cBGColorplusRange.from + v * (cBGColorplusRange.to - cBGColorplusRange.from));
		return buf;
	};
	uiBGColorplus->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (double)cBGColorplus / (cBGColorplusRange.to - cBGColorplusRange.from))("BG colorplus");
	uiBGColorplus->onChanged = [this](double v) {
		cBGColorplus = v * (cBGColorplusRange.to - cBGColorplusRange.from);
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiBGTiling = ui->Make<xx::Slider>();
	uiBGTiling->callbackWhenBlockMoving = true;
	uiBGTiling->valueToString = [](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", cBGTilingRange.from + v * (cBGTilingRange.to - cBGTilingRange.from));
		return buf;
	};
	uiBGTiling->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (double)cBGTiling / (cBGTilingRange.to - cBGTilingRange.from))("BG tiling");
	uiBGTiling->onChanged = [this](double v) {
		cBGTiling = v * (cBGTilingRange.to - cBGTilingRange.from);
		GenBG();
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiLeafScale = ui->Make<xx::Slider>();
	uiLeafScale->callbackWhenBlockMoving = true;
	uiLeafScale->valueToString = [](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", cLeafScaleRange.from + v * (cLeafScaleRange.to - cLeafScaleRange.from));
		return buf;
	};
	uiLeafScale->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (double)cLeafScale / (cLeafScaleRange.to - cLeafScaleRange.from))("leaf scale");
	uiLeafScale->onChanged = [this](double v) {
		cLeafScale = v * (cLeafScaleRange.to - cLeafScaleRange.from);
		GenLeaf();
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiLeafColorplus = ui->Make<xx::Slider>();
	uiLeafColorplus->callbackWhenBlockMoving = true;
	uiLeafColorplus->valueToString = [](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", cLeafColorplusRange.from + v * (cLeafColorplusRange.to - cLeafColorplusRange.from));
		return buf;
	};
	uiLeafColorplus->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (double)cLeafColorplus / (cLeafColorplusRange.to - cLeafColorplusRange.from))("leaf colorplus");
	uiLeafColorplus->onChanged = [this](double v) {
		cLeafColorplus = v * (cLeafColorplusRange.to - cLeafColorplusRange.from);
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiLeafCount = ui->Make<xx::Slider>();
	uiLeafCount->callbackWhenBlockMoving = true;
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
			SpineFrameBatch *sfb;
			if (gg.rnd.Next<float>() > 0.1f) {
				sfb = &sfbsGrass[gg.rnd.Next<int32_t>(sfbsGrass.len)];
			}
			else {
				sfb = &sfbsFlower[gg.rnd.Next<int32_t>(sfbsFlower.len)];
			}
			grasses.Emplace().Init(this, sfb, pos);
		}
		std::sort(grasses.buf, grasses.buf + grasses.len, [](auto& a, auto& b)->bool {
			return a.pos.y > b.pos.y;
		});
	}
}

void Scene_Test7::GenLeaf() {
	texLeaf = xx::FrameBuffer{}.Init().Draw(gg.designSize, true, {}, [&] {
		for (size_t i = 0; i < cLeafCount; i++) {
			auto idx = gg.rnd.Next<int32_t>(gg.res.brush_.size());
			auto& tf = gg.res.brush_[idx];
			XY pos{ gg.rnd.Next<float>(xRange.from, xRange.to)
				, gg.rnd.Next<float>(yRange.from, yRange.to) };
			auto r = gg.rnd.Next<float>(-M_PI, M_PI);
			gg.Quad().Draw(tf.tex->id, tf.uvRect, pos, 0.5f, cLeafScale, r, cLeafColorplus);
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

void Scene_Test7::ChangeGrassColorplus() {
	for (auto& o : grasses) {
		o.FillColorplus();
	}
}

void Scene_Test7::ChangeGrassScale() {
	for (auto& o : grasses) {
		o.FillScale();
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
	gg.Quad().Draw(*texBG, texBG->Rect(), 0, 0.5f, cam.scale, 0, cBGColorplus/*, xx::RGBA8_Red*/);
	gg.Quad().Draw(*texLeaf, texLeaf->Rect(), 0, 0.5f, cam.scale, 0, cLeafColorplus/*, xx::RGBA8_Red*/);
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
