#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

namespace Test1 {

	/***************************************************************************************/

	void Scene::Init() {
		cam.Init(gg.scale, 0.3f);
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


		b2world.InitGravity({ 0, -10.f });
		b2body.InitBox(b2world, b2_dynamicBody, { 0,0 }, { 10,10 });
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
		auto p = b2Body_GetPosition(b2body);
		xx::CoutN(p.x, " ", p.y);
	}

	void Scene::Draw() {

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}

}
