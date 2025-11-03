#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

void Scene_Test1::Init() {
	cam.Init(gg.scale, 1.f, gg.designSize / 2);
	ui.Emplace()->InitRoot(gg.scale);

	static constexpr XYi cGridSize{ 40, 25 };
	XY step{ gg.designSize / cGridSize };
	XY basePoss[]{ {step.x / 2, step.y}, {step.x / 2, step.y + step.y / 2} };
	for (int y = 0; y < cGridSize.y; ++y) {
		for (int x = 0; x < cGridSize.x; ++x) {
			auto& basePos = basePoss[x & 1];
			fixedPosPool.Emplace(basePos.x + step.x * x, basePos.y + step.y * y);
		}
	}

	XY offsetRange{ step / 3 };
	auto count = cGridSize.x * cGridSize.y;// / 2;
	for (int i = 0; i < count; ++i) {
		auto& rock = rocks.Emplace().Emplace();
		auto i1 = gg.rnd.Next<int32_t>(0, gg.res.rocks_.size());
		auto i2 = gg.rnd.Next<int32_t>(2) * 2 + 1;
		rock->tf = gg.res.rocks_[i1][i2];

		auto fpIdx = gg.rnd.Next<int32_t>(fixedPosPool.len);
		rock->fixedPos = fixedPosPool[fpIdx];
		fixedPosPool.SwapRemoveAt(fpIdx);
		XY posOffset{
			gg.rnd.Next<float>(-offsetRange.x, offsetRange.x),
			gg.rnd.Next<float>(-offsetRange.y, offsetRange.y)
		};
		rock->pos = rock->fixedPos + posOffset;
	}
	std::sort(rocks.buf, rocks.buf + rocks.len, [](auto& a, auto& b)->bool {
		return a->pos.y < b->pos.y;
	});
}

void Scene_Test1::Update() {
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

void Scene_Test1::FixedUpdate() {
}

void Scene_Test1::Draw() {
#if 0
	auto& t = *gg.res.pickaxe_[0].tex;
	gg.Quad().Draw(t, t.Rect(), 0, 0.5f, cam.scale);
#endif
	for (auto& rock : rocks) {
		gg.Quad().Draw(rock->tf, rock->tf, cam.ToGLPos(rock->pos), { 0.5f, 0 }, rocksScale * cam.scale);
	}

	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene_Test1::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
