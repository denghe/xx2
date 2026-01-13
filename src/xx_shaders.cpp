#include "xx_gamebase.h"

namespace xx {
	
    /**************************************************************************************************/
    // xx_shader_quad.h
    /**************************************************************************************************/

    void Shader_QuadData::Fill(UVRect rect_, XY pos_, XY anchor_, XY scale_
        , float radians_, float colorplus_, RGBA8 color_) {
        pos = pos_;
        anchor = anchor_;
        scale = scale_;
        radians = radians_;
        colorplus = colorplus_;
        color = color_;
        texRect = rect_;
    }

    void Shader_Quad::Init() {

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
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;

in vec2 vTexCoord;
flat in float vColorplus;
flat in vec4 vColor;

out vec4 oColor;

void main() {
    vec4 c = vColor * texture(uTex0, vTexCoord / vec2(textureSize(uTex0, 0)));
    oColor = vec4( (c.x + 0.00001f) * vColorplus, (c.y + 0.00001f) * vColorplus, (c.z + 0.00001f) * vColorplus, c.w );
})"sv });

            p = LinkGLProgram(v, f);

            uCxy = glGetUniformLocation(p, "uCxy");
            uTex0 = glGetUniformLocation(p, "uTex0");

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

            glVertexAttribPointer(aPosAnchor, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, pos));
            glVertexAttribDivisor(aPosAnchor, 1);
            glEnableVertexAttribArray(aPosAnchor);

            glVertexAttribPointer(aScaleRadiansColorplus, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, scale));
            glVertexAttribDivisor(aScaleRadiansColorplus, 1);
            glEnableVertexAttribArray(aScaleRadiansColorplus);

            glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, color));
            glVertexAttribDivisor(aColor, 1);
            glEnableVertexAttribArray(aColor);

            glVertexAttribPointer(aTexRect, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, texRect));
            glVertexAttribDivisor(aTexRect, 1);
            glEnableVertexAttribArray(aTexRect);

            glBindVertexArray(0);
            CheckGLError();
        }

    void Shader_Quad::Begin() {
        assert(!GameBase::instance->shader);
        assert(lastTextureId == 0);
        assert(count == 0);
        glUseProgram(p);
        glUniform1i(uTex0, 0);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_Quad::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_Quad::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_QuadData) * count, data.get(), GL_STREAM_DRAW);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lastTextureId);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
        CheckGLError();

        GameBase::instance->drawVerts += count * 6;
        GameBase::instance->drawCall += 1;

        lastTextureId = 0;
        count = 0;
    }

    Shader_QuadData* Shader_Quad::Alloc(GLuint texId_, int32_t num_) {
        assert(GameBase::instance->shader == this);
        assert(num_ <= maxNums);
        if (count + num_ > maxNums || (lastTextureId && lastTextureId != texId_)) {
            Commit();
        }
        lastTextureId = texId_;
        auto r = &data[count];
        count += num_;
        return r;
    }

    void Shader_Quad::Draw(GLuint texId_, UVRect rect_, XY pos_, XY anchor_
        , XY scale_, float radians_, float colorplus_, RGBA8 color_) {
        Alloc(texId_, 1)->Fill(rect_, pos_, anchor_, scale_, radians_, colorplus_, color_);
    }

    void Shader_Quad::DrawTinyFrame(TinyFrame& tinyFrame_, XY pos_, XY anchor_
        , XY scale_, float radians_, float colorplus_, RGBA8 color_) {
        Alloc(tinyFrame_, 1)->Fill(tinyFrame_, pos_, anchor_, scale_, radians_, colorplus_, color_);
    }

    void Shader_Quad::DrawFrame(Frame& frame_, XY pos_, XY scale_, float radians_, float colorplus_, RGBA8 color_) {
        Alloc(frame_.tex->id, 1)->Fill(frame_, pos_, frame_.anchor, scale_, radians_, colorplus_, color_);
    }


    /**************************************************************************************************/
    // xx_shader_quad_light.h
    /**************************************************************************************************/

    void Shader_QuadLight::Init() {

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
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;
uniform sampler2D uTex1;

in vec2 vTexCoord;
flat in float vColorplus;
flat in vec4 vColor;

out vec4 oColor;

void main() {
vec2 uv = vTexCoord / vec2(textureSize(uTex0, 0));
vec4 c = vColor * texture(uTex0, uv);
vec4 c1 = texture(uTex1, uv);
oColor = vec4( (c.x + 0.00001f) * vColorplus * c1.x
                , (c.y + 0.00001f) * vColorplus * c1.y
                , (c.z + 0.00001f) * vColorplus * c1.z, c.w );
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uTex0 = glGetUniformLocation(p, "uTex0");
        uTex1 = glGetUniformLocation(p, "uTex1");

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

        glVertexAttribPointer(aPosAnchor, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, pos));
        glVertexAttribDivisor(aPosAnchor, 1);
        glEnableVertexAttribArray(aPosAnchor);

        glVertexAttribPointer(aScaleRadiansColorplus, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, scale));
        glVertexAttribDivisor(aScaleRadiansColorplus, 1);
        glEnableVertexAttribArray(aScaleRadiansColorplus);

        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, color));
        glVertexAttribDivisor(aColor, 1);
        glEnableVertexAttribArray(aColor);

        glVertexAttribPointer(aTexRect, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Shader_QuadData), (GLvoid*)offsetof(Shader_QuadData, texRect));
        glVertexAttribDivisor(aTexRect, 1);
        glEnableVertexAttribArray(aTexRect);

        glBindVertexArray(0);
        CheckGLError();
    }

    void Shader_QuadLight::Begin() {
        assert(!GameBase::instance->shader);
        assert(lastTextureId == 0);
        assert(lastLightTextureId == 0);
        assert(count == 0);
        glUseProgram(p);
        glUniform1i(uTex0, 0);
        glUniform1i(uTex1, 1);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_QuadLight::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_QuadLight::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_QuadData) * count, data.get(), GL_STREAM_DRAW);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lastTextureId);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, lastLightTextureId);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
        CheckGLError();

        GameBase::instance->drawVerts += count * 6;
        GameBase::instance->drawCall += 1;

        lastTextureId = 0;
        lastLightTextureId = 0;
        count = 0;
    }

    Shader_QuadData* Shader_QuadLight::Alloc(GLuint texId, GLuint lightTexId, int32_t num_) {
        assert(GameBase::instance->shader == this);
        assert(num_ <= maxNums);
        if (count + num_ > maxNums
            || (lastTextureId && lastTextureId != texId)
            || (lastLightTextureId && lastLightTextureId != lightTexId)
            ) {
            Commit();
        }
        lastTextureId = texId;
        lastLightTextureId = lightTexId;
        auto r = &data[count];
        count += num_;
        return r;
    }

    void Shader_QuadLight::Draw(Shared<GLTexture> tex, Shared<GLTexture> lightTex, RGBA8 color, float colorplus) {
        Alloc(*tex, *lightTex, 1)->Fill(*tex, 0, 0.5f, 1.f, 0, colorplus, color);
    }

    /**************************************************************************************************/
    // xx_shader_spine38.h
    /**************************************************************************************************/

    void Shader_Spine::Init() {
        v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	// screen center coordinate

in vec4 aPosUv;
in vec4 aColor;

out vec4 vColor;
out vec2 vTexCoord;

void main() {
gl_Position = vec4(aPosUv.xy * uCxy, 0, 1);
vTexCoord = aPosUv.zw;
vColor = aColor;
})"sv });

        f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;
uniform sampler2D uTex0;

in vec4 vColor;
in vec2 vTexCoord;

out vec4 oColor;

void main() {
oColor = vColor * texture(uTex0, vTexCoord / vec2(textureSize(uTex0, 0)));
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uTex0 = glGetUniformLocation(p, "uTex0");

        aPosUv = glGetAttribLocation(p, "aPosUv");
        aColor = glGetAttribLocation(p, "aColor");
        CheckGLError();

        glGenVertexArrays(1, &va.id);
        glGenBuffers(1, &vb.id);

        glBindVertexArray(va);

        glBindBuffer(GL_ARRAY_BUFFER, vb);

        glVertexAttribPointer(aPosUv, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_SpineData), (GLvoid*)offsetof(Shader_SpineData, pos));
        glEnableVertexAttribArray(aPosUv);

        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_SpineData), (GLvoid*)offsetof(Shader_SpineData, color));
        glEnableVertexAttribArray(aColor);

        glBindVertexArray(0);
        CheckGLError();
    }

    void Shader_Spine::Begin() {
        assert(!GameBase::instance->shader);
        assert(count == 0);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0/* + textureUnit*/);
        glUniform1i(uTex0, 0);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_Spine::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_Spine::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_SpineData) * count, data.get(), GL_STREAM_DRAW);

        glBindTexture(GL_TEXTURE_2D, lastTextureId);
        glDrawArrays(GL_TRIANGLES, 0, count);
        CheckGLError();

        GameBase::instance->drawVerts += count;
        GameBase::instance->drawCall += 1;

        lastTextureId = 0;
        count = 0;
    }

    Shader_SpineData* Shader_Spine::Alloc(GLuint texId_, int32_t num_) {
        assert(num_ <= maxNums);
        if (count + num_ > maxNums || (lastTextureId && lastTextureId != texId_)) {
            Commit();
        }
        lastTextureId = texId_;
        auto vc = count;
        count += num_;
        return &data[vc];
    }

    /**************************************************************************************************/
    // xx_shader_texvert.h
    /**************************************************************************************************/

    void Shader_TexVert::Init() {

        v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	                                            // screen center coordinate
uniform sampler2D uTex1;                                        // Shader_TexVertElement[][]

in int aVertIndex;                                              // vert index: 0, 1, 2, ...... 8191

in vec4 aPosScale;
in int aFrameIndex;

out vec2 vTexCoord;
out vec4 vColor;

void main() {
vec2 pos = aPosScale.xy;
vec2 scale = aPosScale.zw;
int offset = aVertIndex * 2;
vec4 xyuv = texelFetch(uTex1, ivec2(offset + 0, aFrameIndex), 0);
vColor    = texelFetch(uTex1, ivec2(offset + 1, aFrameIndex), 0);
vTexCoord = xyuv.zw;
gl_Position = vec4((pos + xyuv.xy * scale) * uCxy, 0, 1);
})"sv });

        f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;
uniform sampler2D uTex0;

in vec2 vTexCoord;
in vec4 vColor;

out vec4 oColor;

void main() {
oColor = vColor * texture(uTex0, vTexCoord / vec2(textureSize(uTex0, 0)));
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uTex0 = glGetUniformLocation(p, "uTex0");
        uTex1 = glGetUniformLocation(p, "uTex1");

        aVertIndex = glGetAttribLocation(p, "aVertIndex");
        aPosScale = glGetAttribLocation(p, "aPosScale");
        aFrameIndex = glGetAttribLocation(p, "aFrameIndex");
        CheckGLError();

        glGenVertexArrays(1, &va.id);
        glGenBuffers(1, &ib.id);
        glGenBuffers(1, &vb.id);

        glBindVertexArray(va);

        auto d = std::make_unique<int[]>(vertCap);
        for (int i = 0; i < vertCap; ++i) d[i] = i;
        glBindBuffer(GL_ARRAY_BUFFER, ib);
        glBufferData(GL_ARRAY_BUFFER, vertCap * 4, d.get(), GL_STATIC_DRAW);
        glVertexAttribIPointer(aVertIndex, 1, GL_INT, sizeof(int), 0);
        glEnableVertexAttribArray(aVertIndex);

        glBindBuffer(GL_ARRAY_BUFFER, vb);

        glVertexAttribPointer(aPosScale, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_TexVertData), (GLvoid*)offsetof(Shader_TexVertData, pos));
        glVertexAttribDivisor(aPosScale, 1);
        glEnableVertexAttribArray(aPosScale);

        glVertexAttribIPointer(aFrameIndex, 1, GL_INT, sizeof(Shader_TexVertData), (GLvoid*)offsetof(Shader_TexVertData, frameIndex));
        glVertexAttribDivisor(aFrameIndex, 1);
        glEnableVertexAttribArray(aFrameIndex);

        glBindVertexArray(0);
        CheckGLError();
    }

    void Shader_TexVert::Begin() {
        assert(!GameBase::instance->shader);
        assert(lastTextureId == 0);
        assert(lastVertTextureId == 0);
        assert(count == 0);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0 + 0);
        glActiveTexture(GL_TEXTURE0 + 1);
        glUniform1i(uTex0, 0);
        glUniform1i(uTex1, 1);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_TexVert::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_TexVert::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_TexVertData) * count, data.get(), GL_STREAM_DRAW);

        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, lastTextureId);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, lastVertTextureId);

        glDrawArraysInstanced(GL_TRIANGLES, 0, numVerts, count);
        CheckGLError();

        GameBase::instance->drawVerts += count * numVerts;
        GameBase::instance->drawCall += 1;

        lastTextureId = 0;
        lastVertTextureId = 0;
        count = 0;
    }

    Shader_TexVertData* Shader_TexVert::Alloc(Shared<GLTexture> tex_, Shared<GLVertTexture> const& vertTex_, int32_t num_) {
        assert(GameBase::instance->shader == this);
        assert(num_ <= maxNums);
        auto texId = tex_->id;
        auto vertTexId = vertTex_->id;
        if (count + num_ > maxNums
            || (lastTextureId && lastTextureId != texId)
            || (lastVertTextureId && lastVertTextureId != vertTexId)) {
            Commit();
        }
        lastTextureId = texId;
        lastVertTextureId = vertTexId;
        numVerts = vertTex_->numVerts;
        assert(vertCap >= numVerts);
        auto r = &data[count];
        count += num_;
        return r;
    }

}
