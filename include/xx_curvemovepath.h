#pragma once
#include "xx_ptr.h"

namespace xx {

    struct CurvePoint {
        XY pos{};
        float tension{ 0.2f };
        int32_t numSegments{ 100 };

        CurvePoint operator+(CurvePoint const& v) const;
        CurvePoint operator-(CurvePoint const& v) const;
        CurvePoint operator*(float v) const;
    };

    struct CurvePoints {
        std::string name;
        bool isLoop{};
        std::vector<CurvePoint> points;
    };

    struct CurvesPointsCollection {
        std::vector<CurvePoints> data;
    };

    struct MovePathPoint {
        XY pos{}, inc{};
        float radians{}, distance{};

        MovePathPoint& operator=(CurvePoint const& v);
    };

    struct MovePath {
        std::vector<MovePathPoint> points;
        float totalDistance{};
        bool loop{};

        void Clear();
        void Fill(XY const* ps, size_t len, bool loop);
        void Fill(bool loop);
        void FillFields(MovePathPoint& p1, MovePathPoint& p2);

        // pathway curve ( p p p ... )  to 2 control points bezier( p c c p c c p ... )
        template<typename List1, typename List2>
        void CurveToBezier(List1& bs, List2 const& cs) {
            auto n = cs.size();
            auto len = n * 3 - 2;
            bs.resize(len);

            bs[0] = cs[0];
            bs[1] = (cs[1] - cs[0]) * cs[0].tension + cs[0];

            for (size_t i = 0; i < n - 2; i++) {
                auto diff = (cs[i + 2] - cs[i]) * cs[i].tension;
                bs[3 * i + 2] = cs[i + 1] - diff;
                bs[3 * i + 3] = cs[i + 1];
                bs[3 * i + 4] = cs[i + 1] + diff;
            }
            bs[len - 2] = (cs[n - 2] - cs[n - 1]) * cs[n - 2].tension + cs[n - 1];
            bs[len - 1] = cs[n - 1];
        }

        // 2 control points bezier( p c c p c c p ... ) to split points
        template<typename List1, typename List2>
        void BezierToPoints(List1& ps, List2 const& bs) {
            auto len = (bs.size() - 1) / 3;
            size_t totalSegments = 0;
            for (size_t j = 0; j < len; ++j) {
                totalSegments += bs[j * 3].numSegments;
            }
            ps.resize(totalSegments);
            totalSegments = 0;
            for (size_t j = 0; j < len; ++j) {
                auto idx = j * 3;
                auto numSegments = bs[idx].numSegments;
                auto step = 1.0f / numSegments;
                for (int i = 0; i < numSegments; ++i) {
                    auto t = step * i;
                    auto t1 = 1 - t;
                    ps[totalSegments + i] = bs[idx] * t1 * t1 * t1
                        + bs[idx + 1] * 3 * t * t1 * t1
                        + bs[idx + 2] * 3 * t * t * (1 - t)
                        + bs[idx + 3] * t * t * t;
                }
                totalSegments += numSegments;
            }
        }

        void FillCurve(bool loop, std::vector<CurvePoint> const& ps);
    };

    struct MovePathSteper {
        Shared<MovePath> mp;
        size_t cursor{};	// mp[ index ]
        float cursorDistance{};	// forward

        void Init(Shared<MovePath> mp);

        struct MoveResult {
            XY pos{};
            float radians{}, movedDistance{};
            bool terminated;
        };

        MoveResult MoveToBegin();
        MoveResult MoveForward(float stepDistance);
    };

    struct MovePathCachePoint {
        XY pos{};
        float radians{};
    };

    struct MovePathCache {
        std::vector<MovePathCachePoint> points;
        bool loop{};
        float stepDistance{};

        void Init(MovePath const& mp, float stepDistance = 1.f);
        MovePathCachePoint* Move(float totalDistance);
    };

}
