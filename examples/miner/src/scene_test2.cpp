#include "pch.h"
#include "scene_test2.h"
#include "scene_mainmenu.h"



void Rock2::Init(Scene_Test2* scene_) {
	scene = scene_;
	auto i1 = gg.rnd.Next<int32_t>(0, gg.res.rocks_.size());
	auto i2 = gg.rnd.Next<int32_t>(2) * 2 + 1;
	tf = gg.res.rocks_[i1][i2];

	auto fpIdx = gg.rnd.Next<int32_t>(scene->rocksFixedPosPool.len);
	fixedPos = scene->rocksFixedPosPool[fpIdx];
	scene->rocksFixedPosPool.SwapRemoveAt(fpIdx);
	XY posOffset{
		gg.rnd.Next<float>(-scene->cRockMarginOffsetRange.x, scene->cRockMarginOffsetRange.x),
		gg.rnd.Next<float>(-scene->cRockMarginOffsetRange.y, scene->cRockMarginOffsetRange.y)
	};
	pos = fixedPos + posOffset;
	scene->rocksGrid.Add(indexAtGrid, this);
}

void Rock2::Update() {
	// todo: born logic? change scale?
}

void Rock2::Draw() {
	gg.Quad().Draw(tf, tf, scene->cam.ToGLPos(pos), { 0.5f, 0 }, scene->cRocksScale * scene->cam.scale);
}

void Rock2::Dispose() {
	assert(indexAtList > -1);
	assert(scene->rocks[indexAtList].pointer == this);
	auto ial = indexAtList;
	scene->rocks.Back()->indexAtList = ial;
	scene->rocks.SwapRemoveAt(ial);
}

Rock2::~Rock2() {
	assert(indexAtGrid > -1);
	scene->rocksGrid.Remove(indexAtGrid, this);
	scene->rocksFixedPosPool.Emplace(fixedPos);
}

/********************************************************************************************************/

void Scene_Test2::Init() {
	cam.Init(gg.scale, 1.f, gg.designSize / 2);
	MakeUI();

	XYi cGridSize{ 50, 30 };
	auto cRockMargin = gg.designSize / cGridSize;
	cRockMarginOffsetRange = { cRockMargin / 3 };
	cRocksScale = 0.37f;
	cRocksMaxCount = cGridSize.x * cGridSize.y;
	cMouseCircleRadius = 128.f;

	auto cellSize = (int32_t)std::ceilf(std::max(cRockMargin.x, cRockMargin.y));
	auto numCRs = gg.designSize.As<int32_t>() / cellSize + 1;
	rocksGrid.Init(cellSize, numCRs.y, numCRs.x);
	rocks.Reserve(cRocksMaxCount);

	XY basePoss[]{ { cRockMargin.x / 2, cRockMargin.y / 2 }, { cRockMargin.x / 2, cRockMargin.y } };
	for (int y = 0; y < cGridSize.y; ++y) {
		for (int x = 0; x < cGridSize.x; ++x) {
			auto& basePos = basePoss[x & 1];
			rocksFixedPosPool.Emplace(basePos.x + cRockMargin.x * x, basePos.y + cRockMargin.y * y);
		}
	}
	assert(rocksFixedPosPool.len == cRocksMaxCount);

	GenRocks(1000);
	SortRocks();
}

void Scene_Test2::MakeUI() {
	ui.Emplace()->InitRoot(gg.scale * cUIScale);
	// todo
}

void Scene_Test2::GenRocks(int32_t count_) {
	if (rocks.len + count_ > cRocksMaxCount) {
		count_ = cRocksMaxCount - rocks.len;
	}
	if (!count_) return;
	for (int i = 0; i < count_; ++i) {
		rocks.Emplace().Emplace()->Init(this);
	}
}

void Scene_Test2::SortRocks() {
	std::sort(rocks.buf, rocks.buf + rocks.len, [](auto& a, auto& b)->bool {
		return a->pos.y < b->pos.y;
	});
	for (int i = 0; i < rocks.len; ++i) {
		rocks[i]->indexAtList = i;
	}
}

void Scene_Test2::Update() {
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

void Scene_Test2::FixedUpdate() {
	auto mp = cam.ToLogicPos(gg.mousePos);
	if (mp.x >= 0 && mp.y >= 0 && mp.x < rocksGrid.pixelSize.x && mp.y < rocksGrid.pixelSize.y) {
		bool dirty{};
		auto cri = rocksGrid.PosToCRIndex(mp);
		rocksGrid.ForeachByRange(cri.y, cri.x, cMouseCircleRadius, gg.sgrdd, [&](xx::Grid2dCircle<Rock2*>::Node& node, float distance) {
			auto& o = *node.value;
			auto d = o.pos - mp;
			if (d.x * d.x + d.y * d.y < cMouseCircleRadius * cMouseCircleRadius) {
				node.value->Dispose();
				dirty = true;
			}
		});
		if (dirty) {
			SortRocks();
		}
	}
}

void Scene_Test2::Draw() {
	// draw rocks
	for (auto& rock : rocks) {
		rock->Draw();
	}

	// draw mouse circle
	gg.Quad().Draw(gg.res.circle256, gg.res.circle256, gg.mousePos);

	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene_Test2::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale * cUIScale);
	cam.SetBaseScale(gg.scale);
}
