#include "pch.h"
#include "scene_test11.h"
#include "scene_mainmenu.h"

namespace Test11 {

	void Wall::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cWallRadius;
		scale = radius * 2.f / gg.pics.cell_wall.uvRect.w;
		radians = {};
		indexAtContainer = scene->walls.len - 1;
		assert(scene->walls[indexAtContainer].pointer == this);
		scene->gridBuildings.Add(indexAtGrid, this);
	}

	void Wall::Draw() {
		gg.Quad().DrawFrame(gg.pics.cell_wall, scene->cam.ToGLPos(pos)
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

	/***************************************************************************************************/
	/***************************************************************************************************/

	void Door::Init(Scene* scene_, XY pos_, bool isCenter_) {
		typeId = cTypeId;
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cWallRadius;
		scale = radius * 2.f / gg.pics.cell_wall.uvRect.w;
		radians = {};
		isCenter = isCenter_;
		indexAtContainer = scene->doors.len - 1;
		assert(scene->doors[indexAtContainer].pointer == this);
		scene->gridBuildings.Add(indexAtGrid, this);
	}

	void Door::Draw() {
		gg.Quad().DrawFrame(gg.pics.cell_door, scene->cam.ToGLPos(pos)
			, scale * scene->cam.scale, radians);
	}

	void Door::Dispose() {
		auto i = indexAtContainer;
		assert(scene->doors[i].pointer == this);
		scene->doors.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->doors.SwapRemoveAt(i);
	}

	Door::~Door() {
		if (indexAtGrid > -1) {
			scene->gridBuildings.Remove(indexAtGrid, this);
			indexAtGrid = -1;
		}
	}

	/***************************************************************************************************/
	/***************************************************************************************************/

	void Player::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cPlayerRadius;
		scale = radius * 2.f / gg.pics.cell_player.uvRect.w;
		radians = {};
		indexAtContainer = scene->players.len - 1;
		assert(scene->players[indexAtContainer].pointer == this);
		scene->gridItems.Add(indexAtGrid, this);
	}

	bool Player::Update() {

		// input check
		XY moveDir{};
		if ((gg.keyboard[GLFW_KEY_A])
			&& gg.keyboard[GLFW_KEY_D]) {
			if (lastMoveDir.x == -1) {
				moveDir.x = 1;
			}
			else if (lastMoveDir.x == 1) {
				moveDir.x = -1;
			}
			else {
				moveDir.x = 0;
			}
		}
		else if (gg.keyboard[GLFW_KEY_A]) {
			lastMoveDir.x = moveDir.x = -1;
		}
		else if (gg.keyboard[GLFW_KEY_D]) {
			lastMoveDir.x = moveDir.x = 1;
		}
		else {
			lastMoveDir.x = moveDir.x = 0;
		}

		if ((gg.keyboard[GLFW_KEY_W])
			&& gg.keyboard[GLFW_KEY_S]) {
			if (lastMoveDir.y == -1) {
				moveDir.y = 1;
			}
			else if (lastMoveDir.y == 1) {
				moveDir.y = -1;
			}
			else {
				moveDir.y = 0;
			}
		}
		else if (gg.keyboard[GLFW_KEY_W]) {
			lastMoveDir.y = moveDir.y = -1;
		}
		else if (gg.keyboard[GLFW_KEY_S]) {
			lastMoveDir.y = moveDir.y = 1;
		}
		else {
			lastMoveDir.y = moveDir.y = 0;
		}

		// speed calc
		if (moveDir.x == 1) {		// right
			if (speed.x < 0) speed.x = 0;
			speed.x += cAccel.x;
			if (speed.x > cSpeedMax.x) {
				speed.x = cSpeedMax.x;
			}
		}
		else if (moveDir.x == -1) {	// left
			if (speed.x > 0) speed.x = 0;
			speed.x -= cAccel.x;
			if (speed.x < -cSpeedMax.x) {
				speed.x = -cSpeedMax.x;
			}
		}
		else {
			speed.x = 0;
		}

		if (moveDir.y == 1) {		// down
			if (speed.y < 0) speed.y = 0;
			speed.y += cAccel.y;
			if (speed.y > cSpeedMax.y) {
				speed.y = cSpeedMax.y;
			}
		}
		else if (moveDir.y == -1) {	// up
			if (speed.y > 0) speed.y = 0;
			speed.y -= cAccel.y;
			if (speed.y < -cSpeedMax.y) {
				speed.y = -cSpeedMax.y;
			}
		}
		else {
			speed.y = 0;
		}

		// new pos calc
		auto newPos = pos;
		if (speed.x != 0 || speed.y != 0) {
			if (speed.x != 0 && speed.y != 0) {
				newPos += speed * 0.7071f;
			}
			else {
				newPos += speed;
			}

			// todo: edge check, collision check

			using G = decltype(scene->gridBuildings);
			auto& g = scene->gridBuildings;
			auto cri = g.PosToCRIndex(newPos);
			g.ForeachBy9(cri.y, cri.x, [&](G::Node& node, float range)->void {
				auto d = newPos - node.cache.pos;
				auto mag2 = d.x * d.x + d.y * d.y;
				auto r = node.cache.radius + radius;
				auto rr = r * r;
				if (mag2 < rr) {	// cross?
					if (mag2 > 0) {
						auto mag = std::sqrtf(mag2);
						auto norm = d / mag;
						newPos = node.cache.pos + norm * r;	// force move away
					}
				}
				});

		}

		// move
		if (pos != newPos) {
			pos = newPos;
			y = pos.y;
			scene->gridItems.Update(indexAtGrid, this);
		}

		return false;
	}

	void Player::Draw() {
		gg.Quad().DrawFrame(gg.pics.cell_player, scene->cam.ToGLPos(pos)
			, scale * scene->cam.scale, radians);
	}

	void Player::Dispose() {
		auto i = indexAtContainer;
		assert(scene->players[i].pointer == this);
		scene->players.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->players.SwapRemoveAt(i);
	}

	Player::~Player() {
		if (indexAtGrid > -1) {
			scene->gridItems.Remove(indexAtGrid, this);
			indexAtGrid = -1;
		}
	}

	/***************************************************************************************************/
	/***************************************************************************************************/

	void Bucket::Init(Scene* scene_, XY pos_) {
		typeId = cTypeId;
		scene = scene_;
		pos = pos_;
		y = pos.y;
		radius = cBucketRadius;
		scale = radius * 2.f / gg.pics.cell_bucket.uvRect.w;
		radians = {};
		indexAtContainer = scene->buckets.len - 1;
		assert(scene->buckets[indexAtContainer].pointer == this);
		scene->gridItems.Add(indexAtGrid, this);
	}

	bool Bucket::Update() {
		// todo: handle inputs
		return false;
	}

	void Bucket::Draw() {
		gg.Quad().DrawFrame(gg.pics.cell_bucket, scene->cam.ToGLPos(pos)
			, scale * scene->cam.scale, radians);
	}

	void Bucket::Dispose() {
		auto i = indexAtContainer;
		assert(scene->buckets[i].pointer == this);
		scene->buckets.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		scene->buckets.SwapRemoveAt(i);
	}

	Bucket::~Bucket() {
		if (indexAtGrid > -1) {
			scene->gridItems.Remove(indexAtGrid, this);
			indexAtGrid = -1;
		}
	}

	/***************************************************************************************************/
	/***************************************************************************************************/

	void GridCache::operator=(SceneItem* p) {
		pos = p->pos;
		radius = p->radius;
	}

	void Scene::GenWallHorizontal(int32_t xFrom_, int32_t xTo_, int32_t y_, bool leftOverflow_, bool rightOverflow_) {
		for (int32_t x = xFrom_; x <= xTo_; ++x) {
			walls.Emplace().Emplace()->Init(this, XY{ x, y_ } * cCellPixelSize + cCellPixelHalfSize);
		}
		for (int32_t x = xFrom_; x < xTo_; ++x) {
			walls.Emplace().Emplace()->Init(this, XY{ x, y_ } * cCellPixelSize + XY{ cCellPixelSize, cCellPixelHalfSize });
		}
		if (leftOverflow_) {
			walls.Emplace().Emplace()->Init(this, XY{ xFrom_, y_ } * cCellPixelSize + XY{ 0, cCellPixelHalfSize });
		}
		if (rightOverflow_) {
			walls.Emplace().Emplace()->Init(this, XY{ xTo_, y_ } * cCellPixelSize + XY{ cCellPixelSize, cCellPixelHalfSize });
		}
	}

	void Scene::GenWallVertical(int32_t x_, int32_t yFrom_, int32_t yTo_, bool topOverflow_, bool bottomOverflow_) {
		for (int32_t y = yFrom_; y <= yTo_; ++y) {
			walls.Emplace().Emplace()->Init(this, XY{ x_, y } * cCellPixelSize + cCellPixelHalfSize);
		}
		for (int32_t y = yFrom_; y < yTo_; ++y) {
			walls.Emplace().Emplace()->Init(this, XY{ x_, y } * cCellPixelSize + XY{ cCellPixelHalfSize, cCellPixelSize });
		}
		if (topOverflow_) {
			walls.Emplace().Emplace()->Init(this, XY{ x_, yFrom_ } * cCellPixelSize + XY{ cCellPixelHalfSize, 0 });
		}
		if (bottomOverflow_) {
			walls.Emplace().Emplace()->Init(this, XY{ x_, yTo_ } * cCellPixelSize + XY{ cCellPixelHalfSize, cCellPixelSize });
		}
	}

	void Scene::GenDoorHorizontal(int32_t x_, int32_t y_) {
		auto p = XY{ x_, y_ } * cCellPixelSize + cCellPixelHalfSize;
		doors.Emplace().Emplace()->Init(this, p + XY{ cCellPixelHalfSize, 0 }, false);
		doors.Emplace().Emplace()->Init(this, p + XY{ -cCellPixelHalfSize, 0 }, false);
		doors.Emplace().Emplace()->Init(this, p, true);
	}

	void Scene::GenDoorVertical(int32_t x_, int32_t y_) {
		auto p = XY{ x_, y_ } * cCellPixelSize + cCellPixelHalfSize;
		doors.Emplace().Emplace()->Init(this, p + XY{ 0, cCellPixelHalfSize }, false);
		doors.Emplace().Emplace()->Init(this, p + XY{ 0, -cCellPixelHalfSize }, false);
		doors.Emplace().Emplace()->Init(this, p, true);
	}

	void Scene::GenPlayer(int32_t x_, int32_t y_) {
		auto p = XY{ x_, y_ } * cCellPixelSize + cCellPixelHalfSize;
		players.Emplace().Emplace()->Init(this, p);
	}

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale * cUIScale);

		mapSize = cRoom1x1PixelSize;
		cam.Init(gg.scale, gg.designSize.y / mapSize.y, mapSize / 2);
		sortContainer.Resize<true>((int32_t)mapSize.y);
		gridBuildings.Init(cCellPixelSize, std::ceilf(mapSize.y / cCellPixelSize), std::ceilf(mapSize.x / cCellPixelSize));
		gridItems.Init(cCellPixelSize, std::ceilf(mapSize.y / cCellPixelSize), std::ceilf(mapSize.x / cCellPixelSize));

		// [][][][][][][]  [][][][][][][]
		// []                          []
		// []                          []
		// []                          []
		//                               
		// []                          []
		// []                          []
		// []                          []
		// [][][][][][][]  [][][][][][][]

		GenWallHorizontal(0, 6, 0);
		GenDoorHorizontal(7, 0);
		GenWallHorizontal(8, 14, 0);

		GenWallHorizontal(0, 6, 8);
		GenDoorHorizontal(7, 8);
		GenWallHorizontal(8, 14, 8);

		GenWallVertical(0, 1, 3, true, false);
		GenDoorVertical(0, 4);
		GenWallVertical(0, 5, 7, false, true);
		GenWallVertical(14, 1, 3, true, false);
		GenDoorVertical(14, 4);
		GenWallVertical(14, 5, 7, false, true);

		GenPlayer(7, 4);
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
		for (auto i = players.len - 1; i >= 0; --i) {
			if (players[i]->Update()) {
				players[i]->Dispose();
			}
		}
	}

	void Scene::Draw() {
		// bg color
		gg.Quad().DrawTinyFrame(gg.embed.shape_dot, 0, 0.5f, gg.windowSize, 0, 1, { 0x81,0xbd,0x57,255 });

		// todo: floor
		// walls & doors
		for (auto& o : walls) o->Draw();
		for (auto& o : doors) o->Draw();

		// sort order by y
		for (auto& o : players) SortContainerAdd(o.pointer);
		SortContainerDraw();

		//gg.uiText->SetText(xx::ToString("num items = ", droppingItems.len));
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
