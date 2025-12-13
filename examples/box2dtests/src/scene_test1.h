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

		B2Id(T id_) : id(id_) {}
		operator T const& () const { return id; }

		XX_INLINE bool IsNull() const {
			auto p = (int32_t*)&id;
			if constexpr (sizeof(id) == 4) if (p[0]) return false;
			if constexpr (sizeof(id) == 8) if (p[0] || p[1]) return false;
			if constexpr (sizeof(id) == 12) if (p[0] || p[1] || p[2]) return false;
			// ...
			return true;
		}

		XX_INLINE void ZeroMem() {
			assert(!IsNull());
			auto p = (int32_t*)&id;
			if constexpr (sizeof(id) == 4) p[0] = 0;
			if constexpr (sizeof(id) == 8) p[0] = p[1] = 0;
			if constexpr (sizeof(id) == 12) p[0] = p[1] = p[2] = 0;
			// ...
		}

		~B2Id() {
			if (IsNull()) return;
			if constexpr (std::is_same_v<T, b2WorldId>) b2DestroyWorld(id);
			if constexpr (std::is_same_v<T, b2BodyId>) b2DestroyBody(id);
			// ...
			ZeroMem();
		}
	};

	struct B2World : B2Id<b2WorldId> {
		using B2Id<b2WorldId>::B2Id;
		XX_INLINE void InitDef(b2WorldDef const& b2worlddef_) {
			assert(IsNull());
			id = b2CreateWorld(&b2worlddef_);
		}
		XX_INLINE void InitGravity(XY gravity_) {
			auto def = b2DefaultWorldDef();
			def.gravity = (b2Vec2&)gravity_;
			InitDef(def);
		}
		// ...
		XX_INLINE void Step(float timeStep_ = gg.cDelta, int subStepCount_ = 4) {
			b2World_Step(id, timeStep_, subStepCount_);
		}
	};

	struct B2Shape;
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
		XX_INLINE b2ShapeId AddBox(XY halfSize_) {
			assert(!IsNull());
			auto b2polygon = b2MakeBox(halfSize_.x, halfSize_.y);
			auto shapeDef = b2DefaultShapeDef();
			return b2CreatePolygonShape(id, &shapeDef, &b2polygon);
		}
		// ... more AddPolygon
		XX_INLINE std::pair<XY, float> GetPosRadians() const {
			auto tran = b2Body_GetTransform(id);
			return { (XY&)tran.p, b2Rot_GetAngle(tran.q) };
		}
	};

	struct B2Shape : B2Id<b2ShapeId> {
		using B2Id<b2ShapeId>::B2Id;
		XX_INLINE void InitDef(B2Body const& b2body_, b2ShapeDef const& b2shapedef_, b2Polygon const& b2polygon_) {
			assert(IsNull());
			assert(!b2body_.IsNull());
			id = b2CreatePolygonShape(b2body_, &b2shapedef_, &b2polygon_);
		}
	};

	// ...

	struct Scene;

	struct SceneItem {
		Scene* scene{};
		B2Body b2body;
		XY size{};
		void Init(Scene* scene_, XY pos_, XY halfSize_);
		void Draw();
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		B2World b2world;
		xx::List<xx::Shared<SceneItem>> items;
		//static constexpr xx::FromTo<float> cCamScaleRange{ 0.3, 1 };
		//xx::Shared<xx::Slider> uiCamScale;

		//static constexpr xx::FromTo<int32_t> cGenAirplaneNumPerFrameRange{ 1, 1000 };
		//int32_t cGenAirplaneNumPerFrame{ 1 };
		//xx::Shared<xx::Slider> uiGenAirplaneNumPerFrame;

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
