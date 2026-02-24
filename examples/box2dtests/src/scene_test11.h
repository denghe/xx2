#pragma once
#include "game.h"

// simulate isaac game scene

namespace Test11 {
	static constexpr float cUIScale{ 0.5f };

	static constexpr float cCellPixelSize{ 128 };
	static constexpr float cCellPixelHalfSize{ cCellPixelSize / 2.f };
	static constexpr float cWallRadius{ cCellPixelSize / 2.f };
	static constexpr float cBucketRadius{ cWallRadius * 0.7f };
	static constexpr float cPlayerRadius{ cWallRadius * 0.7f };

	static constexpr XY cRoomMaxContentSize{ 26, 14 };	// logic
	static constexpr XY cRoomMaxTotalSize{ cRoomMaxContentSize + 2 };
	static constexpr XY cRoomMaxPixelSize{ cRoomMaxTotalSize * cCellPixelSize };

	static constexpr XY cRoom1x1ContentSize{ 13, 7 };
	static constexpr XY cRoom1x1TotalSize{ cRoom1x1ContentSize + 2 };
	static constexpr XY cRoom1x1PixelSize{ cRoom1x1TotalSize * cCellPixelSize };

	// ...

	struct Scene;

	struct SceneItem {
		static constexpr int32_t cTypeId{};
		Scene* scene{};
		SceneItem* next{};
		int32_t typeId{};	// fill by Init: typeId = cTypeId
		XY pos{};
		float y{};
		float scale{}, radians{}, radius{};
		bool flipX{}, isCenter{};
		int32_t indexAtContainer{ -1 };
		int32_t indexAtGrid{ -1 };
		virtual bool Update() { return false; }
		virtual void Draw() {};
		virtual void Dispose() {};	// unsafe: container.swapRemove( this )
		virtual ~SceneItem() {};
	};

	// todo: Player, Wall, Door, Rock, Bucket, Bomb....

	struct Wall : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		void Init(Scene* scene_, XY pos_);
		void Draw() override;
		void Dispose() override;	// unsafe
		~Wall() override;
	};

	struct Door : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		void Init(Scene* scene_, XY pos_, bool isCenter_);
		void Draw() override;
		void Dispose() override;	// unsafe
		~Door() override;
	};

	struct Player : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		static constexpr XY cSpeedMax{ 1500.f };
		static constexpr XY cAccel{ cSpeedMax * 0.1f };
		XY lastMoveDir{}, speed{};
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
		void Draw() override;
		void Dispose() override;	// unsafe
		~Player() override;
	};

	struct Bucket : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
		void Draw() override;
		void Dispose() override;	// unsafe
		~Bucket() override;
	};


	struct GridCache {
		XY pos{};
		float radius{};
		void operator=(SceneItem* p);
	};
	struct PhysSystem;

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		XY mapSize{};
		xx::Grid2dCircle<SceneItem*, GridCache> gridBuildings;	// for walls, doors
		//xx::Grid2dCircle<SceneItem*, GridCache> gridItems;	// for buckets, players, ...
		xx::Shared<PhysSystem> phys;
		xx::List<xx::Shared<Wall>> walls;
		xx::List<xx::Shared<Door>> doors;
		xx::List<xx::Shared<Bucket>> buckets;
		xx::List<xx::Shared<Player>> players;

		void GenWallHorizontal(int32_t xFrom_, int32_t xTo_, int32_t y_, bool leftOverflow_ = false, bool rightOverflow_ = false);
		void GenWallVertical(int32_t x_, int32_t yFrom_, int32_t yTo_, bool topOverflow_ = false, bool bottomOverflow_ = false);
		void GenDoorHorizontal(int32_t x_, int32_t y_);
		void GenDoorVertical(int32_t x_, int32_t y_);
		void GenPlayer(int32_t x_, int32_t y_);
		void GenBucket(int32_t x_, int32_t y_);

		xx::List<SceneItem*> sortContainer;			// for draw order by Y
		void SortContainerAdd(SceneItem* o_);
		void SortContainerDraw();

		void Init();
		void Update() override;
		virtual void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

	/***************************************************************************************************/
	/***************************************************************************************************/

	struct PhysSystem {
		static constexpr float cVelocityDamping{ 500.f };
		//static constexpr float cGravity{ 100.f };
		static constexpr float cResponseCoef{ 0.5f };
		static constexpr float cMaxSpeed{ 0.05f };

		struct Node {
			int32_t next;
			int32_t indexAtDatas;
			SceneItem* ud;
		};

		struct Data {
			int32_t indexAtNodes;
			float radius{};
			XY pos, lpos, acc;
		};

		struct Bucket {
			int32_t len;
			std::array<int32_t, 3> indexAtDatass;	// 3 maybe need larger
		};


		Scene* scene{};

		int32_t numRows{}, numCols{}, bucketsLen{};					// for buckets
		int32_t freeHead{ -1 }, freeCount{}, count{}, capacity{};	// for nodes
		int32_t datasLen{};											// for datas

		float _1_cellSize{};
		XY pixelSize{};

		std::unique_ptr<Node[]> nodes;
		std::unique_ptr<Data[]> datas;
		std::unique_ptr<Bucket[]> buckets;

		Data& At(int32_t indexAtNodes_) {
			assert(indexAtNodes_ >= 0 && indexAtNodes_ < count);
			return datas[nodes[indexAtNodes_].indexAtDatas];
		}

		void Init(Scene* scene_, int32_t capacity_ = 0) {
			assert(!nodes && !buckets && capacity_ >= 0);
			scene = scene_;
			numRows = scene->gridBuildings.numRows;
			numCols = scene->gridBuildings.numCols;
			_1_cellSize = scene->gridBuildings._1_cellSize;
			pixelSize = scene->gridBuildings.pixelSize;
			bucketsLen = numRows * numCols;
			capacity = capacity_;
			freeHead = -1;
			freeCount = count = 0;
			if (capacity_) {
				nodes = std::make_unique_for_overwrite<Node[]>(capacity_);
				datas = std::make_unique_for_overwrite<Data[]>(capacity_);
			}
			buckets = std::make_unique_for_overwrite<Bucket[]>(bucketsLen);
			ClearBuckets();
		}

		void Reserve(int32_t capacity_) {
			assert(buckets && capacity_ > 0);
			if (capacity_ <= capacity) return;
			capacity = capacity_;

			auto newNodes = std::make_unique_for_overwrite<Node[]>(capacity_);
			::memcpy((void*)newNodes.get(), (void*)nodes.get(), count * sizeof(Node));
			nodes = std::move(newNodes);

			auto newDatas = std::make_unique_for_overwrite<Data[]>(capacity_);
			::memcpy((void*)newDatas.get(), (void*)datas.get(), datasLen * sizeof(Data));
			datas = std::move(newDatas);
		}

		// return indexAtNodes
		int32_t Add(SceneItem* ud_) {
			assert(buckets);
			auto pos_ = ud_->pos;
			assert(pos_.y >= 0.f && pos_.y < pixelSize.y);
			assert(pos_.x >= 0.f && pos_.x < pixelSize.x);
			auto radius_ = ud_->radius;
			assert(radius_ > 0.f);

			int32_t ni;
			if (freeCount > 0) {
				ni = freeHead;
				freeHead = nodes[ni].next;
				freeCount--;
			}
			else {
				if (count == capacity) {
					Reserve(count ? count * 2 : 16);
				}
				ni = count;
				count++;
			}

			auto& n = nodes[ni];
			n.next = -1;
			n.indexAtDatas = datasLen;
			n.ud = ud_;

			auto& d = datas[datasLen++];
			d.indexAtNodes = ni;
			d.radius = radius_;
			d.pos = pos_;
			d.lpos = pos_;
			d.acc = 0;

			return ni;
		}

		void Remove(int32_t indexAtNodes_) {
			assert(buckets);
			assert(indexAtNodes_ >= 0 && indexAtNodes_ < count);
			assert(nodes[indexAtNodes_].ud);

			auto& n = nodes[indexAtNodes_];
			auto& ld = datas[datasLen - 1];

			nodes[ld.indexAtNodes].indexAtDatas = n.indexAtDatas;	// redirect
			::memcpy((void*)&datas[n.indexAtDatas], (void*)&ld, sizeof(Data));
			datasLen--;

			n.next = freeHead;
			n.indexAtDatas = -1;
			n.ud = {};
			freeHead = indexAtNodes_;
			freeCount++;
		}

		XX_INLINE void ClearBuckets() {
			assert(buckets);
			for (int32_t i = 0; i < bucketsLen; ++i) {
				buckets[i].len = 0;
			}
		}

		XX_INLINE void FillBuckets() {
			
			for (int32_t di = 0; di < datasLen; ++di) {
				auto p = (datas[di].pos * _1_cellSize).As<int32_t>();
				assert(p.x >= 0 && p.x < numCols && p.y >= 0 && p.y < numRows);
				auto& b = buckets[p.x * numRows + p.y];
				if (b.len < b.indexAtDatass.size()) {	// ignore
					b.indexAtDatass[b.len++] = di;
				}
			}
		}

		XX_INLINE void CalcDatas() {
			for (int32_t bi = 0; bi < bucketsLen; ++bi) {
				auto& b = buckets[bi];
				if (!b.len) continue;
				Calc(b, buckets[bi - 1]);
				Calc(b, buckets[bi]);
				Calc(b, buckets[bi + 1]);
				Calc(b, buckets[bi + numRows - 1]);
				Calc(b, buckets[bi + numRows]);
				Calc(b, buckets[bi + numRows + 1]);
				Calc(b, buckets[bi - numRows - 1]);
				Calc(b, buckets[bi - numRows]);
				Calc(b, buckets[bi - numRows + 1]);
			}
		}

		XX_INLINE void Calc(Bucket& b1_, Bucket& b2_) {
			for (int32_t di1 = 0; di1 < b1_.len; ++di1) {
				for (int32_t di2 = 0; di2 < b2_.len; ++di2) {
					Calc(datas[b1_.indexAtDatass[di1]], datas[b2_.indexAtDatass[di2]]);
				}
			}
		}

		XX_INLINE void Calc(Data& d1_, Data& d2_) {
			static constexpr float eps{ 0.0001f };
			auto d = d1_.pos - d2_.pos;
			auto mag2 = d.x * d.x + d.y * d.y;
			auto r = d1_.radius + d2_.radius;
			auto rr = r * r;
			if (mag2 >= rr || mag2 <= eps) return;
			auto mag = std::sqrtf(mag2);
			auto a = cResponseCoef * (r - mag);
			auto spd = d / mag * a;
			if (spd.x > cMaxSpeed) spd.x = cMaxSpeed;
			else if (spd.x < -cMaxSpeed) spd.x = -cMaxSpeed;
			if (spd.y > cMaxSpeed) spd.y = cMaxSpeed;
			else if (spd.y < -cMaxSpeed) spd.y = -cMaxSpeed;
			d1_.pos += spd;
			d2_.pos -= spd;
		}

		XX_INLINE void UpdateDatas() {
			for (int32_t i = 0; i < datasLen; ++i) {
				auto& o = datas[i];
				//o.acc += XY{ 0, cGravity };

				auto spd = o.pos - o.lpos;
				o.lpos = o.pos;
				o.pos = o.pos + spd + (o.acc - spd * cVelocityDamping) * (gg.cDelta * gg.cDelta);
				o.acc = {};

				// wall collision response
				using G = decltype(scene->gridBuildings);
				auto& g = scene->gridBuildings;
				auto cri = g.PosToCRIndex(o.pos);
				g.ForeachBy9(cri.y, cri.x, [&](G::Node& node, float range)->void {
					auto d = o.pos - node.cache.pos;
					auto mag2 = d.x * d.x + d.y * d.y;
					auto r = node.cache.radius + o.radius;
					auto rr = r * r;
					if (mag2 < rr) {	// cross?
						if (mag2 > 0) {
							auto mag = std::sqrtf(mag2);
							auto norm = d / mag;
							o.pos = node.cache.pos + norm * r;	// force move away
						}
					}
				});

				// write back
				auto& udPos = nodes[o.indexAtNodes].ud->pos;
				if (udPos != o.pos) {
					udPos = o.pos;
				}
			}
		}

		void Update() {
			ClearBuckets();
			FillBuckets();
			CalcDatas();
			UpdateDatas();
		}

	};

}
