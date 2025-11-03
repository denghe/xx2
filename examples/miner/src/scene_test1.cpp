#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

void Scene_Test1::Init() {
	cam.Init(gg.scale, 1.f, gg.designSize / 2);
	ui.Emplace()->InitRoot(gg.scale * cUIScale);

	static constexpr float cSliderWidths[]{ 600, 1000, 240 };
	static constexpr float cMargin{ 5 };
	static constexpr float cLineHeight{ 100 };
	static constexpr XY cItemSize{ cSliderWidths[0] + cSliderWidths[1] + cSliderWidths[2], cLineHeight - cMargin };
	auto fontSize = cItemSize.y - gg.embed.cfg_s9bN->paddings.TopBottom();
	auto anchor = gg.a7;
	auto offset = gg.p7 * 2 + XY{ cMargin, -cMargin };

	/*************************************************************************************************/
	//offset.y -= cLineHeight;
	uiRocksScale = ui->Make<xx::Slider>();
	uiRocksScale->callbackWhenBlockMoving = true;
	uiRocksScale->valueToString = [](double v)->std::string {
		char buf[32];
		snprintf(buf, 32, "%.2f", cRocksScaleRange.from + v * (cRocksScaleRange.to - cRocksScaleRange.from));
		return buf;
	};
	uiRocksScale->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (double)cRocksScale / (cRocksScaleRange.to - cRocksScaleRange.from))("rocks scale");
	uiRocksScale->onChanged = [this](double v) {
		cRocksScale = v * (cRocksScaleRange.to - cRocksScaleRange.from);
	};

	/*************************************************************************************************/
	offset.y -= cLineHeight;
	uiRocksCount = ui->Make<xx::Slider>();
	uiRocksCount->callbackWhenBlockMoving = true;
	uiRocksCount->valueToString = [](double v)->std::string {
		return xx::ToString(cRocksCountRange.from + (uint32_t(v * (cRocksCountRange.to - cRocksCountRange.from))));
	};
	uiRocksCount->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		, (double)cRocksCount / (cRocksCountRange.to - cRocksCountRange.from))("rocks count");
	uiRocksCount->onChanged = [this](double v) {
		cRocksCount = int32_t(v * (cRocksCountRange.to - cRocksCountRange.from));
		GenRocks();
	};

	/*************************************************************************************************/

	XY step{ gg.designSize / cGridSize };
	XY basePoss[]{ {step.x / 2, step.y}, {step.x / 2, step.y + step.y / 2} };
	for (int y = 0; y < cGridSize.y; ++y) {
		for (int x = 0; x < cGridSize.x; ++x) {
			auto& basePos = basePoss[x & 1];
			rocksFixedPosPool.Emplace(basePos.x + step.x * x, basePos.y + step.y * y);
		}
	}

	GenRocks();
}

void Scene_Test1::GenRocks() {
	auto len = rocks.len;
	auto d = cRocksCount - len;
	if (d > 0) {
		XY step{ gg.designSize / cGridSize };
		XY offsetRange{ step / 3 };
		for (int i = 0; i < d; ++i) {
			auto& rock = rocks.Emplace().Emplace();
			auto i1 = gg.rnd.Next<int32_t>(0, gg.res.rocks_.size());
			auto i2 = gg.rnd.Next<int32_t>(2) * 2 + 1;
			rock->tf = gg.res.rocks_[i1][i2];

			auto fpIdx = gg.rnd.Next<int32_t>(rocksFixedPosPool.len);
			rock->fixedPos = rocksFixedPosPool[fpIdx];
			rocksFixedPosPool.SwapRemoveAt(fpIdx);
			XY posOffset{
				gg.rnd.Next<float>(-offsetRange.x, offsetRange.x),
				gg.rnd.Next<float>(-offsetRange.y, offsetRange.y)
			};
			rock->pos = rock->fixedPos + posOffset;
		}
	}
	else {	// d is negative
		for (int i = d; i < 0; ++i) {
			auto idx = gg.rnd.Next<int32_t>(rocks.len);
			rocksFixedPosPool.Emplace(rocks[idx]->fixedPos);
			rocks.SwapRemoveAt(idx);
		}
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
		gg.Quad().Draw(rock->tf, rock->tf
			, cam.ToGLPos(rock->pos)
			, { 0.5f, 0 }
			, cRocksScale* cam.scale);
	}

	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene_Test1::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale * cUIScale);
	cam.SetBaseScale(gg.scale);
}
