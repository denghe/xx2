#include "pch.h"
#include "g2.h"

namespace G2 {

	XY WoodFactory::PivotOffset() {
		auto scale = cFactoryRadius / gg.pics.wood1.uvRect.w;
		auto size = gg.pics.wood1.Size() * scale;
		auto center = size * 0.5f;
		auto pivot = size * gg.pics.wood1.anchor;
		return center - pivot;
	}

	void WoodFactory::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cFactoryRadius;
		scale = radius * 2.f / gg.pics.wood1.uvRect.w;
		radians = {};
		indexAtContainer = scene->factories.len - 1;
		assert(scene->factories[indexAtContainer].pointer == this);
		scene->gridBuildings.Add(indexAtGrid, this);
	}

	void WoodFactory::ShakeA() {
		XX_BEGIN(_1);
		{
			auto r = gg.rnd.Next(M_PI * 2);
			cos = std::cosf(r) * scale * 0.5f;
			sin = std::sinf(r) * scale * 0.5f;
		}
		for (i = 0; i < _countof(cDistances12); ++i) {
			offset.x = cos * cDistances12[i];
			offset.y = sin * cDistances12[i];
			XX_YIELD(_1);
		}
		shaking = false;
		offset = {};
		radians = 0;
		XX_END(_1);
	}

	void WoodFactory::ShakeB() {
		XX_BEGIN(_2);
		for (j = 0; j < _countof(cDistances12); ++j) {
			radians = cDistances12[j] * 0.029f;
			XX_YIELD(_2);
		}
		shaking = false;
		XX_END(_2);
	}

	bool WoodFactory::Update() {
		if (gg.mouse[GLFW_MOUSE_BUTTON_1]) {
			if (scene->woods.len < 30000) {				// todo
				if (nextEffectTime <= scene->time) {
					nextEffectTime = scene->time + 0.01f;
					shaking = true;
					offset = {};
					radians = 0;
					_1 = _2 = 0;
					scene->woods.Emplace().Emplace()->Init(scene, pos);
				}
			}
		}

		if (shaking) {
			ShakeA();
			ShakeB();
		}

		return false;
	}

	void WoodFactory::Draw() {
		gg.Quad().DrawFrame(gg.pics.wood1, scene->cam.ToGLPos(pos + offset)
			, scale * scene->cam.scale, radians);
	}

	void WoodFactory::Dispose() {
		auto i = indexAtContainer;
		assert(scene->factories[i].pointer == this);
		scene->factories.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->factories.SwapRemoveAt(i);
	}

	WoodFactory::~WoodFactory() {
		if (indexAtGrid > -1) {
			scene->gridBuildings.Remove(indexAtGrid, this);
			indexAtGrid = -1;
		}
	}

}
