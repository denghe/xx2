#include "pch.h"
#include "scene_test9.h"
#include "scene_mainmenu.h"

namespace Test9 {

CartRock& CartRock::Init(Scene * scene_, XY pos_) {
	scene = scene_;
	pos = pos_;
	y = pos.y;
	typeId = gg.rnd.Next(gg.tf.rocks_.size());
	return *this;
}

void CartRock::Draw() {
	auto& c = scene->cam;
	auto& f = gg.tf.rocks_[typeId][4];
	gg.Quad().Draw(f, f, c.ToGLPos(pos), { 0.5f, 0.1f }, scene->cRocksScale * c.scale);
}


/***************************************************************************************/

void Scene::Init() {
	cam.Init(gg.scale, 1.f, gg.designSize / 2);
	ui.Emplace()->InitRoot(gg.scale * cUIScale);

	// gen rock pos using image filter
	auto& img = gg.stbi.minecart_3;
	assert(img.comp == 4);
	auto s = 1.f;// / (gg.designSize / img.Size());

	float density{ 7 };
	XYi cGridSize{ 5 * density, 3 * density };
	cRocksScale = 0.6f / density;
	cRockMargin = img.Size() / cGridSize;
	cRockMarginOffsetRange = { cRockMargin / 3 };
	cRocksMaxCount = cGridSize.x * cGridSize.y;

	auto cellSize = (int32_t)std::ceilf(std::max(cRockMargin.x, cRockMargin.y));
	auto numCRs = img.Size().As<int32_t>() / cellSize + 1;
	cartrocks.Reserve(cRocksMaxCount);

	XY basePoss[]{ { cRockMargin.x * 0.5f, cRockMargin.y * 0.25f }, { cRockMargin.x * 0.5f, cRockMargin.y * 0.75f } };
	for (int y = 0; y < cGridSize.y; ++y) {
		for (int x = 0; x < cGridSize.x; ++x) {
			auto& basePos = basePoss[x & 1];
			XY pos{ basePos.x + cRockMargin.x * x, basePos.y + cRockMargin.y * y };
			auto ipos = (pos * s).As<int32_t>();
			// filte by img
			auto cidx = ipos.y * img.w + ipos.x;
			if (img.At(cidx).a) {
				rocksFixedPosPool.Emplace(pos + gg.designSize * 0.5f);
			}
		}
	}
	assert(rocksFixedPosPool.len <= cRocksMaxCount);
	AddCartRocks();
}

void Scene::AddCartRocks() {
	for (auto& o : rocksFixedPosPool) {
		XY posOffset{
			gg.rnd.Next<float>(-cRockMarginOffsetRange.x, cRockMarginOffsetRange.x),
			gg.rnd.Next<float>(-cRockMarginOffsetRange.y, cRockMarginOffsetRange.y)
		};
		auto p = o + posOffset;
		p.y += cCartBaseY;
		cartrocks.Emplace().Emplace()->Init(this, p);
	}
	cCartBaseY -= cRockMargin.y;
}

void Scene::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}
	if (gg.keyboard[GLFW_KEY_SPACE](0.2f)) {
		AddCartRocks();
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
	// todo: add new craftrock
}

void Scene::Draw() {
	// bg
	gg.Quad().Draw(gg.tf.bg1, gg.tf.bg1, 0, 0.5f, gg.designSize.y / gg.tf.bg1.Size().y * cam.scale, 0, 0.5f);

	gg.Quad().Draw(gg.tf.minecart_[0], gg.tf.minecart_[0], 0, { 0.f, 1.f }, cam.scale, 0, 0.5f);
	for (auto& o : cartrocks) o->Draw();
	gg.Quad().Draw(gg.tf.minecart_[0], gg.tf.minecart_[1], 0, { 0.f, 1.f }, cam.scale, 0, 0.5f);


	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale * cUIScale);
	cam.SetBaseScale(gg.scale);
}

}
