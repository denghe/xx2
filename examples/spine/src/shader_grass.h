#pragma once
#include "xx_gamebase.h"

namespace xx {

    struct Shader_GrassData {
        XY pos{}, anchor{ 0.5, 0.5 };                           // float * 4
        XY scale{ 1, 1 }; float radians{}, colorplus{ 1 };      // float * 4
        RGBA8 color{ 255, 255, 255, 255 };                      // u8n * 4
        UVRect texRect{};                                       // u16 * 4 ( left-top: 0,0 )

        XX_INLINE void Fill(UVRect rect_, XY pos_ = {}, XY anchor_ = 0.5f, XY scale_ = 1.f
            , float radians_ = 0.f, float colorplus_ = 1.f, RGBA8 color_ = RGBA8_White) {
            pos = pos_;
            anchor = anchor_;
            scale = scale_;
            radians = radians_;
            colorplus = colorplus_;
            color = color_;
            texRect = rect_;
        }
    };

    struct Shader_Grass : Shader {
        using Shader::Shader;
        GLint aVert{ -1 }, aPosAnchor{ -1 }, aScaleRadiansColorplus{ -1 }, aColor{ -1 }, aTexRect{ -1 };
        GLVertexArrays va;
        GLBuffer vb, ib;

        static constexpr int32_t maxNums{ 100 };
        std::unique_ptr<Shader_GrassData[]> data = std::make_unique_for_overwrite<Shader_GrassData[]>(maxNums);
        int32_t count{};

        void Init() {

            v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	// screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosAnchor;
in vec4 aScaleRadiansColorplus;
in vec4 aColor;
in vec4 aTexRect;

out vec2 vTexCoord;
flat out float vColorplus;
flat out vec4 vColor;

void main() {
    vec2 pos = aPosAnchor.xy;
    vec2 anchor = aPosAnchor.zw;
    vec2 scale = vec2(aScaleRadiansColorplus.x * aTexRect.z, aScaleRadiansColorplus.y * aTexRect.w);
    float radians = aScaleRadiansColorplus.z;
    vec2 offset = vec2((aVert.x - anchor.x) * scale.x, (aVert.y - anchor.y) * scale.y);

    float c = cos(radians);
    float s = sin(radians);
    vec2 v = pos + vec2(
       dot(offset, vec2(c, s)),
       dot(offset, vec2(-s, c))
    );

    gl_Position = vec4(v * uCxy, 0, 1);
    vColor = aColor;
    vColorplus = aScaleRadiansColorplus.w;
    vTexCoord = vec2(aTexRect.x + aVert.x * aTexRect.z, aTexRect.y + aTexRect.w - aVert.y * aTexRect.w);
})"sv });

            
            f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision lowp float;

in vec2 vTexCoord;
flat in float vColorplus;
flat in vec4 vColor;

out vec4 oColor;


// https://www.shadertoy.com/view/WsSGWd
#define BLADES_SPACING 0.004
#define JITTER_MAX 0.004
// depends on size of grass blades in pixels
#define LOOKUP_DIST 5

#define HASHSCALE1 .1031
#define HASHSCALE3 vec3(.1031, .1030, .0973)

#define PI 3.14
float hash12(vec2 p)
{
	vec3 p3  = fract(vec3(p.xyx) * HASHSCALE1);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

///  3 out, 2 in...
vec3 hash32(vec2 p)
{
	vec3 p3 = fract(vec3(p.xyx) * HASHSCALE3);
    p3 += dot(p3, p3.yxz+19.19);
    return fract((p3.xxy+p3.yzz)*p3.zyx);
}

/// 2 out, 2 in...
vec2 hash22(vec2 p)
{
	vec3 p3 = fract(vec3(p.xyx) * HASHSCALE3);
    p3 += dot(p3, p3.yzx+19.19);
    return fract((p3.xx+p3.yz)*p3.zy);

}

vec3 getGrassColor(float x) {
    vec3 a = vec3(0.2, 0.4, 0.3);
    vec3 b = vec3(0.3, 0.5, 0.2);
    vec3 c = vec3(0.2, 0.4, 0.2);
    vec3 d = vec3(0.66, 0.77, 0.33);
    vec3 col = a + b * cos(2. * PI * (c * x + d));
    return col;
}

float getGrassBlade(in vec2 position, in vec2 grassPos, out vec4 color) {
	// between {-1, -1, -1} and {1, 1, 1}
    vec3 grassVector3 = hash32(grassPos * 123512.41) * 2.0 - vec3(1);
    // keep grass z between 0 and 0.4
    grassVector3.z = grassVector3.z * 0.2 + 0.2;
    vec2 grassVector2 = normalize(grassVector3.xy);

    float grassLength = hash12(grassPos * 102348.7) * 0.01 + 0.012;

    // take coordinates in grass blade frame
    vec2 gv = position - grassPos;
    float gx = dot(grassVector2, gv);
    float gy = dot(vec2(-grassVector2.y, grassVector2.x), gv);
    float gxn = gx / grassLength;

    // TODO make gy depends to gx
    if (gxn >= 0.0 && gxn <= 1.0 && abs(gy) <= 0.0008 * (1. - gxn * gxn)) {
        vec3 thisGrassColor = getGrassColor(hash12(grassPos * 2631.6));
        color = vec4(thisGrassColor * (0.2 + 0.8 * gxn), 1.0);
    	return grassVector3.z * gxn;
    }
    else {
        color = vec4(0., 0., 0., 1.);
        return -1.0;
    }
}

float getPoint(in vec2 position, out vec4 color) {
   	int xcount = int(1. / BLADES_SPACING);
    int ycount = int(1. / BLADES_SPACING);
    int ox = int(position.x * float(xcount));
    int oy = int(position.y * float(ycount));

    float maxz = 0.0;

    for (int i = -LOOKUP_DIST; i < LOOKUP_DIST; ++i) {
        for (int j = -LOOKUP_DIST; j < LOOKUP_DIST; ++j) {
            vec2 upos = vec2(ox + i, oy + j);
            vec2 grassPos = (upos * BLADES_SPACING + hash22(upos) * JITTER_MAX);

            vec4 tempColor;
            float z = getGrassBlade(position, grassPos, tempColor);

            if (z > maxz) {
                maxz = z;
                color = tempColor;
            }
        }
    }
    if (maxz == 0.0) {
        color = vec4(0.);
    }

    return maxz;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/vec2(1200., 1200.);
    
    vec4 color;
    float z = getPoint(uv, color);
    
    // Output to screen
    fragColor = color;
}


void main() {
    // oColor = vec4( (c.x + 0.00001f) * vColorplus, (c.y + 0.00001f) * vColorplus, (c.z + 0.00001f) * vColorplus, c.w );
    vec4 c;
    mainImage( c, vTexCoord );
    oColor = c;
})"sv });

            p = LinkGLProgram(v, f);

            uCxy = glGetUniformLocation(p, "uCxy");

            aVert = glGetAttribLocation(p, "aVert");
            aPosAnchor = glGetAttribLocation(p, "aPosAnchor");
            aScaleRadiansColorplus = glGetAttribLocation(p, "aScaleRadiansColorplus");
            aColor = glGetAttribLocation(p, "aColor");
            aTexRect = glGetAttribLocation(p, "aTexRect");
            CheckGLError();

            glGenVertexArrays(1, &va.id);
            glGenBuffers(1, &ib.id);
            glGenBuffers(1, &vb.id);

            glBindVertexArray(va);

            static const XY verts[4] = { { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f } };
            glBindBuffer(GL_ARRAY_BUFFER, ib);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
            glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
            glEnableVertexAttribArray(aVert);

            glBindBuffer(GL_ARRAY_BUFFER, vb);

            glVertexAttribPointer(aPosAnchor, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_GrassData), (GLvoid*)offsetof(Shader_GrassData, pos));
            glVertexAttribDivisor(aPosAnchor, 1);
            glEnableVertexAttribArray(aPosAnchor);

            glVertexAttribPointer(aScaleRadiansColorplus, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_GrassData), (GLvoid*)offsetof(Shader_GrassData, scale));
            glVertexAttribDivisor(aScaleRadiansColorplus, 1);
            glEnableVertexAttribArray(aScaleRadiansColorplus);

            glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_GrassData), (GLvoid*)offsetof(Shader_GrassData, color));
            glVertexAttribDivisor(aColor, 1);
            glEnableVertexAttribArray(aColor);

            glVertexAttribPointer(aTexRect, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Shader_GrassData), (GLvoid*)offsetof(Shader_GrassData, texRect));
            glVertexAttribDivisor(aTexRect, 1);
            glEnableVertexAttribArray(aTexRect);

            glBindVertexArray(0);
            CheckGLError();
        }

        virtual void Begin() override {
            assert(!GameBase::instance->shader);
            assert(count == 0);
            glUseProgram(p);
            glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
            glBindVertexArray(va);
        }

        virtual void End() override {
            assert(GameBase::instance->shader == this);
            if (count) {
                Commit();
            }
        }

        void Commit() {
            glBindBuffer(GL_ARRAY_BUFFER, vb);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_GrassData) * count, data.get(), GL_STREAM_DRAW);

            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
            CheckGLError();

            GameBase::instance->drawVerts += count * 6;
            GameBase::instance->drawCall += 1;

            count = 0;
        }

        XX_INLINE Shader_GrassData* Alloc(int32_t num_) {
            assert(GameBase::instance->shader == this);
            assert(num_ <= maxNums);
            if (count + num_ > maxNums) {
                Commit();
            }
            auto r = &data[count];
            count += num_;
            return r;
        }

        XX_INLINE void Draw(UVRect rect_, XY pos_ = {}, XY anchor_ = 0.5f
            , XY scale_ = 1.f, float radians_ = 0.f, float colorplus_ = 1.f, RGBA8 color_ = RGBA8_White) {
            Alloc(1)->Fill(rect_, pos_, anchor_, scale_, radians_, colorplus_, color_);
        }
    };

}
