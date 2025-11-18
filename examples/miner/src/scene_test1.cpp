#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

namespace Test1 {

	void Scene::Init() {
		cam.Init(gg.scale, 1.f, gg.designSize / 2);
		MakeUI();
		GenAll();
	}

	void Scene::MakeUI() {
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
		uiGridSizeY = ui->Make<xx::Slider>();
		uiGridSizeY->callbackWhenBlockMoving = true;
		uiGridSizeY->valueToString = [](double v)->std::string {
			return xx::ToString(cGridSizeRange.from.y + (uint32_t(v * (cGridSizeRange.to.y - cGridSizeRange.from.y))));
			};
		uiGridSizeY->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
			, (double)cGridSize.y / (cGridSizeRange.to.y - cGridSizeRange.from.y))("row count");
		uiGridSizeY->onChanged = [this](double v) {
			cGridSize.y = int32_t(v * (cGridSizeRange.to.y - cGridSizeRange.from.y));
			GenAll();
			};

		/*************************************************************************************************/
		offset.y -= cLineHeight;
		uiGridSizeX = ui->Make<xx::Slider>();
		uiGridSizeX->callbackWhenBlockMoving = true;
		uiGridSizeX->valueToString = [](double v)->std::string {
			return xx::ToString(cGridSizeRange.from.x + (uint32_t(v * (cGridSizeRange.to.x - cGridSizeRange.from.x))));
			};
		uiGridSizeX->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
			, (double)cGridSize.x / (cGridSizeRange.to.x - cGridSizeRange.from.x))("column count");
		uiGridSizeX->onChanged = [this](double v) {
			cGridSize.x = int32_t(v * (cGridSizeRange.to.x - cGridSizeRange.from.x));
			GenAll();
			};

		/*************************************************************************************************/
		offset.y -= cLineHeight;
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
		uiRocksCount->valueToString = [this](double v)->std::string {
			return xx::ToString(cRocksCountRange.from + (uint32_t(v * (cRocksCountRange.to - cRocksCountRange.from))));
			};
		uiRocksCount->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
			, (double)cRocksCount / (cRocksCountRange.to - cRocksCountRange.from))("rocks count");
		uiRocksCount->onChanged = [this](double v) {
			cRocksCount = int32_t(v * (cRocksCountRange.to - cRocksCountRange.from));
			GenRocks();
			};

		/*************************************************************************************************/
		offset.y -= cLineHeight;
		uiEnableRandomOffset = ui->Make<xx::CheckBox>();
		uiEnableRandomOffset->Init(2, offset, anchor, cItemSize, cEnableRandomOffset)("enable random offset");
		uiEnableRandomOffset->onValueChanged = [this](bool v)->void {
			cEnableRandomOffset = v;
			SetRandomOffset();
			};

		/*************************************************************************************************/

		ui->SetAlphaRecursive(0.7f);
	}

	void Scene::GenRocksFixedPosPool() {
		XY step{ gg.designSize / cGridSize };
		XY basePoss[]{ { step.x / 2, step.y / 2 }, { step.x / 2, step.y } };
		for (int y = 0; y < cGridSize.y; ++y) {
			for (int x = 0; x < cGridSize.x; ++x) {
				auto& basePos = basePoss[x & 1];
				rocksFixedPosPool.Emplace(basePos.x + step.x * x, basePos.y + step.y * y);
			}
		}
	}

	void Scene::GenRocks() {
		auto len = rocks.len;
		auto d = cRocksCount - len;
		if (d > 0) {
			XY step{ gg.designSize / cGridSize };
			XY offsetRange{ step / 3 };
			for (int i = 0; i < d; ++i) {
				auto& rock = rocks.Emplace().Emplace();
				auto i1 = gg.rnd.Next<int32_t>(0, gg.tf.rocks_.size());
				static constexpr int32_t cIdxs[]{ 1,3,4 };
				auto i2 = cIdxs[gg.rnd.Next<int32_t>(3)];
				rock->tf = gg.tf.rocks_[i1][i2];

				auto fpIdx = gg.rnd.Next<int32_t>(rocksFixedPosPool.len);
				rock->fixedPos = rocksFixedPosPool[fpIdx];
				rocksFixedPosPool.SwapRemoveAt(fpIdx);
				rock->pos = rock->fixedPos;
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

	void Scene::SetRandomOffset() {
		if (cEnableRandomOffset) {
			XY step{ gg.designSize / cGridSize };
			XY offsetRange{ step / 3 };
			for (auto& rock : rocks) {
				XY posOffset{
					gg.rnd.Next<float>(-offsetRange.x, offsetRange.x),
					gg.rnd.Next<float>(-offsetRange.y, offsetRange.y)
				};
				rock->pos = rock->fixedPos + posOffset;
			}
		}
		else {
			for (auto& rock : rocks) {
				rock->pos = rock->fixedPos;
			}
		}
	}

	void Scene::GenAll() {
		rocks.Clear();
		rocksFixedPosPool.Clear();
		cRocksCountRange.to = cGridSize.x * cGridSize.y;
		cRocksCount = std::min(cRocksCount, cRocksCountRange.to);
		if (cRocksCountRange.to > 0) {
			uiRocksCount->SetValue((double)cRocksCount / (cRocksCountRange.to - cRocksCountRange.from));
			uiRocksCount->ApplyValue();
			GenRocksFixedPosPool();
			GenRocks();
		}
		else {
			uiRocksCount->SetValue(0);
			uiRocksCount->ApplyValue();
		}
		SetRandomOffset();
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
	}

	void Scene::Draw() {
#if 0
		auto& t = *gg.res.pickaxe_[0].tex;
		gg.Quad().Draw(t, t.Rect(), 0, 0.5f, cam.scale);
#endif
		for (auto& rock : rocks) {
			gg.Quad().Draw(rock->tf, rock->tf
				, cam.ToGLPos(rock->pos)
				, { 0.5f, 0 }
			, cRocksScale * cam.scale);
		}

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}

}
