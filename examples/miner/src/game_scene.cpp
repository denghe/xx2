#include "pch.h"
#include "game_scene.h"
#include "game_rock.h"
#include "scene_mainmenu.h"

void Scene::Init(float totalScale_) {
	cam.Init(gg.scale, 1.f, gg.designSize / 2);
	MakeUI();

	XYi cGridSize{ 80 * totalScale_, 15 * totalScale_ };
	cRockMargin = gg.designSize / cGridSize;
	cRockMarginOffsetRange = { cRockMargin / 4 };
	cRocksScale = 0.4f / totalScale_;
	cRocksMaxCount = cGridSize.x * cGridSize.y;
	cMouseCircleRadius = 128.f;
	cRocksPivotOffset = { 0, -cRockRadius * cRocksScale };

	auto cellSize = (int32_t)std::ceilf(std::max(cRockMargin.x, cRockMargin.y));
	auto numCRs = gg.designSize.As<int32_t>() / cellSize + 1;
	rocksGrid.Init(cellSize, numCRs.y, numCRs.x);
	rocks.Reserve(cRocksMaxCount);

	// pos filter
	auto& img = gg.stbi.bg1a;
	assert(img.comp == 4);
	auto s = 1.f / (gg.designSize / img.Size());

	XY basePoss[]{ { cRockMargin.x * 0.5f, cRockMargin.y * 0.25f }, { cRockMargin.x * 0.5f, cRockMargin.y * 0.75f } };
	for (int y = 0; y < cGridSize.y; ++y) {
		for (int x = 0; x < cGridSize.x; ++x) {
			auto& basePos = basePoss[x & 1];
			XY pos{ basePos.x + cRockMargin.x * x, basePos.y + cRockMargin.y * y };
			auto ipos = (pos * s).As<int32_t>();
			auto cidx = ipos.y * img.w + ipos.x;
			if (img.At(cidx).a) {
				rocksFixedPosPool.Emplace(pos);
			}
		}
	}
	assert(rocksFixedPosPool.len <= cRocksMaxCount);
	rocksFixedPosPoolBak.AddRange(rocksFixedPosPool);

	GenRocks(rocksFixedPosPool.len * 0.5);
	GenMonsters(10);
}

void Scene::MakeUI() {
	static constexpr float cLineHeight{ 50 };
	static constexpr float cMargin{ 10 };
	auto cfg = xx::MakeShared<xx::Scale9Config>();
	*cfg = *gg.embed.cfg_s9bN;
	cfg->paddings = { 10, 20, 10, 20 };
	auto fontSize = cLineHeight - cfg->paddings.TopBottom();
	auto basePos = gg.p7 + XY{ cMargin * 2, -cMargin * 2 };
	auto anchor = gg.a7;

	ui.Emplace()->InitRoot(gg.scale);
	auto MakeCountUI = [&](int32_t i, XY pos) {
		auto& o = ui->Make<xx::ImageLabelButton>()->Init(2, pos, anchor, fontSize, 0, 0, cfg, cfg)
			(gg.tf.rocks_[i][4], cLineHeight * 1.5, cLineHeight * 0.5f, false)(xx::ToString(counts[i]));
		o.At<xx::Scale9>(2).SetAlphaRecursive(0.5f);
		countUIs[i] = xx::WeakFromThis(&o);
		flyTargets[i] = cam.ToLogicPos((pos + XY{ cLineHeight * 0.5f, -cLineHeight * 0.5f }) * ui->scale);
	};
	assert(counts.size() == 9);
	MakeCountUI(0, basePos + XY{ 0 * gg.designSize.x / counts.size(), 0 });
	MakeCountUI(1, basePos + XY{ 1 * gg.designSize.x / counts.size(), 0 });
	MakeCountUI(2, basePos + XY{ 2 * gg.designSize.x / counts.size(), 0 });
	MakeCountUI(3, basePos + XY{ 0 * gg.designSize.x / counts.size(), -cLineHeight - cMargin * 3 });
	MakeCountUI(4, basePos + XY{ 1 * gg.designSize.x / counts.size(), -cLineHeight - cMargin * 3 });
	MakeCountUI(5, basePos + XY{ 2 * gg.designSize.x / counts.size(), -cLineHeight - cMargin * 3 });
	MakeCountUI(6, basePos + XY{ 0 * gg.designSize.x / counts.size(), (-cLineHeight - cMargin * 3) * 2 });
	MakeCountUI(7, basePos + XY{ 1 * gg.designSize.x / counts.size(), (-cLineHeight - cMargin * 3) * 2 });
	MakeCountUI(8, basePos + XY{ 2 * gg.designSize.x / counts.size(), (-cLineHeight - cMargin * 3) * 2 });

	auto cfgH = xx::MakeShared<xx::Scale9Config>();
	*cfgH = *gg.embed.cfg_s9bH;
	cfgH->paddings = { 10, 20, 10, 20 };
	basePos.y = ui->children.Back()->position.y - cLineHeight * 2;
	ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("game speed:1x").onClicked = [this]() {
		timeScale = 1.f;
	};
	basePos.x += ui->children.Back()->size.x + cMargin;
	ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("10x").onClicked = [this]() {
		timeScale = 10.f;
	};
	basePos.x += ui->children.Back()->size.x + cMargin;
	ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("100x").onClicked = [this]() {
		timeScale = 100.f;
	};
	basePos.x += ui->children.Back()->size.x + cMargin;
	ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("1K").onClicked = [this]() {
		timeScale = 1000.f;
	};
	basePos.x = gg.p7.x + cMargin * 2;
	basePos.y -= ui->children.Back()->size.y + cMargin;
	ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("monster count:40").onClicked = [this]() {
		GenMonsters(10);
	};
	basePos.x += ui->children.Back()->size.x + cMargin;
	ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("400").onClicked = [this]() {
		GenMonsters(100);
	};
	basePos.x += ui->children.Back()->size.x + cMargin;
	ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("4K").onClicked = [this]() {
		GenMonsters(1000);
	};
	basePos.x += ui->children.Back()->size.x + cMargin;
	ui->Make<xx::LabelButton>()->Init(2, basePos, anchor, fontSize, {}, cfg, cfgH)("40K").onClicked = [this]() {
		GenMonsters(10000);
	};
}

void Scene::GenMonsters(int32_t count_) {
	monsters.Clear();
	for (int32_t i = 0; i < count_; ++i) {
		for (int32_t j = 0; j < gg.mcs.size(); ++j) {
			//auto posOffset = xx::GetRndPosDoughnut(gg.rnd, cRockRadius * 5, 0);
			//monsters.Emplace().Emplace<Monster>()->Init(this, j, cam.original + posOffset, 23);
			auto pos = rocksFixedPosPoolBak[gg.rnd.Next(rocksFixedPosPoolBak.len)];
			monsters.Emplace().Emplace<Monster>()->Init(this, j, pos, 23);
		}
	}
}

void Scene::GenRocks(int32_t count_) {
	auto n = borningRocks.len + rocks.len;
	if (n + count_ > cRocksMaxCount) {
		count_ = cRocksMaxCount - n;
	}
	if (!count_) return;
	for (int i = 0; i < count_; ++i) {
		borningRocks.Emplace().Emplace()->Init(this);
	}
}

void Scene::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}

	// fixed update
	auto d = float(std::min((float)gg.delta, gg.cMaxDelta) * timeScale);
	timePool += d;
	while (timePool >= gg.cDelta) {
		time += gg.cDelta;
		timePool -= gg.cDelta;
		FixedUpdate();
	}
}

void Scene::FixedUpdate() {
	rocksDisposedCountPerFrame = 0;

	// mouse dig logic
	auto mp = cam.ToLogicPos(gg.mousePos);
	if (mp.x >= 0 && mp.y >= 0 && mp.x < rocksGrid.pixelSize.x && mp.y < rocksGrid.pixelSize.y) {
		auto cri = rocksGrid.PosToCRIndex(mp);
		auto rockRadius = 32 * cRocksScale;
		rocksGrid.ForeachByRange(cri.y, cri.x, cMouseCircleRadius + rockRadius * 3, gg.sgrdd, [&](xx::Grid2dCircle<Rock*>::Node& node, float distance) {
			auto& o = *node.value;
			if (o.digging) return;
			auto d = o.centerPos - mp;
			auto r = cMouseCircleRadius + rockRadius;
			if (d.x * d.x + d.y * d.y < r * r) {
				node.value->BeginDig();
			}
		});
	}

	for (auto i = monsters.len - 1; i >= 0; --i) {
		if (monsters[i]->Update()) {
			monsters.SwapRemoveAt(i);
		}
	}
	for (auto i = rocks.len - 1; i >= 0; --i) {
		if (rocks[i]->Update()) {
			rocks[i]->Dispose();
		}
	}
	for (auto i = borningRocks.len - 1; i >= 0; --i) {
		if (borningRocks[i]->Update()) {
			borningRocks.SwapRemoveAt(i);
		}
	}
	for (auto i = breakingRocks.len - 1; i >= 0; --i) {
		if (breakingRocks[i].Update()) {
			breakingRocks.SwapRemoveAt(i);
		}
	}

	if (rocksDisposedCountPerFrame > 0) {
		GenRocks(rocksDisposedCountPerFrame);
	}

	for (auto i = flyingRocks.len - 1; i >= 0; --i) {
		auto& o = flyingRocks[i];
		if (o.Update()) {
			++counts[o.typeId];
			flyingRocks.SwapRemoveAt(i);
		}
	}

	//if (timer <= time) {
	//	timer += 0.1f;
	//	std::sort((SceneItem**)monsters.buf, (SceneItem**)monsters.buf + monsters.len, [](auto& a, auto& b) { return a->y < b->y; });
	//}
}

void Scene::Draw() {
	// draw bg
	gg.Quad().Draw(gg.tf.bg1, gg.tf.bg1, 0, 0.5f, gg.designSize.y / gg.tf.bg1.Size().y * cam.scale, 0, 0.5f);

	// sort order by y
	assert(sitems.Empty());
	for (auto& o : borningRocks) sitems.Emplace(o->y, o.pointer);
	for (auto& o : breakingRocks)
		sitems.Emplace(o.y, &o);
	for (auto& o : monsters) sitems.Emplace(o->y, o.pointer);
	for (auto& o : rocks) sitems.Emplace(o->y, o.pointer);
	std::sort(sitems.buf, sitems.buf + sitems.len, [](auto& a, auto& b) { return a.first < b.first; });

	// draw by y
	for (auto& o : sitems) o.second->Draw();
	sitems.Clear();

	// draw fly rocks
	for (auto& o : flyingRocks) o.Draw(this);

	// draw mouse circle
	gg.Quad().Draw(gg.tf.circle256, gg.tf.circle256, gg.mousePos, 0.5f
		, cMouseCircleRadius * 0.0078125f * cam.scale, 0, 1.f, { 255,255,255,127 });

	// sync ui
	for (int32_t i = 0; i < counts.size(); ++i) {
		countUIs[i].CastRef<>()(xx::ToString(counts[i]));
	}

	// draw ui
	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);

	//// gizmos
	//for (auto& p : flyTargets) {
	//	gg.Quad().Draw(gg.embed.shape_gear, gg.embed.shape_gear, cam.ToGLPos(p), 0.5f, cam.scale);
	//}
}

void Scene::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}

void Scene::OnFocus(bool focused_) {
	if (focused_) {
		gg.sound.SetGlobalVolume(1);
	}
	else {
		gg.sound.SetGlobalVolume(0);
	}
}
