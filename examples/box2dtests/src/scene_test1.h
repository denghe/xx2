#pragma once
#include "game.h"

namespace Test1 {

	struct Scene;

	static_assert(sizeof(b2Vec2) == sizeof(XY));
	struct B2World {
		b2WorldId worldId{};

		XX_INLINE bool IsInited() {
			static_assert(sizeof(b2WorldId) == sizeof(int32_t));
			return (int32_t&)worldId != 0;
		}

		void InitByDef(b2WorldDef const& def_) {
			assert(!IsInited());
			worldId = b2CreateWorld(&def_);
		}

		void Init(XY gravity_) {
			auto def = b2DefaultWorldDef();
			def.gravity = (b2Vec2&)gravity_;
			InitByDef(def);
		}

		// todo: more Init

		~B2World() {
			if ((int32_t&)worldId) {
				b2DestroyWorld(worldId);
				worldId = {};
			}
		}
	};

	struct B2Body {
		B2World* owner{};
		b2BodyId bodyId{};

		void InitByDef(B2World* owner_, b2BodyDef def_) {
			assert(memcmp(&bodyId, 0, sizeof(bodyId)) == 0);
			assert(owner_->IsInited());
			owner = owner_;
			bodyId = b2CreateBody(owner_->worldId, &def_);
		}

		void InitShapeBox(B2World* owner_, b2BodyType type_, XY pos_, XY halfSize_) {
			auto def = b2DefaultBodyDef();
			def.type = type_;
			def.position = (b2Vec2&)pos_;
			InitByDef(owner_, def);
			auto shape = b2MakeBox(halfSize_.x, halfSize_.y);
		}

		~B2Body() {
			if (memcmp(&bodyId, 0, sizeof(bodyId)) != 0) {
				assert(owner->IsInited());
				b2DestroyBody(bodyId);
				bodyId = {};
			}
		}
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		B2World b2world;
		
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
