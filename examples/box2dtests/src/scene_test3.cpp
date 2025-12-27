#include "pch.h"
#include "scene_test3.h"
#include "scene_mainmenu.h"

namespace Test3 {

	void Wood1::Init(Scene* scene_, XY pos_, float scale_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		scale = scale_;
		radians = {};
	}

	void Wood1::ShakeA() {
		XX_BEGIN(_1);
		{
			auto r = gg.rnd.Next(M_PI * 2);
			cos = std::cosf(r) * scale * 0.5f;
			sin = std::sinf(r) * scale * 0.5f;
		}
		for (i = 0; i < _countof(cDistances); ++i) {
			offset.x = cos * cDistances[i];
			offset.y = sin * cDistances[i];
			XX_YIELD(_1);
		}
		shaking = false;
		offset = {};
		radians = 0;
		XX_END(_1);
	}

	void Wood1::ShakeB() {
		XX_BEGIN(_2);
		for (j = 0; j < _countof(cDistances); ++j) {
			radians = cDistances[j] * 0.029f;
			XX_YIELD(_2);
		}
		shaking = false;
		XX_END(_2);
	}

	bool Wood1::Update() {
		if (gg.mouse[GLFW_MOUSE_BUTTON_1](0.1)) {
			shaking = true;
			offset = {};
			radians = 0;
			_1 = _2 = 0;
			scene->woods.Emplace().Emplace()->Init(scene, pos, scale);
		}

		if (shaking) {
			ShakeA();
			ShakeB();
		}

		return false;
	}

	void Wood1::Draw() {
		// todo: shadow ?
		gg.Quad().DrawFrame(gg.fs.wood1, scene->cam.ToGLPos(pos + offset)
			, scale * scene->cam.scale, radians);
	}

	/***************************************************************************************/


	void Wood2::Init(Scene* scene_, XY pos_, float scale_) {
		scene = scene_;
		pos = pos_;
		y = pos.y;
		scale = scale_;
		indexAtContainer = scene->woods.len - 1;
		assert(scene->woods[indexAtContainer].pointer == this);
	}

	void Wood2::Anim() {
		static constexpr float cD{ 1.2f };
		static constexpr float cDistances[]{
			cD + .9f,cD + .8f,cD + .7f,cD + .6f,cD + .5f,cD + .4f,cD + .3f,cD + .2f,cD + .1f,cD + .0f,
			cD - .1f,cD - .2f,cD - .3f,cD - .4f,cD - .5f,cD - .6f,cD - .7f,cD - .8f,cD - .9f,cD - 1.f,
			cD - 1.1f, cD - 1.15f, cD - 1.175f, cD - 1.19f, 0
		};
		static constexpr float cIdleYMaxOffset{ 2.f };
		static constexpr float cIdleYStep{ cIdleYMaxOffset / (gg.cFps * 0.58f) };

		XX_BEGIN(_1);
		{
			auto r = gg.rnd.Next(M_PI * 2);
			cos = std::cosf(r) * scale;
			sin = std::sinf(r) * scale;
		}
		for (i = 0; i < _countof(cDistances); ++i) {
			offset.x += cos * cDistances[i];
			offset.y += sin * cDistances[i];
			y = pos.y + offset.y;
			XX_YIELD(_1);
		}
		ready = true;
		pos += offset;
		y = pos.y;
		scene->grid.Add(indexAtGrid, this);
		offset = {};
		// idle
		while (true) {
			for (; offset.y < cIdleYMaxOffset * scale; offset.y += cIdleYStep * scale) {
				XX_YIELD(_1);
			}
			for (; offset.y > 0.f; offset.y -= cIdleYStep * scale) {
				XX_YIELD(_1);
			}
		}
		XX_END(_1);
	}

	bool Wood2::Update() {
		Anim();
		if (ready) {
			// todo: phys simu
			scene->grid.Update(indexAtGrid, this);
		}
		return false;
	}

	void Wood2::Draw() {
		// todo: shadow ?
		gg.Quad().DrawFrame(gg.fs.wood2, scene->cam.ToGLPos(pos + offset), scale * scene->cam.scale);
	}

	void Wood2::Dispose() {
		if (indexAtGrid > -1) {
			scene->grid.Remove(indexAtGrid, this);
			indexAtGrid = -1;
		}
		auto i = indexAtContainer;
		assert(scene->woods[i].pointer == this);
		scene->woods.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->woods.SwapRemoveAt(i);
	}

	/***************************************************************************************/

	void Scene::Init() {
		mapSize = 2000;
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);
		grid.Init(130, mapSize.y / 130 + 1, mapSize.x / 130 + 1);
		wood1.Init(this, cam.original, 10);

		ui.Emplace()->InitRoot(gg.scale * cUIScale);
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
		wood1.Update();

		for (auto i = woods.len - 1; i >= 0; --i) {
			auto& o = woods[i];
			if (o->Update()) o->Dispose();
		}
	}

	void Scene::Draw() {
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, 0, 0.5f, gg.windowSize, 0, 1, {0x81,0xbd,0x57,255});

		// sort order by y. map y to 0 ~ 1080 ( design size.y ). FPS 3x faster than std::sort
		for (auto& o : woods) SortContainerAdd(o.pointer);
		SortContainerAdd(&wood1);
		SortContainerDraw();

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale * cUIScale);
		cam.SetBaseScale(gg.scale);
	}



	XX_INLINE void Scene::SortContainerAdd(SceneItem* o) {
		auto& slot = sortContainer[(int32_t)o->y];
		o->next = slot;
		slot = o;
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
