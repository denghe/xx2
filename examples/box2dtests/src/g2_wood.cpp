#include "pch.h"
#include "g2.h"

namespace G2 {

	void Wood::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cWoodRadius;
		scale = radius * 2.f / gg.pics.wood2.uvRect.w;
		indexAtContainer = scene->woods.len - 1;
		assert(scene->woods[indexAtContainer].pointer == this);
	}

	void Wood::Anim() {

		XX_BEGIN(_1);
		{
			auto r = gg.rnd.Next(M_PI * 2);
			cos = std::cosf(r) * scale;
			sin = std::sinf(r) * scale;
		}
		for (i = 0; i < _countof(cDistances25); ++i) {
			offset.x += cos * cDistances25[i];
			offset.y += sin * cDistances25[i];
			y = pos.y + offset.y;
			XX_YIELD(_1);
		}
		ready = true;
		pos += offset;
		y = pos.y;
		scene->gridMaterials.Add(indexAtGrid, this);
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

	void Wood::PreUpdate() {
		if (!ready) return;
		static constexpr float cSpeed{ 5.f };
		auto& cache = scene->gridMaterials.NodeAt(indexAtGrid).cache;
		auto cri = scene->gridMaterials.PosToCRIndex(cache.pos);
		auto limit{ 3 };
		scene->gridMaterials.ForeachBy9(cri.y, cri.x, [&](decltype(scene->gridMaterials)::Node& node, float range)
			->bool {
				if (node.value == this) return false;	// is self?
				auto d = cache.pos - node.cache.pos;
				auto mag2 = d.x * d.x + d.y * d.y;
				auto r = cache.radius + node.cache.radius;
				auto rr = r * r;
				if (mag2 >= rr || mag2 <= 0.0001f) return false;	// not cross?
				auto mag = std::sqrtf(mag2);
				auto a = 0.5f * (r - mag);
				auto spd = d / mag * a;
				if (spd.x > cSpeed) spd.x = cSpeed;
				else if (spd.x < -cSpeed) spd.x = -cSpeed;
				if (spd.y > cSpeed) spd.y = cSpeed;
				else if (spd.y < -cSpeed) spd.y = -cSpeed;
				cache.pos += spd;
				//y = pos.y;
				//scene->gridMaterials.Update(indexAtGrid, this);
				node.cache.pos -= spd;
				//o->y = o->pos.y;
				//scene->gridMaterials.Update(indexAtGrid, o);
				return --limit == 0;
			});

		cri = scene->gridBuildings.PosToCRIndex(cache.pos);
		scene->gridBuildings.ForeachBy9(cri.y, cri.x, [&](decltype(scene->gridBuildings)::Node& node, float range)
			->void {
				auto d = cache.pos - node.cache.pos;
				auto mag2 = d.x * d.x + d.y * d.y;
				auto r = node.cache.radius + cache.radius;
				auto rr = r * r;
				if (mag2 < rr) {	// cross?
					if (mag2 > 0) {
						auto mag = std::sqrtf(mag2);
						auto norm = d / mag;
						cache.pos = node.cache.pos + norm * r;	// force move away
					}
				}
			});
	}

	bool Wood::Update() {
		Anim();
		if (!ready) return false;
		auto& cache = scene->gridMaterials.NodeAt(indexAtGrid).cache;
		if (cache.pos != pos) {
			pos = cache.pos;
			scene->gridMaterials.Update(indexAtGrid, this);
			y = pos.y;
		}
		return false;
	}

	void Wood::Draw() {
		gg.Quad().DrawFrame(gg.pics.wood2, scene->cam.ToGLPos(pos + offset), scale * scene->cam.scale);
	}

	void Wood::Dispose() {
		auto i = indexAtContainer;
		assert(scene->woods[i].pointer == this);
		scene->woods.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->woods.SwapRemoveAt(i);
	}

	Wood::~Wood() {
		if (indexAtGrid > -1) {
			scene->gridMaterials.Remove(indexAtGrid, this);
			indexAtGrid = -1;
		}
	}

}
