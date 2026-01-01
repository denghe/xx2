#include "pch.h"
#include "scene_test5.h"
#include "scene_mainmenu.h"

namespace Test5 {

	void DroppingItem::Init(Scene* scene_, float radius_) {
		typeId = cTypeId;
		scene = scene_;
		frame = gg.rnd.NextElement(gg.fs.fishs);
		radius = radius_;
		scale = radius * 2.f / frame.uvRect.w;
		indexAtContainer = scene->droppingItems.len - 1;
		assert(scene->droppingItems[indexAtContainer].pointer == this);

		auto posFrom = scene->dropFrom + xx::GetRndPosDoughnut(gg.rnd, scene->dropFromRangeRadius);
		auto posTo = scene->dropTo + xx::GetRndPosDoughnut(gg.rnd, scene->dropToRangeRadius, scene->dropToRangeRadiusSafe);
		auto d = posTo - posFrom;
		auto mag2 = d.x * d.x + d.y * d.y;
		auto mag = std::sqrtf(mag2);
		if (mag >= cSpeed) {
			numSteps = mag / cSpeed;
			inc = d / mag * cSpeed;
			maxYOffset = mag / (float(M_SQRT2) * 2.f) * 2.f;
			scale2Ratio = maxYOffset >= 1000.f ? 1.f : maxYOffset / 1000.f;
		}
		pos = posFrom;
		y = pos.y;
		scale2 = 1.f;
	}

	bool DroppingItem::Update() {
		if (step < numSteps) {
			auto x = (float)step / numSteps;					// normalize
			x = x * float(M_SQRT2) * 2 - float(M_SQRT2);		// map to -1.41 ~ +1.41
			auto v = -0.5 * x * x + 1;							// calc parabola y
			scale2 = 1.f + v * scale2Ratio;						// make scale effect
			yOffset = -v * maxYOffset;							// map y to max val
			pos += inc;
			y = pos.y;
			++step;
			return false;
		}
		return true;
	}

	void DroppingItem::Draw() {
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(pos + XY{0, yOffset}), scale * scale2 * scene->cam.scale);
	}

	void DroppingItem::Dispose() {
		auto i = indexAtContainer;
		assert(scene->droppingItems[i].pointer == this);
		scene->droppingItems.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->droppingItems.SwapRemoveAt(i);
	}

	/***************************************************************************************************/
	/***************************************************************************************************/

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		mapSize = gg.designSize;
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);

		dropFrom = { 200, mapSize.y * 0.5f + 250.f };
		dropFromRangeRadius = 100.f;
		dropTo = { mapSize.x - 500, mapSize.y * 0.5f };
		dropToRangeRadius = 400.f;
		dropToRangeRadiusSafe = 300.f;

		genSpeed = 0.1f;
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
		for (auto i = droppingItems.len - 1; i >= 0; --i) {
			auto& o = droppingItems[i];
			if (o->Update()) o->Dispose();
		}

		// gen
		if (gg.keyboard[GLFW_KEY_SPACE](0.2f)) {
			genSpeed *= 2.f;
			if (genSpeed >= 500.f) genSpeed = 500.f;
		}

		genPool += genSpeed;
		while (genPool >= 1.f) {
			genPool -= 1.f;
			droppingItems.Emplace().Emplace()->Init(this, 64.f);
		}
	}

	void Scene::Draw() {
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, 0, 0.5f, gg.windowSize, 0, 1, { 0x81,0xbd,0x57,255 });
		gg.Quad().DrawFrame(gg.fs.circle256, cam.ToGLPos(dropFrom), dropFromRangeRadius * 2.f / 256.f * cam.scale);
		gg.Quad().DrawFrame(gg.fs.circle256, cam.ToGLPos(dropTo), dropToRangeRadius * 2.f / 256.f * cam.scale);

		// sort order by y. map y to 0 ~ 1080 ( design size.y ). FPS 3x faster than std::sort
		for (auto& o : droppingItems) SortContainerAdd(o.pointer);
		SortContainerDraw();

		gg.uiText->SetText(xx::ToString("num droppingItems = ", droppingItems.len));
		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}



	XX_INLINE void Scene::SortContainerAdd(SceneItem* o_) {
		auto& slot = sortContainer[(int32_t)o_->y];
		o_->next = slot;
		slot = o_;
	}

	XX_INLINE void Scene::SortContainerDraw() {
		for (auto o : sortContainer) {
			while (o) {
				o->Draw();
				o = o->next;
			}
		}
		memset(sortContainer.buf, 0, sortContainer.len * sizeof(void*));
	}
}
