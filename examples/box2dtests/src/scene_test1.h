#pragma once
#include "game.h"

namespace Test1 {

	template<typename T>
	struct alignas(4) B2Id {
		T id{};

		B2Id() = default;
		B2Id(B2Id const&) = delete;
		B2Id& operator=(B2Id const&) = delete;
		B2Id(B2Id&& o) noexcept { std::swap(id, o.id); }
		B2Id& operator=(B2Id&& o) noexcept { std::swap(id, o.id); return *this; }

		B2Id(T id_) : id(id_) {} // unsafe: for easy use
		operator T const& () const { return id; }

		XX_INLINE bool IsNull() const {
			auto p = (int32_t*)&id;
			if constexpr (sizeof(id) == 4) if (p[0]) return false;
			if constexpr (sizeof(id) == 8) if (p[0] || p[1]) return false;
			return true;
		}

		// unsafe
		XX_INLINE void ZeroMem() {
			assert(!IsNull());
			auto p = (int32_t*)&id;
			if constexpr (sizeof(id) == 4) p[0] = 0;
			if constexpr (sizeof(id) == 8) p[0] = p[1] = 0;
		}

		// unsafe
		template<bool B = false>
		XX_INLINE void Destroy() {
			static_assert(sizeof(b2WorldId) <= 8 && sizeof(b2BodyId) <= 8 && sizeof(b2ShapeId) <= 8
				&& sizeof(b2JointId) <= 8 && sizeof(b2ChainId) <= 8);
			if constexpr (std::is_same_v<T, b2WorldId>) b2DestroyWorld(id);
			if constexpr (std::is_same_v<T, b2BodyId>) b2DestroyBody(id);
			if constexpr (std::is_same_v<T, b2ShapeId>) b2DestroyShape(id, B);
			if constexpr (std::is_same_v<T, b2JointId>) b2DestroyJoint(id, B);
			if constexpr (std::is_same_v<T, b2ChainId>) b2DestroyChain(id);
		}

		template<bool B = false>
		XX_INLINE void Reset() {
			if (!IsNull()) {
				Destroy<B>();
				ZeroMem();
			}
		}

		~B2Id() {
			Reset();
		}
	};

	struct B2World : B2Id<b2WorldId> {
		using B2Id<b2WorldId>::B2Id;

		XX_INLINE void InitDef(b2WorldDef const& b2worlddef_) {
			assert(IsNull());
			id = b2CreateWorld(&b2worlddef_);
		}

		XX_INLINE void InitGravity(XY gravity_, int workerCount_ = 4) {
			auto def = b2DefaultWorldDef();
			def.gravity = (b2Vec2&)gravity_;
			def.workerCount = workerCount_;
			InitDef(def);
		}
		// ...

		XX_INLINE void Step(float timeStep_ = gg.cDelta, int subStepCount_ = 4) {
			b2World_Step(id, timeStep_, subStepCount_);
		}
	};

	struct B2Body : B2Id<b2BodyId> {
		using B2Id<b2BodyId>::B2Id; 

		XX_INLINE void InitDef(B2World const& b2World_, b2BodyDef const& b2bodydef_) {
			assert(IsNull());
			assert(!b2World_.IsNull());
			id = b2CreateBody(b2World_, &b2bodydef_);
		}

		XX_INLINE B2Body& InitTypePos(B2World const& b2World_, XY pos_, b2BodyType type_ = b2_staticBody) {
			auto def = b2DefaultBodyDef();
			def.type = type_;
			def.position = (b2Vec2&)pos_;
			InitDef(b2World_, def);
			return *this;
		}
		// ...

		XX_INLINE b2Transform GetTransform() const {
			return b2Body_GetTransform(id);
		}

		XX_INLINE XY GetPos() const {
			return b2Body_GetTransform(id).p;
		}

		XX_INLINE std::pair<XY, float> GetPosRadians() const {
			auto tran = GetTransform();
			return { tran.p, b2Rot_GetAngle(tran.q) };
		}
	};

	struct B2Shape : B2Id<b2ShapeId> {
		using B2Id<b2ShapeId>::B2Id;

		XX_INLINE void InitDefPolygon(B2Body const& b2body_, b2Polygon const& b2polygon_
			, b2ShapeDef const& b2shapedef_ = b2DefaultShapeDef()) {
			assert(!b2body_.IsNull());
			Reset();
			id = b2CreatePolygonShape(b2body_, &b2shapedef_, &b2polygon_);
		}

		XX_INLINE void InitBox(B2Body const& b2body_, XY halfSize_
			, b2ShapeDef const& b2shapedef_ = b2DefaultShapeDef()) {
			auto b2polygon = b2MakeBox(halfSize_.x, halfSize_.y);
			InitDefPolygon(b2body_, b2polygon, b2shapedef_);
		}

		XX_INLINE void InitCircle(B2Body const& b2body_, XY center_, float radius_
			, b2ShapeDef const& b2shapedef_ = b2DefaultShapeDef()) {
			Reset();
			auto circle = b2Circle{ .center = (b2Vec2&)center_, .radius = radius_ };
			id = b2CreateCircleShape(b2body_, &b2shapedef_, &circle);
		}
		// ...
	};

	// ...

	struct Scene;

	struct SceneItem1 {
		Scene* scene{};
		B2Body b2body;
		B2Shape b2shape;
		XY halfSize{};
		float scale{};
		int32_t _1{}, i{};
		void Init(Scene* scene_, XY pos_, XY halfSize_);
		bool Update();
		void Draw();
	};

	struct SceneItem2 {
		Scene* scene{};
		B2Body b2body;
		B2Shape b2shape;
		float radius{};
		float scale{};
		int32_t _1{}, i{};
		void Init(Scene* scene_, XY pos_, float radius_);
		bool Update();
		void Draw();
	};

	struct SceneEdge {
		Scene* scene{};
		B2Body b2body;
		B2Shape b2shape;
		XY halfSize{};
		void Init(Scene* scene_, XY pos_, XY halfSize_);
		void Update();
		void Draw();
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		B2World b2world;
		SceneEdge edge;
		xx::List<xx::Shared<SceneItem1>> item1s;
		xx::List<xx::Shared<SceneItem2>> item2s;
		//static constexpr xx::FromTo<float> cCamScaleRange{ 0.3, 1 };
		//xx::Shared<xx::Slider> uiCamScale;

		//static constexpr xx::FromTo<int32_t> cGenAirplaneNumPerFrameRange{ 1, 1000 };
		//int32_t cGenAirplaneNumPerFrame{ 1 };
		//xx::Shared<xx::Slider> uiGenAirplaneNumPerFrame;

		float genTimer{};
		void Gen(int32_t num_);

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
