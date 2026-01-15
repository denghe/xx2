#pragma once
#include <xx_box2d.h>

struct _phys_rocks {
	using XY = xx::XY;
	typedef void (*InitFunc)(b2BodyId const&, float, b2ShapeDef*, const char*);
	static void MakePolygon(b2BodyId const& id_, b2ShapeDef& def_, float scale_, XY const* data_, size_t len_);
	static void MakeCircle(b2BodyId const& id_, b2ShapeDef& def_, float scale_, float x_, float y_, float r_);

	static constexpr float ptm_ratio{ 32 };		// need call b2SetLengthUnitsPerMeter(  ????  );

	struct rock1_1 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ 25, 5 }, { 3, -27 }, { -7, -24 }, { -15, 24 }, { -5, 27 }, { 8, 27 }, { 23, 14 }, { 25, 10 }};
			static constexpr XY polygons2[] {{ 29, -14 }, { 25, -19 }, { 3, -27 }, { 25, 5 }, { 29, -3 }};
			static constexpr XY polygons3[] {{ 25, -22 }, { 19, -27 }, { 3, -27 }, { 25, -19 }};
			static constexpr XY polygons4[] {{ -15, 24 }, { -7, -24 }, { -21, -17 }, { -28, -8 }, { -29, 11 }, { -27, 16 }};
		};
	};
	struct rock1_2 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ 21, -12 }, { 19, -12 }, { -24, -2 }, { 8, 30 }, { 12, 28 }, { 24, 16 }, { 27, 9 }, { 22, -9 }};
			static constexpr XY polygons2[] {{ 8, -27 }, { 2, -30 }, { -8, -30 }, { -21, -19 }, { -24, -2 }, { 19, -12 }, { 17, -18 }};
			static constexpr XY polygons3[] {{ 25, -7 }, { 22, -9 }, { 27, 9 }, { 27, -1 }};
			static constexpr XY polygons4[] {{ -25, 15 }, { 8, 30 }, { -24, -2 }, { -27, 3 }, { -27, 13 }};
			static constexpr XY polygons5[] {{ -25, 19 }, { -21, 26 }, { -16, 30 }, { 8, 30 }, { -25, 15 }};
		};
	};
	struct rock1_3 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5080,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -26.004, -6 }, { -30.004, -2 }, { -32.004, 2 }, { -32.004, 14 }, { -24.004, 24 }, { -16.004, 27 }, { 1.996, 27 }, { 6.996, 24 }};
			static constexpr XY polygons2[] {{ 14.996, -27 }, { 2.996, -27 }, { -19.004, -16 }, { 6.996, 24 }, { 9.996, 24 }, { 17.996, 17 }, { 28.996, -9 }, { 22.996, -21 }};
			static constexpr XY polygons3[] {{ -25.004, -10 }, { -26.004, -6 }, { 6.996, 24 }, { -19.004, -16 }};
			static constexpr XY polygons4[] {{ 30.996, -8 }, { 28.996, -9 }, { 17.996, 17 }, { 19.996, 17 }, { 28.996, 8 }, { 30.996, 4 }};
		};
	};
	struct rock1_4 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -27, -6 }, { -17, 26 }, { -2, 27 }, { 14, 27 }, { 32, -1 }, { -14, -17 }, { -21, -15 }, { -27, -9 }};
			static constexpr XY polygons2[] {{ -26, 21 }, { -17, 26 }, { -27, -6 }, { -32, 2 }, { -32, 15 }};
			static constexpr XY polygons3[] {{ -24, 24 }, { -17, 26 }, { -26, 21 }};
			static constexpr XY polygons4[] {{ 29, -21 }, { 24, -25 }, { 16, -27 }, { 6, -27 }, { -14, -17 }, { 32, -1 }, { 32, -15 }};
			static constexpr XY polygons5[] {{ -28, 21 }, { -26, 21 }, { -32, 15 }};
			static constexpr XY polygons6[] {{ 32, -1 }, { 14, 27 }, { 21, 24 }, { 28, 17 }};
		};
	};
	struct rock1_5 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -2, -25 }, { -4, -23 }, { -27, 12 }, { 11, 27 }, { 29, 2 }, { 28, -20 }, { 18, -24 }, { 1, -25 }};
			static constexpr XY polygons2[] {{ -27, 12 }, { -4, -23 }, { -24, -16 }, { -31, -8 }, { -32, 5 }, { -29, 11 }};
			static constexpr XY polygons3[] {{ 29, 2 }, { 32, -2 }, { 32, -15 }, { 28, -20 }};
			static constexpr XY polygons4[] {{ 1, -25 }, { 18, -24 }, { 15, -27 }, { 3, -27 }};
			static constexpr XY polygons5[] {{ 19, 24 }, { 24, 19 }, { 29, 2 }, { 11, 27 }};
			static constexpr XY polygons6[] {{ -27, 12 }, { -27, 15 }, { -23, 22 }, { -18, 26 }, { 11, 27 }};
		};
	};
	struct rock1_6 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.4920 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ 2, -32.004 }, { -8, -32.004 }, { -11, -31.004 }, { -17, -25.004 }, { -19, -21.004 }, { -21, -9.004 }, { 22, -7.004 }, { 8, -27.004 }};
			static constexpr XY polygons2[] {{ 22, -7.004 }, { -21, -9.004 }, { -16, 26.996 }, { -7, 30.996 }, { 9, 30.996 }, { 17, 27.996 }, { 26, 17.996 }, { 27, 0.996 }};
			static constexpr XY polygons3[] {{ 12, -26.004 }, { 8, -27.004 }, { 22, -7.004 }, { 22, -10.004 }, { 17, -21.004 }};
			static constexpr XY polygons4[] {{ -16, 26.996 }, { -21, -9.004 }, { -27, 4.996 }, { -27, 14.996 }, { -25, 18.996 }};
		};
	};
	struct bag_1 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5020,0.5020 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -116.97, 1.95 }, { -116.97, 32.95 }, { -31.97, 112.95 }, { 100.03, -37.05 }, { 89.03, -56.05 }, { -76.97, -76.05 }, { -100.97, -42.05 }, { -110.97, -19.05 }};
			static constexpr XY polygons2[] {{ 86.03, 89.95 }, { 100.03, 76.95 }, { 110.03, 59.95 }, { 116.03, 41.95 }, { 116.03, 5.95 }, { 110.03, -15.05 }, { 61.03, 105.95 }, { 73.03, 99.95 }};
			static constexpr XY polygons3[] {{ -104.97, 67.95 }, { -93.97, 81.95 }, { -31.97, 112.95 }, { -116.97, 32.95 }, { -114.97, 43.95 }};
			static constexpr XY polygons4[] {{ -62.97, -90.05 }, { -76.97, -76.05 }, { 89.03, -56.05 }, { 67.03, -82.05 }, { -14.97, -111.05 }, { -23.97, -109.05 }, { -35.97, -105.05 }};
			static constexpr XY polygons5[] {{ 14.03, -112.05 }, { -14.97, -111.05 }, { 67.03, -82.05 }, { 67.03, -84.05 }, { 47.03, -100.05 }, { 30.03, -108.05 }};
			static constexpr XY polygons6[] {{ -93.97, 83.95 }, { -82.97, 93.95 }, { -61.97, 105.95 }, { -45.97, 110.95 }, { -31.97, 112.95 }, { -93.97, 81.95 }};
			static constexpr XY polygons7[] {{ 86.03, 89.95 }, { 88.03, 89.95 }, { 100.03, 76.95 }};
			static constexpr XY polygons8[] {{ 38.03, 112.95 }, { 61.03, 105.95 }, { 110.03, -15.05 }, { 100.03, -37.05 }, { -31.97, 112.95 }};
		};
	};
};
