#include "pch.h"
#include "g2.h"

namespace G2 {

	void Wall::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cWallRadius;
		scale = radius * 2.f / gg.pics.circle256.uvRect.w;
		radians = {};
		indexAtContainer = scene->walls.len - 1;
		assert(scene->walls[indexAtContainer].pointer == this);
		scene->gridBuildings.Add(indexAtGrid, this);
	}

	void Wall::Draw() {
		gg.Quad().DrawFrame(gg.pics.circle256, scene->cam.ToGLPos(pos)
			, scale * scene->cam.scale, radians);
	}

	void Wall::Dispose() {
		auto i = indexAtContainer;
		assert(scene->walls[i].pointer == this);
		scene->walls.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->walls.SwapRemoveAt(i);
	}

	Wall::~Wall() {
		if (indexAtGrid > -1) {
			scene->gridBuildings.Remove(indexAtGrid, this);
			indexAtGrid = -1;
		}
	}

}
