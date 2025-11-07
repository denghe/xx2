#include "pch.h"
#include "scene_grass.h"

void Grass::FillColorplus() {
	colorPlus = gg.rnd.Next<float>(scene->cGrassColorPlus.from, scene->cGrassColorPlus.to);
}
void Grass::FillScale() {
	scale = gg.rnd.Next<float>(scene->cGrassScale.from, scene->cGrassScale.to);
}

void Grass::Init(Scene_Grass* scene_, SpineFrameBatch* sfb_, XY pos_, bool randomFrameIndex_) {
	scene = scene_;
	sfb = sfb_;
	pos = pos_;
	FillColorplus();
	FillScale();
	if (randomFrameIndex_) {
		frameIndex = gg.rnd.Next<int32_t>(sfb->numFrames);
		assert(frameIndex <= sfb->numFrames);
	}
	else {
		frameIndex = 0;
	}
}

void Grass::InitGridIndex() {
	scene->grid.Add(gridIndex, this);
}

void Grass::Update() {
	++frameIndex;
	if (frameIndex == sfb->numFrames) {
		frameIndex = 0;
	}
}

void Grass::Draw() {
	auto& f = sfb->tfs[frameIndex];
	gg.Quad().Draw(*f.tex, f.uvRect, scene->cam.ToGLPos(pos), sfb->anchor, scale * scene->cam.scale
		, 0, colorPlus/*, xx::RGBA8_Red*/);
}

Grass::Grass(Grass&& o) noexcept {
	operator=(std::move(o));
}

Grass& Grass::operator=(Grass&& o) noexcept {
	memcpy(this, &o, sizeof(*this));
	o.gridIndex = -1;
	return *this;
}


Grass::~Grass() {
	if (gridIndex >= 0) {
		scene->grid.Remove(gridIndex, this);
	}
}
