#include "pch.h"
#include "game_scene.h"
#include "game_minecart.h"

void MineCart::Init(Scene* scene_, XY pos_) {
	scene = scene_;
	pos = pos_;
	y = pos.y + gg.stbi.minecart_3.h;
	flyTargetPos = pos + XY{ gg.stbi.minecart_3.w * 0.55f, gg.stbi.minecart_3.h * 0.4f };
	cCartBaseYLimit = -gg.designSize.y * 0.7;

	// gen rock pos using image filter
	auto& img = gg.stbi.minecart_3;
	assert(img.comp == 4);
	auto s = 1.f;// / (gg.designSize / img.Size());

	float density{ 8 };
	XYi cGridSize{ 5 * density, 3 * density };
	cRocksScale = 0.6f / density;
	cRockMargin = img.Size() / cGridSize;
	cRockMarginOffsetRange = { cRockMargin.x / 6, cRockMargin.y / 10 };

	auto cellSize = (int32_t)std::ceilf(std::max(cRockMargin.x, cRockMargin.y));
	auto numCRs = img.Size().As<int32_t>() / cellSize + 1;

	XY minXY{ std::numeric_limits<float>::max() };
	XY maxXY{ std::numeric_limits<float>::min() };
	XY basePoss[]{ { cRockMargin.x * 0.5f, cRockMargin.y * 0.25f }, { cRockMargin.x * 0.5f, cRockMargin.y * 0.75f } };
	for (int y = 0; y < cGridSize.y; ++y) {
		for (int x = 0; x < cGridSize.x; ++x) {
			auto& basePos = basePoss[x & 1];
			XY pos{ basePos.x + cRockMargin.x * x, basePos.y + cRockMargin.y * y };
			auto ipos = (pos * s).As<int32_t>();
			// filte by img
			auto cidx = ipos.y * img.w + ipos.x;
			if (img.At(cidx).a) {
				fixedPosPool.Emplace(pos);
				if (pos.x > maxXY.x) maxXY.x = pos.x;
				if (pos.x < minXY.x) minXY.x = pos.x;
				if (pos.y > maxXY.y) maxXY.y = pos.y;
				if (pos.y < minXY.y) minXY.y = pos.y;
			}
		}
	}
	minXY -= cRockMargin;
	maxXY += cRockMargin;
	offset = pos + minXY;
	texSize = maxXY - minXY;
	for (auto& p : fixedPosPool) {
		p += pos;
	}
	fixedPoss.AddRange(fixedPosPool);
	rocks.Reserve(fixedPosPool.len);

	// todo
	//tex = xx::FrameBuffer().Init().Draw(texSize, true, xx::RGBA8_Zero, [] {});
}

void MineCart::Add(int32_t typeId_) {
	if (cCartBaseY < cCartBaseYLimit) return;
	if(fixedPosPool.Empty()) {
		fixedPosPool.AddRange(fixedPoss);
		cCartBaseY -= cRockMargin.y;
		// todo: combine to tex
	}
	XY posOffset{
		gg.rnd.Next<float>(-cRockMarginOffsetRange.x, cRockMarginOffsetRange.x),
		gg.rnd.Next<float>(-cRockMarginOffsetRange.y, cRockMarginOffsetRange.y)
	};
	auto idx = gg.rnd.Next(fixedPosPool.len);
	auto pos = fixedPosPool[idx];
	fixedPosPool.SwapRemoveAt(idx);
	pos.y += cCartBaseY;
	rocks.Emplace(typeId_, pos + posOffset);
}

void MineCart::Draw() {
	auto& f0 = gg.all.minecart_[0];
	auto& f1 = gg.all.minecart_[1];
	auto& c = scene->cam;
	auto p = c.ToGLPos(pos);
	// bg
	gg.Quad().Draw(f0, f0.uvRect, p, {0, 1}, c.scale, 0, 0.5f);

	if (tex) {
		//gg.Quad().Draw(tex, tex->Rect(), pos + offset, {0.f, 1.f}, c.scale);
		// todo
	}
	for (auto& o : rocks) {
		auto& f = gg.all_rocks_()[o.first][4];
		gg.Quad().DrawFrame(f, c.ToGLPos(o.second), cRocksScale * c.scale);
	}
	
	// cover
	gg.Quad().Draw(f1, f1, p, { 0, 1 }, c.scale, 0, 0.5f, { 255,255,255,180 });
}
