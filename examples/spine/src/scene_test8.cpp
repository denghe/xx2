#include "pch.h"
#include "scene_test8.h"
#include "scene_mainmenu.h"

void Scene_Test8::Init() {
	// todo: load from disk ?

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
	rp.AutoPack(4096, 8);
	for (auto& o : sfbsFlower) o.tex.Reset();
	for (auto& o : sfbsGrass) o.tex.Reset();


	float maxWidth{};
	for (auto& o : sfbsFlower) if (o.size.x > maxWidth) maxWidth = o.size.x;
	for (auto& o : sfbsGrass) if (o.size.x > maxWidth) maxWidth = o.size.x;
	assert(maxWidth > 0);
	auto cellSize = (int32_t)std::ceilf(maxWidth);



	XYi cGridSize{ 100, 50 };
	auto cRockMargin = gg.designSize / cGridSize;
	cGrassMarginOffsetRange = { cRockMargin / 3 };
	cGrassMaxCount = cGridSize.x * cGridSize.y;

	XY basePoss[]{ { cRockMargin.x * 0.5f, cRockMargin.y * 0.25f }, { cRockMargin.x * 0.5f, cRockMargin.y * 0.75f } };
	for (int y = 0; y < cGridSize.y; ++y) {
		for (int x = 0; x < cGridSize.x; ++x) {
			auto& basePos = basePoss[x & 1];
			grassFixedPosPool.Emplace(basePos.x + cRockMargin.x * x, basePos.y + cRockMargin.y * y);
		}
	}

	// data init
	cGrassScale = { 1.f, 1.f };
	cGrassColorPlus = { 0.3f, 1.f };
	cGrassRandomFrameIndex = true;
	cGrassCount = cGrassMaxCount * 0.5;
	cLeafScale = 0.1f;
	cLeafColorplus = 0.f;
	cLeafCount = 1000;
	cBGColorplus = 1.f;
	cBGColor = xx::RGBA8_Red;
	cBGTiling = 0.315f;

	// grid init
	auto numCRs = (gg.designSize / cellSize).As<int32_t>() + 1;
	grid.Init(cellSize, numCRs.y, numCRs.x, cGrassMaxCount);
	grasses.Reserve(cGrassMaxCount);

	// cam init
	cam.Init(gg.scale, 1.f, gg.designSize / 2);

	// ui init
	ui.Emplace()->InitRoot(gg.scale * 0.5f);

	/*************************************************************************************************/

	GenGrass();
	GenLeaf();
	GenBG();
}

void Scene_Test8::GenGrass() {
	grasses.Clear();
	if (!cGrassCount) return;
	for (size_t i = 0; i < cGrassCount; i++) {
		auto idx = gg.rnd.Next<int32_t>(grassFixedPosPool.len);
		auto pos = grassFixedPosPool[idx];
		grassFixedPosPool.SwapRemoveAt(idx);
		SpineFrameBatch* sfb;
		if (gg.rnd.Next<float>() > 0.1f) {
			sfb = &sfbsGrass[gg.rnd.Next<int32_t>(sfbsGrass.len)];
		}
		else {
			sfb = &sfbsFlower[gg.rnd.Next<int32_t>(sfbsFlower.len)];
		}
		grasses.Emplace().Init(this, sfb, pos, cGrassRandomFrameIndex);
	}
	std::sort(grasses.buf, grasses.buf + grasses.len, [](auto& a, auto& b)->bool {
		return a.pos.y < b.pos.y;
		});
	for (auto& o : grasses) o.InitGridIndex();
}

void Scene_Test8::GenLeaf() {
	xx::FromTo<float> xRange{ -gg.designSize.x / 2, gg.designSize.x / 2 };
	xx::FromTo<float> yRange{ -gg.designSize.y / 2, gg.designSize.y / 2 };
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

void Scene_Test8::GenBG() {
	texBG = xx::FrameBuffer{}.Init().Draw(gg.designSize, true, {}, [&] {
		gg.Grass().Draw({ 0, 0, uint16_t(gg.designSize.x * cBGTiling), uint16_t(gg.designSize.y * cBGTiling) }
		, 0, 0.5f, 1.f / cBGTiling);
		});
	texBG->TryGenerateMipmap();
}

void Scene_Test8::ChangeGrassColorplus() {
	for (auto& o : grasses) {
		o.FillColorplus();
	}
}

void Scene_Test8::ChangeGrassScale() {
	for (auto& o : grasses) {
		o.FillScale();
	}
}

void Scene_Test8::Update() {
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

void Scene_Test8::FixedUpdate() {
	for (auto& o : grasses) {
		o.Update();
	}

	// todo: check mouse pos & set range grass frame index
	auto mp = cam.ToLogicPos(gg.mousePos);
	if (mp.x >= 0 && mp.y >= 0 && mp.x < grid.pixelSize.x && mp.y < grid.pixelSize.y) {
		auto cri = grid.PosToCRIndex(mp);
		grid.ForeachByRange(cri.y, cri.x, cMouseRadius, gg.sgrdd, [&](xx::Grid2dCircle<Grass*>::Node& node, float distance) {
			auto& o = *node.value;
			auto d = o.pos - mp;
			if (d.x * d.x + d.y * d.y < cMouseRadius * cMouseRadius) {
				if (o.pos.x > mp.x) {
					o.frameIndex = o.sfb->numFrames / 2;
				}
				else {
					o.frameIndex = 0;
				}
			}
		});
	}
}

void Scene_Test8::Draw() {
	gg.Quad().Draw(*texBG, texBG->Rect(), 0, 0.5f, cam.scale, 0, cBGColorplus, cBGColor);
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

void Scene_Test8::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale * 0.5f);
	cam.SetBaseScale(gg.scale);
}
