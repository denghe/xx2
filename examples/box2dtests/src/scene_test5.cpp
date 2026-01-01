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

		auto posFrom = scene->dropFromPos + xx::GetRndPosDoughnut(gg.rnd, scene->dropFromRangeRadius);
		auto posTo = scene->dropToPos + xx::GetRndPosDoughnut(gg.rnd, scene->dropToRangeRadius, scene->dropToRangeRadiusSafe);
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
		scene->vortexItems.Emplace().Emplace()->Init(this);

		auto i = indexAtContainer;
		assert(scene->droppingItems[i].pointer == this);
		scene->droppingItems.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->droppingItems.SwapRemoveAt(i);
	}

	/***************************************************************************************************/
	/***************************************************************************************************/

	void VortexItem::Init(DroppingItem* tar_) {
		typeId = cTypeId;
		scene = tar_->scene;
		frame = tar_->frame;
		radius = tar_->radius;
		scale = tar_->scale;
		indexAtContainer = scene->vortexItems.len - 1;
		assert(scene->vortexItems[indexAtContainer].pointer == this);

		pos = tar_->pos;
		y = pos.y;
		center = scene->dropToPos;
		auto v = pos - center;
		angle = std::atan2f(v.y, v.x);
		angleSpeed = float(M_PI) * 2.f / 3.f / gg.cFps;
		angleAccel = angleSpeed / 100.f;
		maxDistance = distance = std::sqrtf(v.x * v.x + v.y * v.y);
		distanceReduceStep = distance / (gg.cFps * 5.f);
		scale2 = 1.f;
	}

	bool VortexItem::Update() {
		if (distance >= distanceReduceStep) {
			XY inc{ std::cosf(angle),std::sinf(angle) };
			pos = center + inc * distance;
			y = pos.y;
			distance -= distanceReduceStep;
			scale2 = 0.2f + distance / maxDistance * 0.8f;
			angle += angleSpeed;
			angleSpeed += angleAccel;
			return false;
		}
		return true;
	}

	void VortexItem::Draw() {
		gg.Quad().DrawFrame(frame, scene->cam.ToGLPos(pos), scale * scale2 * scene->cam.scale);
	}

	void VortexItem::Dispose() {
		auto i = indexAtContainer;
		assert(scene->vortexItems[i].pointer == this);
		scene->vortexItems.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->vortexItems.SwapRemoveAt(i);
	}

	/***************************************************************************************************/
	/***************************************************************************************************/

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		mapSize = gg.designSize * 2.f;
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);

		dropFromPos = { 700, mapSize.y - 300.f };
		dropFromRangeRadius = 100.f;
		dropToPos = { mapSize.x - 1100, mapSize.y - 600.f };
		dropToRangeRadius = 400.f;
		dropToRangeRadiusSafe = 300.f;

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
		for (auto i = vortexItems.len - 1; i >= 0; --i) {
			auto& o = vortexItems[i];
			if (o->Update()) o->Dispose();
		}

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
			droppingItems.Emplace().Emplace()->Init(this, 64.f);
		}
#else
		if (gg.keyboard[GLFW_KEY_SPACE](0.2f)) {
			droppingItems.Emplace().Emplace()->Init(this, 64.f);
		}
#endif
	}

	void Scene::Draw() {
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, 0, 0.5f, gg.windowSize, 0, 1, { 0x81,0xbd,0x57,255 });
		gg.Quad().DrawFrame(gg.fs.circle256, cam.ToGLPos(dropFromPos), dropFromRangeRadius * 2.f / 256.f * cam.scale);
		gg.Quad().DrawFrame(gg.fs.circle256, cam.ToGLPos(dropToPos), dropToRangeRadius * 2.f / 256.f * cam.scale);

		// sort order by y. map y to 0 ~ 1080 ( design size.y ). FPS 3x faster than std::sort
		for (auto& o : droppingItems) SortContainerAdd(o.pointer);
		for (auto& o : vortexItems) SortContainerAdd(o.pointer);
		SortContainerDraw();

		gg.uiText->SetText(xx::ToString("num items = ", droppingItems.len + vortexItems.len));
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
