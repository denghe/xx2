#pragma once
#include "pch.h"
#include <xx_box2d.h>

struct _phys {
	using XY = xx::XY;
	typedef void (*InitFunc)(b2BodyId const&, float, b2ShapeDef*, const char*);
	static void MakePolygon(b2BodyId const& id_, b2ShapeDef& def_, float scale_, XY const* data_, size_t len_);
	static void MakeCircle(b2BodyId const& id_, b2ShapeDef& def_, float scale_, float x_, float y_, float r_);

	static constexpr float ptm_ratio{ 32 };		// need call b2SetLengthUnitsPerMeter(  ????  );

	struct _10 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5000 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -128, 72 }, { -154, 107 }, { -98, 162 }, { -38, 114 }};
			static constexpr XY polygons2[] {{ 10, -189 }, { -136, -107 }, { -155, -54 }, { -128, 72 }, { -38, 114 }, { 15, 123 }, { 80, -129 }};
			static constexpr XY polygons3[] {{ 291, -19 }, { 196, -94 }, { 80, -129 }, { 15, 123 }, { 44, 189 }, { 288, 33 }};
			static constexpr XY polygons4[] {{ -254, 110 }, { -170, 65 }, { -155, -54 }, { -244, -116 }, { -281, -108 }, { -290, 95 }};
			static constexpr XY polygons5[] {{ -128, 72 }, { -155, -54 }, { -170, 65 }};
		};
	};
	struct _110 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5000 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ 22, -138 }, { -56, -92 }, { -102, -24 }, { -112, 46 }, { -25, 90 }, { 63, -83 }};
			static constexpr XY polygons2[] {{ -163, 73 }, { -112, 46 }, { -102, -24 }, { -152, -62 }, { -180, -52 }, { -183, 55 }};
			static constexpr XY polygons3[] {{ 63, -83 }, { -25, 90 }, { -1, 137 }, { 184, 40 }, { 183, 2 }, { 124, -62 }};
		};
	};
	struct _123 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5000 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -133, -49 }, { -137, 35 }, { -128, 62 }, { -78, 40 }, { -86, -38 }};
			static constexpr XY polygons2[] {{ 41, -131 }, { -47, -86 }, { -86, -38 }, { -78, 40 }, { -43, 91 }, { 55, -83 }};
			static constexpr XY polygons3[] {{ 40, 130 }, { 129, 47 }, { 135, 3 }, { 106, -51 }, { 55, -83 }, { -43, 91 }};
		};
	};
	struct _128 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5020 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ 25, 141.102 }, { 55, 150.102 }, { 270, 27.102 }, { 268, -31.898 }, { 46, 100.102 }};
			static constexpr XY polygons2[] {{ -158, 80.102 }, { -110, 111.102 }, { -19, 99.102 }, { -142, 56.102 }};
			static constexpr XY polygons3[] {{ -237, -75.898 }, { -269, -33.898 }, { -264, 45.102 }, { -235, 73.102 }, { -160, 45.102 }, { -157, -45.898 }};
			static constexpr XY polygons4[] {{ -152, -90.898 }, { -157, -45.898 }, { -142, 56.102 }, { -19, 99.102 }, { 46, 100.102 }, { 268, -31.898 }, { 88, -147.898 }, { -30, -138.898 }};
			static constexpr XY polygons5[] {{ -142, 56.102 }, { -157, -45.898 }, { -160, 45.102 }};
		};
	};
	struct _138 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5020,0.5030 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -122.046, -27.957 }, { -136.046, 15.043 }, { -115.046, 54.043 }, { -80.046, 48.043 }, { -86.046, -21.957 }};
			static constexpr XY polygons2[] {{ -61.046, -63.957 }, { -86.046, -21.957 }, { -80.046, 48.043 }, { -21.046, 73.043 }, { 28.954, 89.043 }, { 102.954, 51.043 }, { -13.046, -58.957 }};
			static constexpr XY polygons3[] {{ -56.046, 79.043 }, { -21.046, 73.043 }, { -80.046, 48.043 }};
			static constexpr XY polygons4[] {{ 134.954, 18.043 }, { 132.954, -18.957 }, { 22.954, -89.957 }, { -13.046, -58.957 }, { 102.954, 51.043 }};
		};
	};
	struct _14 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5000 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -302.131, -60 }, { -284.131, 24 }, { -159.131, 84 }, { -159.131, -20 }, { -247.131, -62 }};
			static constexpr XY polygons2[] {{ 35.869, 169 }, { 106.869, 127 }, { 117.869, -92 }, { 51.869, -164 }, { -0.131, -158 }, { -104.131, -29 }, { -7.131, 122 }};
			static constexpr XY polygons3[] {{ -104.131, -29 }, { -159.131, -20 }, { -159.131, 84 }, { -86.131, 141 }, { -7.131, 122 }};
			static constexpr XY polygons4[] {{ 313.869, 40 }, { 307.869, -1 }, { 117.869, -92 }, { 106.869, 127 }, { 253.869, 88 }};
			static constexpr XY polygons5[] {{ -291.131, 149 }, { -159.131, 84 }, { -284.131, 24 }, { -316.131, 111 }};
		};
	};
	struct _147 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5000 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -167.837, -43 }, { -163.837, 58 }, { -101.837, 46 }, { -110.837, -21 }};
			static constexpr XY polygons2[] {{ -110.837, -21 }, { -101.837, 46 }, { 2.163, 89 }, { 144.163, 63 }, { 168.163, 5 }, { 144.163, -38 }, { -5.837, -68 }};
			static constexpr XY polygons3[] {{ -10.837, -98 }, { -5.837, -68 }, { 144.163, -38 }, { 19.163, -124 }};
			static constexpr XY polygons4[] {{ 166.163, 41 }, { 168.163, 5 }, { 144.163, 63 }};
			static constexpr XY polygons5[] {{ -0.837, 123 }, { 144.163, 63 }, { 2.163, 89 }};
		};
	};
	struct _168 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5011,0.5020 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -216, -71.882 }, { -219, 44.118 }, { -141, 32.118 }, { -132, -43.882 }};
			static constexpr XY polygons2[] {{ -125, -111.882 }, { -113, -67.882 }, { 15, 127.118 }, { 214, 58.118 }, { 216, 16.118 }, { 81, -131.882 }, { -0, -153.882 }, { -67, -145.882 }};
			static constexpr XY polygons3[] {{ -131, 108.118 }, { 15, 127.118 }, { -113, -67.882 }, { -132, -43.882 }, { -141, 32.118 }};
			static constexpr XY polygons4[] {{ 15, 127.118 }, { 17, 155.118 }, { 214, 58.118 }};
		};
	};
	struct _192 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -193.909, -41 }, { -204.909, 10 }, { -184.909, 71 }, { -115.909, 38 }, { -139.909, -29 }};
			static constexpr XY polygons2[] {{ 125.091, -137 }, { 13.091, -141 }, { -82.909, -110 }, { -139.909, -29 }, { -115.909, 38 }, { 30.091, 89 }, { 113.091, 75 }, { 123.091, -39 }};
			static constexpr XY polygons3[] {{ 30.091, 89 }, { 38.091, 129 }, { 80.091, 145 }, { 113.091, 75 }};
			static constexpr XY polygons4[] {{ -121.909, 76 }, { -42.909, 104 }, { 30.091, 89 }, { -115.909, 38 }};
			static constexpr XY polygons5[] {{ 203.091, 43 }, { 201.091, 8 }, { 123.091, -39 }, { 113.091, 75 }};
			static constexpr XY polygons6[] {{ -139.909, -29 }, { -82.909, -110 }, { -143.909, -62 }};
		};
	};
	struct _206 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5020 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -151, 41.13 }, { -141, -40.87 }, { -291, -73.87 }, { -287, 74.13 }, { -260, 82.13 }};
			static constexpr XY polygons2[] {{ 52, 157.13 }, { 83, 146.13 }, { 113, 96.13 }, { 36, -156.87 }, { 30, 103.13 }};
			static constexpr XY polygons3[] {{ -96, 104.13 }, { -40, 87.13 }, { -131, -82.87 }, { -141, -40.87 }, { -151, 41.13 }};
			static constexpr XY polygons4[] {{ 113, 96.13 }, { 305, 41.13 }, { 293, -19.87 }, { 36, -156.87 }};
			static constexpr XY polygons5[] {{ -40, 87.13 }, { 30, 103.13 }, { 36, -156.87 }, { -131, -82.87 }};
		};
	};
	struct _243 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5009,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -274, -127 }, { -225, -14 }, { -131, 38 }, { -112, -48 }, { -218, -121 }};
			static constexpr XY polygons2[] {{ -52, -186 }, { -112, -48 }, { -131, 38 }, { 35, 145 }, { 276, 40 }, { 212, -91 }, { 51, -182 }};
			static constexpr XY polygons3[] {{ 51, 191 }, { 74, 185 }, { 276, 40 }, { 35, 145 }};
			static constexpr XY polygons4[] {{ -89, 167 }, { -44, 176 }, { 35, 145 }, { -131, 38 }};
			static constexpr XY polygons5[] {{ 276, 40 }, { 270, -17 }, { 212, -91 }};
			static constexpr XY polygons6[] {{ -225, -14 }, { -277, 105 }, { -231, 108 }, { -131, 38 }};
		};
	};
	struct _244 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5016 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -49.041, -145 }, { -41.041, -97 }, { 62.959, 115 }, { 62.959, -122 }, { -6.041, -151 }};
			static constexpr XY polygons2[] {{ -269.041, -78 }, { -234.041, -4 }, { -135.041, 34 }, { -130.041, -36 }};
			static constexpr XY polygons3[] {{ -45.041, 146 }, { 62.959, 115 }, { -39.041, 103 }};
			static constexpr XY polygons4[] {{ 268.959, -33 }, { 170.959, -103 }, { 62.959, -122 }, { 62.959, 115 }, { 99.959, 152 }, { 268.959, 22 }};
			static constexpr XY polygons5[] {{ -41.041, -97 }, { -130.041, -36 }, { -135.041, 34 }, { -39.041, 103 }, { 62.959, 115 }};
			static constexpr XY polygons6[] {{ -261.041, 83 }, { -135.041, 34 }, { -234.041, -4 }, { -260.041, 52 }};
		};
	};
	struct _283 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5018 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -110, -43 }, { -160, -63 }, { -158, 60 }, { -105, 35 }};
			static constexpr XY polygons2[] {{ -71, -132 }, { -110, -43 }, { -105, 35 }, { -77, 132 }, { -48, 140 }, { 89, 93 }, { 113, -86 }, { -26, -138 }};
			static constexpr XY polygons3[] {{ 89, 93 }, { 162, 28 }, { 162, -9 }, { 113, -86 }};
		};
	};
	struct _285 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5020 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ 4, -146.91 }, { -64, -114.91 }, { -105, -37.91 }, { -112, 56.09 }, { -49, 133.09 }, { -1, 150.09 }, { 179, 26.09 }, { 100, -84.91 }};
			static constexpr XY polygons2[] {{ -167, 98.09 }, { -112, 56.09 }, { -105, -37.91 }, { -161, -83.91 }, { -181, -71.91 }, { -179, 78.09 }};
			static constexpr XY polygons3[] {{ 179, 26.09 }, { 183, -13.91 }, { 100, -84.91 }};
		};
	};
	struct _288 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ 243.013, 29 }, { 237.013, -19 }, { 73.013, -125 }, { -98.987, -87 }, { 5.013, 89 }, { 34.013, 127 }, { 198.013, 65 }};
			static constexpr XY polygons2[] {{ -220.987, 88 }, { -149.987, 46 }, { -137.987, -33 }, { -229.987, 3 }, { -241.987, 62 }};
			static constexpr XY polygons3[] {{ -137.987, -33 }, { -149.987, 46 }, { -102.987, 88 }, { 5.013, 89 }, { -98.987, -87 }};
			static constexpr XY polygons4[] {{ -229.987, 3 }, { -137.987, -33 }, { -215.987, -85 }, { -242.987, -71 }};
		};
	};
	struct _296 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5010 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -163, -67.151 }, { -196, -52.151 }, { -191, 85.849 }, { -164, 101.849 }, { -109, 61.849 }, { -115, -29.151 }};
			static constexpr XY polygons2[] {{ 96, 173.849 }, { 195, 49.849 }, { 196, 1.849 }, { 101, -121.151 }, { -115, -29.151 }, { 48, 145.849 }};
			static constexpr XY polygons3[] {{ 11, -174.151 }, { -54, -155.151 }, { -115, -29.151 }, { 101, -121.151 }};
			static constexpr XY polygons4[] {{ -109, 61.849 }, { -89, 105.849 }, { -36, 138.849 }, { 48, 145.849 }, { -115, -29.151 }};
		};
	};
	struct _303 {
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
			static constexpr XY polygons1[] {{ -115, -66 }, { -140, -23 }, { -139, 35 }, { -107, 80 }, { 20, 79 }, { -29, -64 }};
			static constexpr XY polygons2[] {{ 20, 79 }, { 60, 116 }, { 233, 39 }, { 235, 2 }, { 55, -117 }, { -29, -64 }};
			static constexpr XY polygons3[] {{ -218, -56 }, { -233, -34 }, { -230, 56 }, { -208, 65 }, { -139, 35 }, { -140, -23 }};
		};
	};
	struct _309 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5016 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -185, -64 }, { -164, -0 }, { -77, 63 }, { -72, -53 }, { -137, -87 }};
			static constexpr XY polygons2[] {{ -25, 152 }, { 4, 160 }, { 185, 39 }, { 179, -21 }, { -25, 117 }};
			static constexpr XY polygons3[] {{ -138, 103 }, { -77, 63 }, { -164, -0 }, { -181, 74 }};
			static constexpr XY polygons4[] {{ -77, 63 }, { -25, 117 }, { 179, -21 }, { 31, -154 }, { -70, -98 }};
			static constexpr XY polygons5[] {{ 31, -154 }, { -3, -157 }, { -70, -98 }};
		};
	};
	struct _402 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5015,0.5017 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -158, 53 }, { -113, 48 }, { 1, -141 }, { -128, -62 }, { -170, -0 }};
			static constexpr XY polygons2[] {{ 28, 110 }, { 34, 144 }, { 168, 40 }, { 128, -75 }, { 1, -141 }};
			static constexpr XY polygons3[] {{ -96, 114 }, { 28, 110 }, { 1, -141 }, { -113, 48 }};
			static constexpr XY polygons4[] {{ -104, -128 }, { -128, -62 }, { 1, -141 }};
			static constexpr XY polygons5[] {{ -161, -49 }, { -170, -0 }, { -128, -62 }};
		};
	};
	struct _407 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5015,0.5020 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ 42, -124.998 }, { -101, -26.998 }, { -103, 42.002 }, { -17, 126.002 }, { 162, 58.002 }, { 167, 16.002 }, { 133, -39.998 }};
			static constexpr XY polygons2[] {{ -137, -70.998 }, { -160, -54.998 }, { -170, 59.002 }, { -148, 76.002 }, { -103, 42.002 }, { -101, -26.998 }};
		};
	};
	struct _408 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5010 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ 17.069, 143.849 }, { 64.069, 173.849 }, { 215.069, 26.849 }, { 209.069, -27.151 }, { 31.069, 116.849 }};
			static constexpr XY polygons2[] {{ -188.931, 81.849 }, { -98.931, 48.849 }, { -110.931, -38.151 }, { -189.931, -59.151 }, { -208.931, -41.151 }, { -209.931, 43.849 }};
			static constexpr XY polygons3[] {{ -113.931, -97.151 }, { -110.931, -38.151 }, { 15.069, 108.849 }, { 31.069, 116.849 }, { 209.069, -27.151 }, { 60.069, -166.151 }, { 13.069, -170.151 }};
			static constexpr XY polygons4[] {{ -106.931, 99.849 }, { -91.931, 124.849 }, { 15.069, 108.849 }, { -98.931, 48.849 }};
			static constexpr XY polygons5[] {{ -98.931, 48.849 }, { 15.069, 108.849 }, { -110.931, -38.151 }};
		};
	};
	struct _415 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5020 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -137, -52.002 }, { -150, -30.002 }, { -155, 41.998 }, { -144, 60.998 }, { -101, 48.998 }, { -103, -34.002 }};
			static constexpr XY polygons2[] {{ -82, 89.998 }, { -28, 99.998 }, { 92, 84.998 }, { 150, 1.998 }, { 18, -124.002 }, { -56, -96.002 }, { -103, -34.002 }, { -101, 48.998 }};
			static constexpr XY polygons3[] {{ 155, 38.998 }, { 150, 1.998 }, { 92, 84.998 }};
			static constexpr XY polygons4[] {{ -15, 124.998 }, { 92, 84.998 }, { -28, 99.998 }};
		};
	};
	struct _421 {
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
			static constexpr XY polygons1[] {{ -175, 72 }, { -99, 52 }, { -102, -37 }, { -163, -68 }, { -189, -54 }, { -191, 36 }};
			static constexpr XY polygons2[] {{ -99, 52 }, { -112, 77 }, { -71, 118 }, { 6, 100 }};
			static constexpr XY polygons3[] {{ 6, 100 }, { 191, 32 }, { 189, -3 }, { 18, -142 }, { -97, -97 }, { -102, -37 }, { -99, 52 }};
			static constexpr XY polygons4[] {{ 25, 140 }, { 191, 32 }, { 6, 100 }};
		};
	};
	struct _422 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5014,0.5020 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -22, 128.042 }, { 14, 136.042 }, { 173, 54.042 }, { 172, 0.042 }, { -17, 94.042 }};
			static constexpr XY polygons2[] {{ -63, -40.958 }, { -17, 94.042 }, { 172, 0.042 }, { 15, -134.958 }, { -60, -79.958 }};
			static constexpr XY polygons3[] {{ -174, -62.958 }, { -175, 57.042 }, { -142, 74.042 }, { -75, 44.042 }, { -63, -40.958 }, { -132, -69.958 }};
			static constexpr XY polygons4[] {{ -75, 44.042 }, { -17, 94.042 }, { -63, -40.958 }};
		};
	};
	struct _443 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5009,0.5020 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -174, -41.958 }, { -154, -19.958 }, { -5, 94.042 }, { 271, 50.042 }, { 187, -68.958 }, { 66, -134.958 }, { -143, -75.958 }};
			static constexpr XY polygons2[] {{ -154, -19.958 }, { -169, 42.042 }, { -144, 94.042 }, { -5, 94.042 }};
			static constexpr XY polygons3[] {{ -274, -57.958 }, { -274, 65.042 }, { -242, 75.042 }, { -169, 42.042 }, { -238, -62.958 }};
			static constexpr XY polygons4[] {{ -169, 42.042 }, { -154, -19.958 }, { -238, -62.958 }};
			static constexpr XY polygons5[] {{ 267, 3.042 }, { 187, -68.958 }, { 271, 50.042 }};
			static constexpr XY polygons6[] {{ -5, 94.042 }, { 12, 136.042 }, { 271, 50.042 }};
		};
	};
	struct _449 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5012,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -198, -53 }, { -216, -9 }, { -216, 11 }, { -193, 55 }, { -137, 42 }, { -152, -46 }};
			static constexpr XY polygons2[] {{ -108, 86 }, { -8, 81 }, { 49, -115 }, { -92, -90 }, { -152, -46 }, { -137, 42 }};
			static constexpr XY polygons3[] {{ -8, 81 }, { 36, 117 }, { 215, 20 }, { 197, -44 }, { 49, -115 }};
		};
	};
	struct _458 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5010 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -211, 125.857 }, { -149, 116.857 }, { -61, -151.143 }, { -243, -33.143 }, { -250, 63.857 }};
			static constexpr XY polygons2[] {{ 61, 178.857 }, { 84, 172.857 }, { 252, 18.857 }, { 246, -45.143 }, { 162, -135.143 }, { 89, -172.143 }, { 29, 133.857 }};
			static constexpr XY polygons3[] {{ -79, 154.857 }, { 29, 133.857 }, { 89, -172.143 }, { 7, -173.143 }, { -61, -151.143 }, { -149, 116.857 }};
		};
	};
	struct _461 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5014,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -17, 147 }, { 11, 157 }, { 65, 102 }, { -3, 96 }};
			static constexpr XY polygons2[] {{ -114, 148 }, { -77, 149 }, { -34, 62 }, { 35, -143 }, { -71, -27 }, { -134, 111 }};
			static constexpr XY polygons3[] {{ 166, 58 }, { 170, -0 }, { 35, -143 }, { -34, 62 }, { -3, 96 }, { 65, 102 }};
			static constexpr XY polygons4[] {{ -8, -155 }, { -69, -86 }, { -71, -27 }, { 35, -143 }};
			static constexpr XY polygons5[] {{ -133, -53 }, { -170, -39 }, { -134, 111 }, { -71, -27 }};
		};
	};
	struct _462 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5020 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -228, -39.91 }, { -247, 11.09 }, { -244, 59.09 }, { -226, 85.09 }, { -154, 60.09 }, { -148, -19.91 }};
			static constexpr XY polygons2[] {{ 4, 112.09 }, { 44, 148.09 }, { 246, 55.09 }, { 239, -4.91 }, { 218, -34.91 }, { 79, -143.91 }, { -40, -107.91 }};
			static constexpr XY polygons3[] {{ -124, 111.09 }, { 4, 112.09 }, { -40, -107.91 }, { -144, -56.91 }, { -154, 60.09 }};
		};
	};
	struct _463 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5009,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -245, 87 }, { -146, 47 }, { -144, -18 }, { -259, -48 }, { -270, 3 }};
			static constexpr XY polygons2[] {{ 264, -5 }, { 237, -5 }, { 130, 86 }, { 243, 33 }};
			static constexpr XY polygons3[] {{ 77, 138 }, { 130, 86 }, { 114, -97 }, { 60, -138 }, { 46, 106 }};
			static constexpr XY polygons4[] {{ 269, -38 }, { 259, -55 }, { 114, -97 }, { 130, 86 }, { 237, -5 }};
			static constexpr XY polygons5[] {{ -106, 112 }, { 46, 106 }, { 60, -138 }, { -144, -18 }, { -146, 47 }};
			static constexpr XY polygons6[] {{ -144, -54 }, { -144, -18 }, { 60, -138 }};
		};
	};
	struct _464 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -169.873, -50 }, { -186.873, -5 }, { -186.873, 30 }, { -115.873, 37 }, { -110.873, -35 }};
			static constexpr XY polygons2[] {{ -167.873, 67 }, { -115.873, 37 }, { -186.873, 30 }};
			static constexpr XY polygons3[] {{ 185.127, 5 }, { 163.127, -36 }, { 50.127, -118 }, { 20.127, 88 }, { 30.127, 118 }, { 151.127, 53 }};
			static constexpr XY polygons4[] {{ -42.873, -77 }, { -110.873, -35 }, { -115.873, 37 }, { -85.873, 81 }, { 20.127, 88 }, { 50.127, -118 }};
			static constexpr XY polygons5[] {{ -110.873, -51 }, { -110.873, -35 }, { -42.873, -77 }, { -88.873, -72 }};
		};
	};
	struct _467 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5020 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -205, 91.13 }, { -134, 39.13 }, { -116, -32.87 }, { -188, -81.87 }, { -214, -70.87 }};
			static constexpr XY polygons2[] {{ -116, 84.13 }, { -92, 113.13 }, { -30, 92.13 }, { -106, 56.13 }};
			static constexpr XY polygons3[] {{ 69, 148.13 }, { 99, 153.13 }, { 125, 79.13 }, { 141, -94.87 }, { -29, -156.87 }, { 50, 97.13 }};
			static constexpr XY polygons4[] {{ 212, 15.13 }, { 204, -28.87 }, { 141, -94.87 }, { 125, 79.13 }};
			static constexpr XY polygons5[] {{ 50, 97.13 }, { -29, -156.87 }, { -57, -145.87 }, { -116, -32.87 }, { -134, 39.13 }, { -106, 56.13 }, { -30, 92.13 }};
		};
	};
	struct _470 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5000 };
		struct fixture1 {
			static constexpr float density{ 2 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -225.995, 67 }, { -168.995, 47 }, { -113.995, -16 }, { -215.995, -51 }, { -241.995, -18 }, { -243.995, 33 }};
			static constexpr XY polygons2[] {{ -55.995, 97 }, { 5.005, 76 }, { -5.995, -70 }, { -53.995, -83 }, { -113.995, -16 }, { -110.995, 38 }};
			static constexpr XY polygons3[] {{ -168.995, 47 }, { -110.995, 38 }, { -113.995, -16 }};
			static constexpr XY polygons4[] {{ 81.005, 89 }, { 234.005, 42 }, { 243.005, -16 }, { 88.005, -122 }, { -5.995, -70 }, { 5.005, 76 }};
			static constexpr XY polygons5[] {{ 101.005, 121 }, { 234.005, 42 }, { 81.005, 89 }};
			static constexpr XY polygons6[] {{ -113.995, -16 }, { -53.995, -83 }, { -119.995, -41 }};
		};
	};
	struct _472 {
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
			static constexpr XY polygons1[] {{ 99, 98 }, { 128, 63 }, { 146, -53 }, { 100, -98 }, { 26, -56 }, { 72, 66 }};
			static constexpr XY polygons2[] {{ -253, 59 }, { -171, 27 }, { -148, -28 }, { -237, -2 }};
			static constexpr XY polygons3[] {{ -257, -68 }, { -237, -2 }, { -148, -28 }};
			static constexpr XY polygons4[] {{ -171, 27 }, { -117, 39 }, { -62, -74 }, { -148, -28 }};
			static constexpr XY polygons5[] {{ -62, -74 }, { -117, 39 }, { -86, 75 }, { 28, 66 }, { 26, -56 }};
			static constexpr XY polygons6[] {{ 258, 23 }, { 254, -16 }, { 146, -53 }, { 128, 63 }, { 230, 40 }};
			static constexpr XY polygons7[] {{ 72, 66 }, { 26, -56 }, { 28, 66 }};
		};
	};
	struct _51 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5000 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -181, -76 }, { -206, -38 }, { -198, 33 }, { -177, 54 }, { -128, 39 }, { -125, -53 }};
			static constexpr XY polygons2[] {{ 44, 150 }, { 84, 146 }, { 115, 83 }, { 111, -124 }, { 35, -153 }, { 20, 112 }};
			static constexpr XY polygons3[] {{ -128, 96 }, { -48, 130 }, { 20, 112 }, { 35, -153 }, { -77, -138 }, { -125, -53 }};
			static constexpr XY polygons4[] {{ -123, -99 }, { -125, -53 }, { -77, -138 }};
			static constexpr XY polygons5[] {{ 202, -26 }, { 111, -124 }, { 115, 83 }, { 208, 1 }};
		};
	};
	struct _75 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5000 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -163.051, 59 }, { -150.051, -43 }, { -265.051, -84 }, { -267.051, 96 }};
			static constexpr XY polygons2[] {{ -104.051, 156 }, { 4.949, 134 }, { 273.949, 65 }, { 273.949, 9 }, { 136.949, -130 }, { -93.051, -94 }, { -150.051, -43 }, { -163.051, 59 }};
			static constexpr XY polygons3[] {{ -52.051, -180 }, { -93.051, -94 }, { 136.949, -130 }};
			static constexpr XY polygons4[] {{ 51.949, 183 }, { 273.949, 65 }, { 4.949, 134 }};
		};
	};
	struct _98 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5020 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -69, 42.962 }, { -62, -28.038 }, { -161, -69.038 }, { -158, 78.962 }};
			static constexpr XY polygons2[] {{ -42, 92.962 }, { 12, 74.962 }, { 120, -57.038 }, { 71, -68.038 }, { -62, -28.038 }, { -69, 42.962 }};
			static constexpr XY polygons3[] {{ 29, -115.038 }, { -64, -65.038 }, { -62, -28.038 }, { 71, -68.038 }};
			static constexpr XY polygons4[] {{ 167, -10.038 }, { 120, -57.038 }, { 12, 74.962 }, { 20, 96.962 }, { 43, 116.962 }, { 165, 34.962 }};
		};
	};
	struct _26 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5011,0.5000 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ 13, 168 }, { 70, 155 }, { 233, -1 }, { 163, -86 }, { 18, 114 }};
			static constexpr XY polygons2[] {{ 18, 114 }, { 163, -86 }, { 55, -172 }, { -109, -85 }, { -121, -34 }, { -30, 102 }};
			static constexpr XY polygons3[] {{ -212, 83 }, { -136, 49 }, { -121, -34 }, { -186, -82 }, { -220, -80 }, { -234, 45 }};
			static constexpr XY polygons4[] {{ -72, 122 }, { -30, 102 }, { -121, -34 }, { -136, 49 }, { -123, 89 }};
		};
	};
	struct _18 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5010,0.5000 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -257.143, 119 }, { -181.143, 91 }, { -184.143, -83 }, { -238.143, -114 }, { -288.143, -95 }, { -322.143, -4 }, { -304.143, 81 }};
			static constexpr XY polygons2[] {{ -1.143, 127 }, { 317.857, 58 }, { 314.857, -33 }, { 104.857, -208 }, { -123.143, -152 }, { -184.143, -83 }};
			static constexpr XY polygons3[] {{ -148.143, 152 }, { -78.143, 169 }, { -1.143, 127 }, { -184.143, -83 }, { -181.143, 91 }};
			static constexpr XY polygons4[] {{ 2.857, 184 }, { 36.857, 206 }, { 317.857, 58 }, { -1.143, 127 }};
		};
	};
	struct _17 {
		static void Init(b2BodyId const& id_, float scale_ = 1.f, b2ShapeDef* def_ = nullptr, char const* fixturesMask_ = nullptr);
		static constexpr XY anchorpoint{ 0.5000,0.5000 };
		struct fixture1 {
			static constexpr float density{ 1 };
			static constexpr float friction{ 0 };
			static constexpr float restitution{ 0.2 };
			static constexpr uint32_t filter_categoryBits{ 1 };
			static constexpr uint32_t filter_groupIndex{ 0 };
			static constexpr uint32_t filter_maskBits{ 65535 };
			static constexpr bool isSensor{ false };
			static constexpr XY polygons1[] {{ -248, 105 }, { -182, 85 }, { -166, -52 }, { -251, -99 }, { -285, -53 }, { -280, 62 }};
			static constexpr XY polygons2[] {{ 40, 197 }, { 134, 127 }, { 283, -3 }, { 66, -197 }, { 25, 145 }};
			static constexpr XY polygons3[] {{ -103, 164 }, { 25, 145 }, { 66, -197 }, { -83, -160 }, { -166, -52 }, { -182, 85 }};
			static constexpr XY polygons4[] {{ 283, 79 }, { 283, -3 }, { 134, 127 }};
			static constexpr XY polygons5[] {{ -164, -103 }, { -166, -52 }, { -83, -160 }};
		};
	};
};
