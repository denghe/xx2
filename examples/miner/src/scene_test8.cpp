#include "pch.h"
#include "scene_test8.h"
#include "scene_mainmenu.h"

namespace Test8 {

Rock& Rock::Init(Scene * scene_, XY pos_, float radius_) {
	scene = scene_;
	pos = pos_;
	y = pos.y;
	radius = radius_;
	return *this;
}

void Rock::Update() {
	// todo: color white effect
	// todo: hit bounce effect
}

void Rock::Draw() {
	auto& c = scene->cam;
	auto& f = gg.tf.rocks_[0][0];
	gg.Quad().Draw(f, f, c.ToGLPos(pos), { 0.5f, 0.1f }, radius / 64.f * c.scale);
}


/***************************************************************************************/

void Scene::Init() {
	cam.Init(gg.scale, 1.f, gg.designSize/2);
	ui.Emplace()->InitRoot(gg.scale * cUIScale);

	// todo: gen rock pos from image
	auto& img = gg.stbi.bg1a;
	assert(img.comp == 4);
	auto siz = img.Size();
	auto s = 1.f / (gg.designSize / siz);
	auto cs = (xx::RGBA8*)img.buf;

	float totalScale_{ 1.f };
	XYi cGridSize{ 80 * totalScale_, 15 * totalScale_ };
	cRockMargin = gg.designSize / cGridSize;
	cRockMarginOffsetRange = { cRockMargin / 4 };
	cRocksScale = 0.4f / totalScale_;
	cRocksMaxCount = cGridSize.x * cGridSize.y;
	cMouseCircleRadius = 128.f;
	cRocksPivotOffset = { 0, -cRockRadius * cRocksScale };

	auto cellSize = (int32_t)std::ceilf(std::max(cRockMargin.x, cRockMargin.y));
	auto numCRs = gg.designSize.As<int32_t>() / cellSize + 1;
	//rocksGrid.Init(cellSize, numCRs.y, numCRs.x);
	rocks.Reserve(cRocksMaxCount);

	XY basePoss[]{ { cRockMargin.x * 0.5f, cRockMargin.y * 0.25f }, { cRockMargin.x * 0.5f, cRockMargin.y * 0.75f } };
	for (int y = 0; y < cGridSize.y; ++y) {
		for (int x = 0; x < cGridSize.x; ++x) {
			auto& basePos = basePoss[x & 1];
			XY pos{ basePos.x + cRockMargin.x * x, basePos.y + cRockMargin.y * y };

			// todo: check img color
			auto cidx = int32_t(pos.y * s.y * img.w + pos.x * s.x);
			auto c = cs[cidx];
			if (c.a) {
				rocksFixedPosPool.Emplace(pos);
			}
		}
	}
	assert(rocksFixedPosPool.len <= cRocksMaxCount);

	for (auto& o : rocksFixedPosPool) {
		rocks.Emplace().Emplace()->Init(this, o, 32.f);
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
	time += d;
	timePool += d;
	while (timePool >= gg.cDelta) {
		timePool -= gg.cDelta;
		FixedUpdate();
	}
}

void Scene::FixedUpdate() {
	if (timer <= time) {
		timer += 2.f;
		std::sort((OrderByYItem**)rocks.buf, (OrderByYItem**)rocks.buf + rocks.len, [](auto& a, auto& b) { return a->y < b->y; });
	}

	for (auto& o : rocks) o->Update();
}

void Scene::Draw() {
	// bg
	gg.Quad().Draw(gg.tf.bg1, gg.tf.bg1, 0, 0.5f, gg.designSize.y / gg.tf.bg1.Size().y * cam.scale, 0, 0.5f);

	// sort order by y
	assert(obyis.Empty());
	for (auto& o : rocks) obyis.Emplace(o->y, o.pointer);
	std::sort(obyis.buf, obyis.buf + obyis.len, [](auto& a, auto& b) { return a.first < b.first; });

	// draw by y
	for (auto& o : obyis) o.second->Draw();
	obyis.Clear();

	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale * cUIScale);
	cam.SetBaseScale(gg.scale);
}

}
