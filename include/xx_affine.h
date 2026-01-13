#pragma once
#include "xx_string.h"
#include "xx_data.h"
#include "xx_prims.h"

namespace xx {

    struct AffineTransform {
        float a, b, c, d;
        float tx, ty;

        // anchorSize = anchor * size
        void PosScaleRadiansAnchorSize(XY const& pos, XY const& scale, float radians, XY const& anchorSize);
        void PosScaleRadians(XY const& pos, XY const& scale, float radians);
        // anchorSize = anchor * size
        void PosScaleAnchorSize(XY const& pos, XY const& scale, XY const& anchorSize);
        void PosScale(XY const& pos, XY const& scale);
        void Pos(XY const& pos);
        void Identity();
        // default apply
        XY operator()(XY const& point) const;
        XY NoRadiansApply(XY const& point) const;
        // child concat parent
        AffineTransform MakeConcat(AffineTransform const& t2);
        AffineTransform MakeInvert();
        static AffineTransform MakePosScaleRadiansAnchorSize(XY const& pos, XY const& scale, float radians, XY const& anchorSize);
        static AffineTransform MakePosScaleAnchorSize(XY const& pos, XY const& scale, XY const& anchorSize);
        static AffineTransform MakePosScaleRadians(XY const& pos, XY const& scale, float radians);
        static AffineTransform MakePosScale(XY const& pos, XY const& scale);
        static AffineTransform MakePos(XY const& pos);
        static AffineTransform MakeIdentity();
    };

    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_same_v<AffineTransform, std::remove_cvref_t<T>>>> {
        static inline void Append(std::string& s, T const& in) {
            ::xx::Append(s, in.a, ", ", in.b, ", ", in.c, ", ", in.d, ", ", in.tx, ", ", in.ty);
        }
    };

    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_same_v<AffineTransform, std::remove_cvref_t<T>>>> {
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteFixedArray<needReserve>((float*)&in, 6);
        }
        static inline int Read(Data_r& d, T& out) {
            return d.ReadFixedArray((float*)&out, 6);
        }
    };

    /*******************************************************************************************************************************************/
    /*******************************************************************************************************************************************/
	
	// AffineTransform without rotation support
    struct SimpleAffineTransform {
        float a{ 1 }, d{ 1 };
        float tx{}, ty{};
        
        void PosScaleAnchorSize(XY const& pos, XY const& scale, XY const& anchorSize);	// anchorSize = anchor * size
        void Identity();
        XY const& Offset() const;
        XY const& Scale() const;
        XY operator()(XY const& point) const;	// apply
        SimpleAffineTransform MakeConcat(SimpleAffineTransform const& t2) const;	// child concat parent
        SimpleAffineTransform MakeInvert() const;
        static SimpleAffineTransform MakeIdentity();
        static SimpleAffineTransform MakePosScaleAnchorSize(XY const& pos, XY const& scale, XY const& anchorSize);
    };

    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_same_v<SimpleAffineTransform, std::remove_cvref_t<T>>>> {
        static inline void Append(std::string& s, T const& in) {
            ::xx::Append(s, in.a, ", ", in.d, ", ", in.tx, ", ", in.ty);
        }
    };

    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_same_v<SimpleAffineTransform, std::remove_cvref_t<T>>>> {
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteFixedArray<needReserve>((float*)&in, 4);
        }
        static inline int Read(Data_r& d, T& out) {
            return d.ReadFixedArray((float*)&out, 4);
        }
    };
}
