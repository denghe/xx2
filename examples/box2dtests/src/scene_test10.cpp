#include "pch.h"
#include "scene_test10.h"
#include "scene_mainmenu.h"

namespace Test10 {
	static constexpr XY xyRatio{ 1.f, 0.3f };

	void DroppingItem::Init(Scene* scene_) {
		typeId = cTypeId;
		scene = scene_;
		frame = *std::get<0>(gg.rnd.NextElement(gg.rocksFramePhysMaps));
		radius = gg.rnd.Next(8.f,24.f);
		radians = gg.rnd.Next<float>(-M_PI, M_PI);
		flipX = gg.rnd.Next<bool>();
		scale = radius * 2.f / frame.uvRect.w;
		indexAtContainer = scene->droppingItems.len - 1;
		assert(scene->droppingItems[indexAtContainer].pointer == this);

		auto maxRadius_ = scene->dropFromRangeRadius;
		auto safeRadius_ = scene->dropFromRangeRadiusSafe;
		auto len = maxRadius_ - safeRadius_;
		auto len_radius = len / maxRadius_;
		auto safeRadius_radius = safeRadius_ / maxRadius_;
		auto radius = std::sqrtf(gg.rnd.Next<float>(0, len_radius) + safeRadius_radius) * maxRadius_;

		maxRadius_ = scene->dropToRangeRadius;
		safeRadius_ = scene->dropToRangeRadiusSafe;
		len = maxRadius_ - safeRadius_;
		len_radius = len / maxRadius_;
		safeRadius_radius = safeRadius_ / maxRadius_;
		auto radius2 = std::sqrtf(gg.rnd.Next<float>(0, len_radius) + safeRadius_radius) * maxRadius_;

		auto radians = gg.rnd.Next<float>(-M_PI, M_PI);
		XY cossin{ std::cosf(radians), std::sinf(radians) };

		auto posFrom = scene->dropFromPos + cossin * radius * xyRatio;
		auto posTo = scene->dropToPos + cossin * radius2 * xyRatio;

		auto d = posTo - posFrom;
		auto mag2 = d.x * d.x + d.y * d.y;
		auto mag = std::sqrtf(mag2);
		if (mag >= cSpeed) {
			numSteps = mag / cSpeed;
			inc = d / mag * cSpeed;
			maxYOffset = mag / (float(M_SQRT2) * 2.f) * (gg.rnd.Next(1.f, 2.f));
			//scale2Ratio = maxYOffset >= 1000.f ? 1.f : maxYOffset / 1000.f;
			scale2Ratio = 0.2;
		}
		pos = posFrom;
		y = pos.y;
		scale2 = 1.f;
		alpha = 1.f;
	}

	bool DroppingItem::Update() {
		XX_BEGIN(_1);

		// jump up & down
		while (step < numSteps) {
			{
				auto x = (float)step / numSteps;					// normalize
				x = x * float(M_SQRT2) * 2 - float(M_SQRT2);		// map to -1.41 ~ +1.41
				auto v = -0.5 * x * x + 1;							// calc parabola y
				scale2 = 1.f + v * scale2Ratio;						// make scale effect
				yOffset = -v * maxYOffset;							// map y to max val
			}
			pos += inc;
			y = pos.y;
			++step;
			XX_YIELD_F(_1);
		}
		step = 0;
		
		// half height jump again
		if (numSteps > 12) {
			numSteps *= 0.4f;
			maxYOffset *= 0.4f;
			scale2Ratio *= 0.4f;
			XX_YIELD_F_TO_BEGIN(_1);
		}
		scale2 = 1.f;

		// begin slide & fade out
		while (step < 20) {
			pos += inc;
			y = pos.y;
			alpha = 1.f - step / 60.f;
			++step;
			XX_YIELD_F(_1);
		}

		// fade out
		while (step < 60) {
			alpha = 1.f - step / 60.f;
			++step;
			XX_YIELD_F(_1);
		}

		XX_END(_1);
		return true;
	}

	void DroppingItem::Draw() {
		XY s{ scale * scale2 * scene->cam.scale };
		if (flipX) s.x = -s.x;
		auto a = uint8_t(255.f * alpha);
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(pos + XY{0, yOffset})
			, s, radians, 1.f, {a,a,a,a});
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

		mapSize = gg.designSize * 2.f;
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);

		dropFromPos = mapSize / 2;
		dropFromRangeRadius = 100.f;
		dropFromRangeRadiusSafe = 80.f;
		dropToPos = mapSize / 2;
		dropToRangeRadius = 300.f;
		dropToRangeRadiusSafe = 120.f;

		genSpeed = 0.01f;
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

#if 1
		// gen
		if (gg.keyboard[GLFW_KEY_SPACE](0.2f)) {
			genSpeed *= 2.f;
			if (genSpeed >= 200.f) genSpeed = 200.f;
		}

		genPool += genSpeed;
		while (genPool >= 1.f) {
			genPool -= 1.f;
			droppingItems.Emplace().Emplace()->Init(this);
		}
#else
		if (gg.keyboard[GLFW_KEY_SPACE](0.2f)) {
			droppingItems.Emplace().Emplace()->Init(this);
		}
#endif
	}

	void Scene::Draw() {
		// bg color
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, 0, 0.5f, gg.windowSize, 0, 1, { 0x81,0xbd,0x57,255 });

		gg.Quad().DrawFrame(gg.pics.circle256, cam.ToGLPos(dropFromPos)
			, dropFromRangeRadius * 2.f / 256.f * cam.scale * xyRatio);
		gg.Quad().DrawFrame(gg.pics.circle256, cam.ToGLPos(dropToPos)
			, dropToRangeRadius * 2.f / 256.f * cam.scale * xyRatio);

		// sort order by y. map y to 0 ~ 1080 ( design size.y ). FPS 3x faster than std::sort
		for (auto& o : droppingItems) SortContainerAdd(o.pointer);
		SortContainerDraw();

		gg.uiText->SetText(xx::ToString("num items = ", droppingItems.len));
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
