#include "pch.h"
#include "scene_test3.h"
#include "scene_mainmenu.h"

namespace Test3 {

	void Airplane::Init(Scene* scene_) {
		scene = scene_;
		auto& tracks = scene->tracks;
		auto trackIndex = gg.rnd.Next<int32_t>(tracks.len);
		radians = tracks[trackIndex].first - M_PI / 2;	// - M_PI/2: fix original radians
		track = tracks[trackIndex].second.buf;
		trackLen = tracks[trackIndex].second.len;
		trackPointIndex = 0;
		f = gg.rnd.NextElement(gg.all.airplane_);
	}

	bool Airplane::Update() {
		++trackPointIndex;
		if (trackPointIndex == trackLen) return true;
		return false;
	}

	void Airplane::Draw() {
		gg.Quad().DrawFrame(f, scene->cam.ToGLPos(track[trackPointIndex]), scene->cam.scale, radians);
	}

	/***************************************************************************************/

	void Scene::Init() {
		cam.Init(gg.scale, 0.3f);
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		static constexpr float cSliderWidths[]{ 600, 1000, 240 };
		static constexpr float cMargin{ 5 };
		static constexpr float cLineHeight{ 100 };
		static constexpr XY cItemSize{ cSliderWidths[0] + cSliderWidths[1] + cSliderWidths[2], cLineHeight - cMargin };
		auto fontSize = cItemSize.y - gg.embed.cfg_s9bN->paddings.TopBottom();
		auto anchor = gg.a7;
		auto offset = gg.p7 * 2 + XY{ cMargin, -cMargin };

		/*************************************************************************************************/

		uiCamScale = ui->Make<xx::Slider>();
		uiCamScale->callbackWhenBlockMoving = true;
		uiCamScale->valueToString = [this](double v)->std::string {
			char buf[32];
			snprintf(buf, 32, "%.2f", cCamScaleRange.from + v * (cCamScaleRange.to - cCamScaleRange.from));
			return buf;
			};
		uiCamScale->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
			, (double)(cam.logicScale - cCamScaleRange.from) / (cCamScaleRange.to - cCamScaleRange.from))("camera scale");
		uiCamScale->onChanged = [this](double v) {
			cam.SetLogicScale(cCamScaleRange.from + float(v * (cCamScaleRange.to - cCamScaleRange.from)));
			};

		/*************************************************************************************************/
		offset.y -= cLineHeight;
		uiGenAirplaneNumPerFrame = ui->Make<xx::Slider>();
		uiGenAirplaneNumPerFrame->callbackWhenBlockMoving = true;
		uiGenAirplaneNumPerFrame->valueToString = [this](double v)->std::string {
			return xx::ToString(cGenAirplaneNumPerFrameRange.from + (uint32_t(v * (cGenAirplaneNumPerFrameRange.to - cGenAirplaneNumPerFrameRange.from))));
			};
		uiGenAirplaneNumPerFrame->Init(2, offset, anchor, cItemSize.y, cSliderWidths[0], cSliderWidths[1], cSliderWidths[2]
			, (double)(cGenAirplaneNumPerFrame - cGenAirplaneNumPerFrameRange.from) / (cGenAirplaneNumPerFrameRange.to - cGenAirplaneNumPerFrameRange.from))("N gen per frame");
		uiGenAirplaneNumPerFrame->onChanged = [this](double v) {
			cGenAirplaneNumPerFrame = cGenAirplaneNumPerFrameRange.from + int32_t(v * (cGenAirplaneNumPerFrameRange.to - cGenAirplaneNumPerFrameRange.from));
			};

		/*************************************************************************************************/
		ui->SetAlphaRecursive(0.7f);

		// fill tracks
		auto w = gg.designSize.x / 2.f;
		auto r = 1.4142136f * w + 108/* tex height */ / 2.f;
		auto d = r * 2;
		auto numSteps = gg.cFps * 5;
		auto step = d / numSteps;
		tracks.Resize(360);
		for (int32_t i = 0; i < 360; ++i) {
			auto& track = tracks[i];
			track.first = i / 360.f * M_PI * 2;
			track.second.Resize(numSteps);
			auto cos = std::cosf(track.first);
			auto sin = std::sinf(track.first);
			XY inc{ -cos * step , -sin * step };
			XY posBegin{ cos * r, sin * r };
			for (int32_t j = 0; j < numSteps; ++j) {
				track.second[j] = posBegin + inc * j;
			}
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
		// drive airplane
		for (auto i = airplanes.len - 1; i >= 0; --i) {
			if (airplanes[i]->Update()) {
				airplanes.SwapRemoveAt(i);
			}
		}

		// gen airplane
		for (int32_t i = 0; i < cGenAirplaneNumPerFrame; ++i) {
			airplanes.Emplace().Emplace()->Init(this);
		}
	}

	void Scene::Draw() {
		// draw airplane
		for (auto& o : airplanes) o->Draw();

		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}

}
