#pragma once
#include "xx_shader.h"

namespace xx {

    struct Shader_RadiateGoldenRaysData {
        XY pos{};
        float time{}, radius{};

        // Center color (almost white)
        RGBA8 centerColor{};

        // Ray color (gold / yellow)
        RGBA8 rayColor{};

        // Number of radial rays( 8.0 ~ 48.0 )
        float rayCount{};
        // Ray sharpness (higher value creates thinner, more defined rays)( 2.0 ~ 20.0 )
        float raySharpness{};
        // Radius fade (rays extend from center to this distance; 1 = full extent) ( 0.3 ~ 1.2 )
        float radiusFade{};
        // Center glow radius (range of the central bright area) ( 0.0 ~ 0.5 )
        float centerGlowRadius{};

        // Clockwise rotation speed (radians per second) ( -2.0 ~ 2.0 )
        float rotationSpeed{};
        // Flicker speed (higher value = faster flicker) ( 0.5 ~ 8.0 )
        float flickerSpeed{};
        // Flicker intensity (0 = no flicker, 1 = rays nearly turn fully on and off) ( 0.0 ~ 1.0)
        float flickerStrength{};
    };

    struct Shader_RadiateGoldenRays : Shader {
        using Shader::Shader;
        GLint aVert{ -1 }, aPosTimeRadius{ -1 }, aCenterColor{ -1 }, aRayColor{ -1 }
        , aRayCountRaySharpnessRadiusFadeCenterGlowRadius{ -1 }
        , aRotationSpeedFlickerSpeedFlickerStrength{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t numCircleMeshSegaments{ 16 };
        static constexpr int32_t maxNums{ 100 };
        std::unique_ptr<Shader_RadiateGoldenRaysData[]> data = std::make_unique_for_overwrite<Shader_RadiateGoldenRaysData[]>(maxNums);
        int32_t count{};

        void Init();

        virtual void Begin() override;

        virtual void End() override;

        void Commit();

        Shader_RadiateGoldenRaysData* Alloc(int32_t num_);

        void Draw(XY pos_ = {}, float time_ = 0, float radius_ = 100
            , RGBA8 centerColor_ = { 255, 255, 250, 255 }
            , RGBA8 rayColor_ = { 255, 224, 112, 255 }
            , float rayCount_ = 12.f
            , float raySharpness_ = 2.f
            , float radiusFade_ = 1.1f
            , float centerGlowRadius_ = 0.5f
            , float rotationSpeed_ = 0.1f
            , float flickerSpeed_ = 2.0f
            , float flickerStrength_ = 1.0f
        );
    };

}
