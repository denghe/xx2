#include "pch.h"
#include "game_scene.h"
#include "game_rock.h"
#include "game_flyingrock.h"
#include "scene_mainmenu.h"

void Scene::Init(float totalScale_) {
	cam.Init(gg.scale, 0.8f, gg.designSize / 2);
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

	GenRocks(rocksFixedPosPool.len * 0.9);
	SortRocks();
}

void Scene::MakeUI() {
	static constexpr float cLineHeight{ 50 };
	static constexpr float cMargin{ 10 };
	auto cfg = xx::MakeShared<xx::Scale9Config>();
	*cfg = *gg.embed.cfg_s9bN;
	cfg->paddings = { 10, 20, 10, 20 };
	auto fontSize = cLineHeight - cfg->paddings.TopBottom();
	auto basePos = gg.p7 + XY{ cMargin, -cMargin };
	auto anchor = gg.a7;

	ui.Emplace()->InitRoot(gg.scale);
	for (int i = 0; i < counts.size(); ++i) {
		auto pos = basePos + XY{ i * gg.designSize.x / counts.size(), 0 };
		auto& o = ui->Make<xx::ImageLabelButton>()->Init(2, pos, anchor, fontSize, 0, 0, cfg, cfg)
			(gg.tf.rocks_[i][4], cLineHeight * 1.5, cLineHeight * 0.5f, false)(xx::ToString(counts[i]));
		o.At<xx::Scale9>(2).SetAlphaRecursive(0.5f);
		countUIs[i] = xx::WeakFromThis(&o);
		flyTargets[i] = cam.ToLogicPos((pos + XY{ cLineHeight * 0.5f, -cLineHeight * 0.5f }) * ui->scale);
	}
}

void Scene::GenRocks(int32_t count_) {
	if (rocks.len + count_ > cRocksMaxCount) {
		count_ = cRocksMaxCount - rocks.len;
	}
	if (!count_) return;
	for (int i = 0; i < count_; ++i) {
		rocks.Emplace().Emplace()->Init(this);
	}
}

void Scene::SortRocks() {
	std::sort(rocks.buf, rocks.buf + rocks.len, [](auto& a, auto& b)->bool {
		return a->pos.y < b->pos.y;
		});
	for (int i = 0; i < rocks.len; ++i) {
		rocks[i]->indexAtList = i;
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
	time += d;
	timePool += d;
	while (timePool >= gg.cDelta) {
		timePool -= gg.cDelta;
		FixedUpdate();
	}
}

void Scene::FixedUpdate() {
	rocksDisposedCountPerFrame = 0;

	auto mp = cam.ToLogicPos(gg.mousePos);
	if (mp.x >= 0 && mp.y >= 0 && mp.x < rocksGrid.pixelSize.x && mp.y < rocksGrid.pixelSize.y) {
		auto total = rocks.len;
		auto cri = rocksGrid.PosToCRIndex(mp);
		auto rockRadius = 32 * cRocksScale;
		rocksGrid.ForeachByRange(cri.y, cri.x, cMouseCircleRadius + rockRadius * 3, gg.sgrdd, [&](xx::Grid2dCircle<Rock*>::Node& node, float distance) {
			if (!node.value->ready || node.value->mining) return;
			auto& o = *node.value;
			auto d = o.centerPos - mp;
			auto r = cMouseCircleRadius + rockRadius;
			if (d.x * d.x + d.y * d.y < r * r) {
				node.value->BeginDig();
			}
			});
	}

	for (auto& rock : rocks) {
		rock->Update();
	}

	if (rocksDisposedCountPerFrame > 0) {
		GenRocks(rocksDisposedCountPerFrame);
		SortRocks();
	}

	for (auto i = flyingRocks.len - 1; i >= 0; --i) {
		auto& o = flyingRocks[i];
		if (o.Update()) {
			++counts[o.typeId];
			flyingRocks.SwapRemoveAt(i);
		}
	}
}

void Scene::Draw() {
	// draw bg
	gg.Quad().Draw(gg.tf.bg1, gg.tf.bg1, 0, 0.5f, gg.designSize.y / gg.tf.bg1.Size().y * cam.scale, 0, 0.5f);

	// draw rocks
	for (auto& rock : rocks) {
		rock->Draw();
	}

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

