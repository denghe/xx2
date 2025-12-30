#include "pch.h"
#include "g2.h"

namespace G2 {

	XY Box::PivotOffset() {
		auto scale = cRadius / gg.fs.wood3.uvRect.w;
		auto size = gg.fs.wood3.Size() * scale;
		auto center = size * 0.5f;
		auto pivot = size * gg.fs.wood3.anchor;
		return center - pivot;
	}

	void Box::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cRadius;
		scale = radius * 2.f / gg.fs.wood3.uvRect.w;
		radians = {};
		indexAtContainer = scene->factories.len - 1;
		assert(scene->factories[indexAtContainer].pointer == this);
		scene->gridBuildings.Add(indexAtGrid, this);
	}

	bool Box::Update() {
		if (nextEffectTime <= scene->time) {
			nextEffectTime = scene->time + 0.1f;

			// auto collect ground materials
			auto cri = scene->gridMaterials.PosToCRIndex(pos);
			static constexpr float cCollectRange{ cRadius * 2 };
			scene->gridMaterials.ForeachByRange(cri.y, cri.x, cCollectRange + 64, gg.sgrdd, [&](decltype(scene->gridMaterials)::Node& node, float range)
				->void {
					auto d = pos - node.cache.pos;
					auto mag2 = d.x * d.x + d.y * d.y;
					static constexpr float rr = cCollectRange * cCollectRange;
					if (mag2 >= rr || mag2 <= 0.0001f) return;	// not cross?
					flyingWoods.Emplace().Emplace()->Init(this, node.value);
					node.value->Dispose();
				});

			shaking = true;
			offset = {};
			radians = 0;
			_1 = _2 = 0;
		}

		if (shaking) {
			ShakeA();
			ShakeB();
		}

		for (auto i = flyingWoods.len - 1; i >= 0; --i) {
			auto& o = flyingWoods[i];
			if (o->Update()) o->Dispose();
		}

		return false;
	}

	void Box::Draw() {
		gg.Quad().DrawFrame(gg.fs.wood3, scene->cam.ToGLPos(pos + offset)
			, scale * scene->cam.scale, radians);
	}

}
