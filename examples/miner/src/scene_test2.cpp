#include "pch.h"
#include "scene_test2.h"
#include "scene_mainmenu.h"

void Pickaxe::Init(Rock2* target_) {
	pos = target_->centerPos + XY{ Scene_Test2::cRockRadius * target_->scene->cRocksScale, 0 };
	radians = {};
	_1 = {};
}

bool Pickaxe::Update() {
	static constexpr float cStep1Radians{ 30.f / 180.f * M_PI };
	static constexpr float cStep1RadiansStep{ cStep1Radians / (gg.cFps * 0.1f) };
	static constexpr float cStep2Radians{ -50.f / 180.f * M_PI };
	static constexpr float cStep2RadiansStep{ (cStep2Radians - cStep1Radians) / (gg.cFps * 0.2f) };

	XX_BEGIN(_1);
	for (radians = 0; radians < cStep1Radians; radians += cStep1RadiansStep) {
		XX_YIELD_F(_1);
	}
	gg.PlayAudio(gg.ss.pickaxe);
	for (; radians > cStep2Radians; radians += cStep2RadiansStep) {
		XX_YIELD_F(_1);
	}
	return true;
	XX_END(_1);
	return false;
}

void Pickaxe::Draw(Scene_Test2* scene_) {
	gg.Quad().Draw(gg.res.pickaxe_[0], gg.res.pickaxe_[0], scene_->cam.ToGLPos(pos)
		, { 0.5f, 0.25f }, scene_->cRocksScale * scene_->cam.scale, radians);
}

/********************************************************************************************************/

void Rock2::BeginDig() {
	assert(!digging);
	digging = true;
	pickaxe.Init(this);
}

void Rock2::Init(Scene_Test2* scene_) {
	gg.sound.SetGlobalVolume(0.1);
	scene = scene_;
	hp = cHPMax;
	typeId = gg.rnd.Next<int32_t>(0, gg.res.rocks_.size());
	qualityId = gg.rnd.Next<int32_t>(2);
	tf = gg.res.rocks_[typeId][qualityId * 2 + 1];

	auto fpIdx = gg.rnd.Next<int32_t>(scene->rocksFixedPosPool.len);
	fixedPos = scene->rocksFixedPosPool[fpIdx];
	scene->rocksFixedPosPool.SwapRemoveAt(fpIdx);
	XY posOffset{
		gg.rnd.Next<float>(-scene->cRockMarginOffsetRange.x, scene->cRockMarginOffsetRange.x),
		gg.rnd.Next<float>(-scene->cRockMarginOffsetRange.y, scene->cRockMarginOffsetRange.y)
	};
	pos = fixedPos + posOffset;
	centerPos = pos + scene->cRocksPivotOffset;
	flip = gg.rnd.Next<bool>();
	scene->rocksGrid.Add(indexAtGrid, this);
}

void Rock2::BeginBounce() {
	bouncing = true;
	_2 = 0;
	scale = 1.f;
}

void Rock2::Bounce() {
	// todo: stone scale anim
	// stone scale 23 frame
	static constexpr float cScaleStep{ 0.1f / (gg.cFps * 0.133333f) };
	XX_BEGIN(_2);
	for (scale.x = 1.f; scale.x < 1.1f; scale.x += cScaleStep) {
		scale.y = 2.f - scale.x;
		XX_YIELD(_2);
	}
	for (; scale.x > 0.95f; scale.x -= cScaleStep) {
		scale.y = 2.f - scale.x;
		XX_YIELD(_2);
	}
	for (; scale.x < 1.f; scale.x += cScaleStep) {
		scale.y = 2.f - scale.x;
		XX_YIELD(_2);
	}
	scale = 1.f;
	bouncing = false;
	XX_END(_2);
}

void Rock2::Update() {
	static constexpr auto esiz = gg.res.explosion_1_.size();
	static constexpr float cScaleStep{ 1.f / (gg.cFps * 0.25f) };

	XX_BEGIN(_1);
	for (scale.x = 0; scale.x < 1.f; scale.x += cScaleStep) {
		scale.y = scale.x;
		XX_YIELD(_1);
	}
	scale = 1.f;
	ready = true;
	while (true) {
		if (bouncing) {
			Bounce();
		}
		XX_YIELD(_1);
		if (digging) {
			if (pickaxe.Update()) {
				hp -= 25;
				if (hp <= cHPMax / 2) {
					tf = gg.res.rocks_[typeId][qualityId * 2];
				}
				if (hp <= 0) goto LabBreak;
				else {
					digging = false;
					BeginBounce();
				}
			}
		}
	}
LabBreak:
	ready = false;
	breaking = true;
	gg.PlayAudio(gg.ss.rockbreak);
	for (breakFrameIndex = 0.f; breakFrameIndex < esiz; breakFrameIndex += (gg.cDelta * esiz / 0.5f)) {
		XX_YIELD(_1);
	}
	++scene->rocksDisposedCountPerFrame;
	Dispose();
	return;
	XX_END(_1);
}

void Rock2::Draw() {
	XY s{ scene->cRocksScale * scale * scene->cam.scale };
	if (breaking) {
		auto& f = gg.res.explosion_1_[breakFrameIndex];
		gg.Quad().Draw(f, f, scene->cam.ToGLPos(centerPos), 0.5f, s * 3.f);
	}
	else {
		if (flip) s.x = -s.x;
		gg.Quad().Draw(tf, tf, scene->cam.ToGLPos(pos), { 0.5f, 0 }, s);
		if (digging) {
			pickaxe.Draw(scene);
		}
	}
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

void Scene_Test2::Init(float totalScale_) {
	cam.Init(gg.scale, 1.f, gg.designSize / 2);
	MakeUI();

	XYi cGridSize{ 80 * totalScale_, 15 * totalScale_ };
	auto cRockMargin = gg.designSize / cGridSize;
	cRockMarginOffsetRange = { cRockMargin / 3 };
	cRocksScale = 0.4f / totalScale_;
	cRocksMaxCount = cGridSize.x * cGridSize.y;
	cMouseCircleRadius = 128.f;
	cRocksPivotOffset = { 0, -cRockRadius * cRocksScale };

	auto cellSize = (int32_t)std::ceilf(std::max(cRockMargin.x, cRockMargin.y));
	auto numCRs = gg.designSize.As<int32_t>() / cellSize + 1;
	rocksGrid.Init(cellSize, numCRs.y, numCRs.x);
	rocks.Reserve(cRocksMaxCount);

	XY basePoss[]{ { cRockMargin.x * 0.5f, cRockMargin.y * 0.25f }, { cRockMargin.x * 0.5f, cRockMargin.y * 0.75f } };
	for (int y = 0; y < cGridSize.y; ++y) {
		for (int x = 0; x < cGridSize.x; ++x) {
			auto& basePos = basePoss[x & 1];
			rocksFixedPosPool.Emplace(basePos.x + cRockMargin.x * x, basePos.y + cRockMargin.y * y);
		}
	}
	assert(rocksFixedPosPool.len == cRocksMaxCount);

	GenRocks(cRocksMaxCount * 0.9);
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
	rocksDisposedCountPerFrame = 0;

	auto mp = cam.ToLogicPos(gg.mousePos);
	if (mp.x >= 0 && mp.y >= 0 && mp.x < rocksGrid.pixelSize.x && mp.y < rocksGrid.pixelSize.y) {
		auto total = rocks.len;
		auto cri = rocksGrid.PosToCRIndex(mp);
		auto rockRadius = 32 * cRocksScale;
		rocksGrid.ForeachByRange(cri.y, cri.x, cMouseCircleRadius + rockRadius * 3, gg.sgrdd, [&](xx::Grid2dCircle<Rock2*>::Node& node, float distance) {
			if (!node.value->ready || node.value->digging) return;
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
}

void Scene_Test2::Draw() {
	// todo: draw bg
	gg.Quad().Draw(gg.embed.shape_dot, gg.embed.shape_dot, 0, 0.5f, gg.designSize, 0, 1, { 15, 67, 11, 255 });

	// draw rocks
	for (auto& rock : rocks) {
		rock->Draw();
	}

	// draw mouse circle
	gg.Quad().Draw(gg.res.circle256, gg.res.circle256, gg.mousePos, 0.5f, cMouseCircleRadius * 0.0078125f * cam.scale);

	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene_Test2::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale * cUIScale);
	cam.SetBaseScale(gg.scale);
}
