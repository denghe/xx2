#pragma once
#include "game.h"

namespace Test1 {

	struct Scene;

	template<typename T>
	struct alignas(4) B2Id {
		T id{};

		B2Id() = default;
		B2Id(B2Id const&) = delete;
		B2Id& operator=(B2Id const&) = delete;
		B2Id(B2Id&& o) noexcept { std::swap(id, o.id); }
		B2Id& operator=(B2Id&& o) noexcept { std::swap(id, o.id); return *this; }

		//B2Id(T id_) : id(id_) {}
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
		XX_INLINE void InitDef(b2WorldDef const& def_) {
			assert(IsNull());
			id = b2CreateWorld(&def_);
		}
		XX_INLINE void InitGravity(XY gravity_) {
			auto def = b2DefaultWorldDef();
			def.gravity = (b2Vec2&)gravity_;
			InitDef(def);
		}
		// ...
	};

	struct B2Body : B2Id<b2BodyId> {
		XX_INLINE void InitDef(B2World const& b2World_, b2BodyDef const& def_) {
			assert(IsNull());
			assert(!b2World_.IsNull());
			id = b2CreateBody(b2World_, &def_);
		}
		XX_INLINE void InitBox(B2World const& b2World_, b2BodyType type_, XY pos_, XY halfSize_) {
			auto def = b2DefaultBodyDef();
			def.type = type_;
			def.position = (b2Vec2&)pos_;
			InitDef(b2World_, def);
			auto shape = b2MakeBox(halfSize_.x, halfSize_.y);
			// todo
		}
		// ...
	};

	// ...

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		B2World b2world;
		B2Body b2body;
		
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
