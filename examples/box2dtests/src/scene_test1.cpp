#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

namespace Test1 {

	void SceneItem1::Init(Scene* scene_, XY pos_, XY halfSize_) {
		scene = scene_;
		halfSize = halfSize_;
		b2body.InitTypePos(scene_->b2world, pos_, b2_dynamicBody);
		b2shape.InitBox(b2body, halfSize_);
		scale = 1.f;
	}

	bool SceneItem1::Update() {
		static constexpr float cStep{ 1.f / (gg.cFps * 0.5f) };
		XX_BEGIN(_1);
		for (i = 0; i < 5; ++i) {
			for (; scale < 2.f; scale += cStep) {
				b2shape.InitBox(b2body, halfSize * scale);
				XX_YIELD_F(_1);
			}
			for (; scale > 1.f; scale -= cStep) {
				b2shape.InitBox(b2body, halfSize * scale);
				XX_YIELD_F(_1);
			}
		}
		for (; scale > 0.1f; scale -= cStep) {
			b2shape.InitBox(b2body, halfSize * scale);
			XX_YIELD_F(_1);
		}
		return true;
		XX_END(_1);
		return false;
	}

	void SceneItem1::Draw() {
		auto [p, r] = b2body.GetPosRadians();
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, scene->cam.ToGLPos(p), 0.5f, halfSize * 2.f * scale * scene->cam.scale, r);
	}

	/***************************************************************************************/

	void SceneItem2::Init(Scene* scene_, XY pos_, float radius_) {
		scene = scene_;
		radius = radius_;
		b2body.InitTypePos(scene_->b2world, pos_, b2_dynamicBody);
		b2shape.InitCircle(b2body, 0, radius_);
		scale = 1.f;
	}

	bool SceneItem2::Update() {
		static constexpr float cStep{ 1.f / (gg.cFps * 1.5f) };
		XX_BEGIN(_1);
		for (i = 0; i < 5; ++i) {
			for (; scale < 2.f; scale += cStep) {
				b2shape.InitCircle(b2body, 0, radius * scale);
				XX_YIELD_F(_1);
			}
			for (; scale > 1.f; scale -= cStep) {
				b2shape.InitCircle(b2body, 0, radius * scale);
				XX_YIELD_F(_1);
			}
		}
		for (; scale > 0.1f; scale -= cStep) {
			b2shape.InitCircle(b2body, 0, radius * scale);
			XX_YIELD_F(_1);
		}
		return true;
		XX_END(_1);
		return false;
	}

	void SceneItem2::Draw() {
		auto [p, r] = b2body.GetPosRadians();
		gg.Quad().DrawFrame(gg.fs.circle256, scene->cam.ToGLPos(p), radius * 2.f / 256.f * scale * scene->cam.scale, r);
	}

	/***************************************************************************************/

	void Scene::Init() {
		cam.Init(gg.scale, 1.f);
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		//static constexpr float cSliderWidths[]{ 600, 1000, 240 };
		//static constexpr float cMargin{ 5 };
		//static constexpr float cLineHeight{ 100 };
		//static constexpr XY cItemSize{ cSliderWidths[0] + cSliderWidths[1] + cSliderWidths[2], cLineHeight - cMargin };
		//auto fontSize = cItemSize.y - gg.embed.cfg_s9bN->paddings.TopBottom();
		//auto anchor = gg.a7;
		//auto offset = gg.p7 * 2 + XY{ cMargin, -cMargin };

		///*************************************************************************************************/

		//uiCamScale = ui->Make<xx::Slider>();
		//uiCamScale->callbackWhenBlockMoving = true;
		//uiCamScale->valueToString = [this](double v)->std::string {
		//	char buf[32];
		//	snprintf(buf, 32, "%.2f", cCamScaleRange.from + v * (cCamScaleRange.to - cCamScaleRange.from));
		//	return buf;
		//	};
		//uiCamScale->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		//	, (double)(cam.logicScale - cCamScaleRange.from) / (cCamScaleRange.to - cCamScaleRange.from))("camera scale");
		//uiCamScale->onChanged = [this](double v) {
		//	cam.SetLogicScale(cCamScaleRange.from + float(v * (cCamScaleRange.to - cCamScaleRange.from)));
		//	};

		///*************************************************************************************************/
		//offset.y -= cLineHeight;
		//uiGenAirplaneNumPerFrame = ui->Make<xx::Slider>();
		//uiGenAirplaneNumPerFrame->callbackWhenBlockMoving = true;
		//uiGenAirplaneNumPerFrame->valueToString = [this](double v)->std::string {
		//	return xx::ToString(cGenAirplaneNumPerFrameRange.from + (uint32_t(v * (cGenAirplaneNumPerFrameRange.to - cGenAirplaneNumPerFrameRange.from))));
		//	};
		//uiGenAirplaneNumPerFrame->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
		//	, (double)(cGenAirplaneNumPerFrame - cGenAirplaneNumPerFrameRange.from) / (cGenAirplaneNumPerFrameRange.to - cGenAirplaneNumPerFrameRange.from))("N gen per frame");
		//uiGenAirplaneNumPerFrame->onChanged = [this](double v) {
		//	cGenAirplaneNumPerFrame = cGenAirplaneNumPerFrameRange.from + int32_t(v * (cGenAirplaneNumPerFrameRange.to - cGenAirplaneNumPerFrameRange.from));
		//	};

		///*************************************************************************************************/
		//ui->SetAlphaRecursive(0.7f);


		b2world.InitGravity(0);
		//auto& o = items.Emplace().Emplace();
		//o->Init(this, 0, 10);
	}

	void Scene::Gen(int32_t num_) {
		for (int i = 0; i < num_; ++i) {
			XY pos;
			pos.x = gg.rnd.Next(-800, 800);
			pos.y = gg.rnd.Next(-800, 800);
			//if (gg.rnd.Next<bool>())
				//item1s.Emplace().Emplace()->Init(this, pos, 5);
			//else 
				item2s.Emplace().Emplace()->Init(this, pos, 5);
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
		for (auto i = item1s.len - 1; i >= 0; --i) {
			if (item1s[i]->Update()) item1s.SwapRemoveAt(i);
		}
		for (auto i = item2s.len - 1; i >= 0; --i) {
			if (item2s[i]->Update()) item2s.SwapRemoveAt(i);
		}
		b2world.Step();
		genTimer += gg.cDelta * 500.f;
		if (genTimer >= 1.f) {
			auto n = (int32_t)genTimer;
			genTimer -= n;
			Gen(n);
		}
	}

	void Scene::Draw() {
		for (auto& o : item1s) o->Draw();
		for (auto& o : item2s) o->Draw();

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}

}
