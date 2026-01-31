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
    //oColor = vec4( (c.x + 0.00001f) * vColorplus, (c.y + 0.00001f) * vColorplus, (c.z + 0.00001f) * vColorplus, c.w );
    oColor = vec4( c.x * vColorplus, c.y * vColorplus, c.z * vColorplus, c.w );
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
oColor = vec4( c.x * vColorplus * c1.x, c.y * vColorplus * c1.y, c.z * vColorplus * c1.z, c.w );
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

    /**************************************************************************************************/
    // xx_shader_texvert.h
    /**************************************************************************************************/

    void Shader_Line::Init() {
        v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;
uniform vec2 uCxy;	// screen center coordinate

in vec2 aPos;
in vec4 aColor;

out vec4 vColor;

void main() {
	gl_Position = vec4(aPos * uCxy, 0, 1);
	vColor = aColor;
})"sv });

        f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;

in vec4 vColor;

out vec4 oColor;

void main() {
	oColor = vColor;
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");

        aPos = glGetAttribLocation(p, "aPos");
        aColor = glGetAttribLocation(p, "aColor");
        CheckGLError();

        glGenVertexArrays(1, &va.id);
        glGenBuffers(1, &vb.id);
        glGenBuffers(1, &ib.id);

        glBindVertexArray(va);

        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE, sizeof(Shader_LineData), 0);
        glEnableVertexAttribArray(aPos);
        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_LineData), (GLvoid*)offsetof(Shader_LineData, r));
        glEnableVertexAttribArray(aColor);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

        glBindVertexArray(0);
        CheckGLError();
    }

    void Shader_Line::Begin() {
        assert(!GameBase::instance->shader);
        assert(pointsCount == 0 && indexsCount == 0);
        glUseProgram(p);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_Line::End() {
        assert(GameBase::instance->shader == this);
        if (indexsCount) {
            assert(pointsCount);
            Commit();
        }
    }

    void Shader_Line::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_LineData) * pointsCount, points.get(), GL_STREAM_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indexsCount, indexs.get(), GL_STREAM_DRAW);

        glDrawElements(GL_LINE_STRIP, indexsCount, GL_UNSIGNED_SHORT, 0);

		GameBase::instance->drawVerts += indexsCount;
        GameBase::instance->drawCall += 1;

        pointsCount = 0;
        indexsCount = 0;
    }

    Shader_LineData* Shader_Line::Alloc(int32_t num_) {
        assert(num_ <= maxVertNums);
        auto c = pointsCount + num_;
        if (c > maxVertNums) {
            Commit();
            c = num_;
        }
        auto rtv = &points[pointsCount];
        for (size_t i = pointsCount; i < c; ++i) {
            indexs[indexsCount++] = i;
        }
        indexs[indexsCount++] = 65535;	// primitive restart
        assert(indexsCount <= maxIndexNums);
        pointsCount = c;
        return rtv;
    }

    void Shader_Line::Fill(Shader_LineData const* buf_, int32_t num_) {
        memcpy(Alloc(num_), buf_, sizeof(Shader_LineData) * num_);
    }


    /**************************************************************************************************/
    // xx_shader_numbers.h
    /**************************************************************************************************/

    int32_t Shader_NumbersData::FillTxt(std::string_view n_) {
        assert(n_.size());
        auto buf = n_.data();
        auto len = n_.size();
        auto tar = (uint8_t*)&numbers;
        for (size_t i = 0; i < len; ++i) {
            tar[i] = buf[i] - '0';
        }
        tar[15] = (uint8_t)len;
		return (int32_t)len;
    }

    int32_t Shader_NumbersData::FillTxt(int32_t n_, bool includeSignal_) {
        std::string s;
        if (includeSignal_) {
            if (n_ >= 0) {
                s.push_back((char)58);
                Append(s, (uint32_t)n_);
            }
            else {
                s.push_back((char)59);
                Append(s, (uint32_t)-n_);
            }
        }
        else {
            Append(s, std::abs(n_));
        }
        return FillTxt(s);
    }

    void Shader_NumbersData::FillBase(XY pos_, XY scale_, RGBA8 color_) {
        pos = pos_;
        scale = scale_;
        color = color_;
    }

    int32_t Shader_NumbersData::Fill(XY pos_, XY scale_, RGBA8 color_, std::string_view n_) {
        FillBase(pos_, scale_, color_);
        return FillTxt(n_);
    }

    int32_t Shader_NumbersData::Fill(XY pos_, XY scale_, RGBA8 color_, int32_t n_, bool includeSignal_) {
        FillBase(pos_, scale_, color_);
        return FillTxt(n_, includeSignal_);
    }


    void Shader_Numbers::Init() {

        v = LoadGLVertexShader({ XX_SHADER_CODE_FIRST_LINE R"(
uniform vec2 uCxy;	            // screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosScale;
in vec4 aColor;
in ivec4 aNumbers;

out vec2 vTexCoord;
out vec4 vColor;
flat out ivec4 vNumbers;

void main() {
    vec2 pos = aPosScale.xy;
    vec2 scale = aPosScale.zw;
    vec2 offset = aVert * scale;
    float len = float(aNumbers.w >> 24);
    vec2 size = vec2(9.f * len, 13.f);
    gl_Position = vec4((pos + size * offset) * uCxy, 0.f, 1.f);
    vColor = aColor;
    vTexCoord = vec2(9.f * len * aVert.x, 13.f - 13.f * aVert.y);
    vNumbers = aNumbers;
})"sv });

        f = LoadGLFragmentShader({ XX_SHADER_CODE_FIRST_LINE R"(
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;

in vec2 vTexCoord;
in vec4 vColor;
flat in ivec4 vNumbers;

out vec4 oColor;

void main() {
    uint i = uint(vTexCoord.x * 0.1111111f);        // / 9
    uint rIdx = (i >> 2);                           // / 4
    uint cIdx = i - (rIdx << 2);                    // * 4
    uint n4 = uint(vNumbers[rIdx]);
    uint n = (n4 >> (cIdx << 3)) & 0xffu;           // * 8
    vec2 uv = vec2(10.f * float(n) + vTexCoord.x - float(i) * 9.f, vTexCoord.y);
    //vec2 uv = vTexCoord;
    oColor = vColor * texture(uTex0, uv / vec2(120.f, 13.f));
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uTex0 = glGetUniformLocation(p, "uTex0");

        aVert = glGetAttribLocation(p, "aVert");
        aPosScale = glGetAttribLocation(p, "aPosScale");
        aColor = glGetAttribLocation(p, "aColor");
        aNumbers = glGetAttribLocation(p, "aNumbers");
        CheckGLError();

        glGenVertexArrays(1, &va.id);
        glBindVertexArray(va);

        glGenBuffers(1, &ib.id);
        static const XY verts[4] = { { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f } };
        glBindBuffer(GL_ARRAY_BUFFER, ib);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
        glEnableVertexAttribArray(aVert);

        glGenBuffers(1, &vb.id);
        glBindBuffer(GL_ARRAY_BUFFER, vb);

        glVertexAttribPointer(aPosScale, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_NumbersData), 0);
        glVertexAttribDivisor(aPosScale, 1);
        glEnableVertexAttribArray(aPosScale);

        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_NumbersData), (GLvoid*)offsetof(Shader_NumbersData, color));
        glVertexAttribDivisor(aColor, 1);
        glEnableVertexAttribArray(aColor);

        // "I" Pointer for int
        glVertexAttribIPointer(aNumbers, 4, GL_INT, sizeof(Shader_NumbersData), (GLvoid*)offsetof(Shader_NumbersData, numbers));
        glVertexAttribDivisor(aNumbers, 1);
        glEnableVertexAttribArray(aNumbers);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        CheckGLError();
    }

    void Shader_Numbers::Begin() {
        assert(!GameBase::instance->shader);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0/* + textureUnit*/);
        glUniform1i(uTex0, 0);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_Numbers::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_Numbers::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_NumbersData) * count, data.get(), GL_STREAM_DRAW);

        glBindTexture(GL_TEXTURE_2D, GameBase::instance->embed.png_numbers);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

        CheckGLError();

        GameBase::instance->drawVerts += count * 6;
        GameBase::instance->drawCall += 1;

        count = 0;
    }

    Shader_NumbersData* Shader_Numbers::Alloc(int32_t num_) {
        assert(GameBase::instance->shader == this);
        if (count + num_ > maxNums) {
            Commit();
        }
        auto r = &data[count];
        count += num_;
        return r;
    }

    void Shader_Numbers::Draw(Shader_NumbersData const* buf_, int32_t len_) {
        auto ptr = Alloc(len_);
        memcpy(ptr, buf_, sizeof(Shader_NumbersData) * len_);
    }

    void Shader_Numbers::Draw(Shader_NumbersData const& d_) {
        Draw(&d_, 1);
    }

    /**************************************************************************************************/
    // xx_shader_quad_outline.h
    /**************************************************************************************************/

    void Shader_QuadOutlineData::Fill(UVRect rect_, XY pos_, XY anchor_, XY scale_
        , float radians_, float colorplus_, RGBA8 color_
        , RGBA8 outlineColor_, XY outlineThickness_) {
        pos = pos_;
        anchor = anchor_;
        scale = scale_;
        radians = radians_;
        colorplus = colorplus_;
        color = color_;
        texRect = rect_;
        outlineColor = outlineColor_;
        outlineThickness = outlineThickness_;
    }

    void Shader_QuadOutline::Init() {

        v = LoadGLVertexShader({ R"(#version 300 es
uniform vec2 uCxy;	// screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosAnchor;
in vec4 aScaleRadiansColorplus;
in vec4 aColor;
in vec4 aTexRect;
in vec4 aOutlineColor;
in vec2 aOutlineThickness;

out vec2 vTexCoord;
flat out float vColorplus;
flat out vec4 vColor;
flat out vec4 vOutlineColor;
flat out vec2 vOutlineThickness;

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
vOutlineColor = aOutlineColor;
vOutlineThickness = aOutlineThickness;
})"sv });

        f = LoadGLFragmentShader({ R"(#version 300 es
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;

in vec2 vTexCoord;
flat in float vColorplus;
flat in vec4 vColor;
flat in vec4 vOutlineColor;
flat in vec2 vOutlineThickness;

out vec4 oColor;

void main() {
vec2 texSize = vec2(textureSize(uTex0, 0));

vec2 p1 = vec2(vTexCoord.x - vOutlineThickness.x, vTexCoord.y + vOutlineThickness.y);
vec2 p2 = vec2(vTexCoord.x + vOutlineThickness.x, vTexCoord.y + vOutlineThickness.y);
vec2 p3 = vec2(vTexCoord.x + vOutlineThickness.x, vTexCoord.y - vOutlineThickness.y);
vec2 p4 = vec2(vTexCoord.x - vOutlineThickness.x, vTexCoord.y - vOutlineThickness.y);
float a1 = vColor.w * texture(uTex0, p1 / texSize).w;
float a2 = vColor.w * texture(uTex0, p2 / texSize).w;
float a3 = vColor.w * texture(uTex0, p3 / texSize).w;
float a4 = vColor.w * texture(uTex0, p4 / texSize).w;

// vec2 p5 = vec2(vTexCoord.x, vTexCoord.y + vOutlineThickness.y);
// vec2 p6 = vec2(vTexCoord.x, vTexCoord.y - vOutlineThickness.y);
// vec2 p7 = vec2(vTexCoord.x + vOutlineThickness.x, vTexCoord.y);
// vec2 p8 = vec2(vTexCoord.x - vOutlineThickness.x, vTexCoord.y);
// float a5 = vColor.w * texture(uTex0, p5 / texSize).w;
// float a6 = vColor.w * texture(uTex0, p6 / texSize).w;
// float a7 = vColor.w * texture(uTex0, p7 / texSize).w;
// float a8 = vColor.w * texture(uTex0, p8 / texSize).w;

vec4 c = vColor * texture(uTex0, vTexCoord / texSize);
c = clamp(vec4( (c.x + 0.00001f) * vColorplus, (c.y + 0.00001f) * vColorplus, (c.z + 0.00001f) * vColorplus, c.w ), 0.f, 1.f);
float a = clamp(a1 + a2 + a3 + a4
// + a5 + a6 + a7 + a8
, 0.f, 1.f) - c.w;
if (a > 0.5f) oColor = vOutlineColor;
else oColor = c;
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uTex0 = glGetUniformLocation(p, "uTex0");

        aVert = glGetAttribLocation(p, "aVert");
        aPosAnchor = glGetAttribLocation(p, "aPosAnchor");
        aScaleRadiansColorplus = glGetAttribLocation(p, "aScaleRadiansColorplus");
        aColor = glGetAttribLocation(p, "aColor");
        aTexRect = glGetAttribLocation(p, "aTexRect");
        aOutlineColor = glGetAttribLocation(p, "aOutlineColor");
        aOutlineThickness = glGetAttribLocation(p, "aOutlineThickness");
        CheckGLError();

        glGenVertexArrays(1, &va.id);
        glBindVertexArray(va);

        glGenBuffers(1, (GLuint*)&ib);
        static const XY verts[4] = { { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f } };
        glBindBuffer(GL_ARRAY_BUFFER, ib);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
        glEnableVertexAttribArray(aVert);

        glGenBuffers(1, (GLuint*)&vb);
        glBindBuffer(GL_ARRAY_BUFFER, vb);

        glVertexAttribPointer(aPosAnchor, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadOutlineData), 0);  // offsetof(Shader_QuadOutlineData, pos
        glVertexAttribDivisor(aPosAnchor, 1);
        glEnableVertexAttribArray(aPosAnchor);

        glVertexAttribPointer(aScaleRadiansColorplus, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadOutlineData), (GLvoid*)offsetof(Shader_QuadOutlineData, scale));
        glVertexAttribDivisor(aScaleRadiansColorplus, 1);
        glEnableVertexAttribArray(aScaleRadiansColorplus);

        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_QuadOutlineData), (GLvoid*)offsetof(Shader_QuadOutlineData, color));
        glVertexAttribDivisor(aColor, 1);
        glEnableVertexAttribArray(aColor);

        glVertexAttribPointer(aTexRect, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Shader_QuadOutlineData), (GLvoid*)offsetof(Shader_QuadOutlineData, texRect));
        glVertexAttribDivisor(aTexRect, 1);
        glEnableVertexAttribArray(aTexRect);

        glVertexAttribPointer(aOutlineColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_QuadOutlineData), (GLvoid*)offsetof(Shader_QuadOutlineData, outlineColor));
        glVertexAttribDivisor(aOutlineColor, 1);
        glEnableVertexAttribArray(aOutlineColor);

        glVertexAttribPointer(aOutlineThickness, 2, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadOutlineData), (GLvoid*)offsetof(Shader_QuadOutlineData, outlineThickness));
        glVertexAttribDivisor(aOutlineThickness, 1);
        glEnableVertexAttribArray(aOutlineThickness);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        CheckGLError();
    }

    void Shader_QuadOutline::Begin() {
        assert(!GameBase::instance->shader);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0/* + textureUnit*/);
        glUniform1i(uTex0, 0);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_QuadOutline::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_QuadOutline::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_QuadOutlineData) * count, data.get(), GL_STREAM_DRAW);

        glBindTexture(GL_TEXTURE_2D, lastTextureId);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

        CheckGLError();

        GameBase::instance->drawVerts += count * 6;
        GameBase::instance->drawCall += 1;

        lastTextureId = 0;
        count = 0;
    }

    Shader_QuadOutlineData* Shader_QuadOutline::Alloc(GLuint texId, int32_t num_) {
        assert(GameBase::instance->shader == this);
        assert(num_ <= maxNums);
        if (count + num_ > maxNums || (lastTextureId && lastTextureId != texId)) {
            Commit();
        }
        lastTextureId = texId;
        auto r = &data[count];
        count += num_;
        return r;
    }

    Shader_QuadOutlineData* Shader_QuadOutline::Alloc(GLuint texId, UVRect const& uvRect, float outlineThickness, RGBA8 outlineColor) {
        auto q = Alloc(texId, 1);
        q->pos = {};
        q->anchor = 0.5f;
        q->scale = 1;
        q->radians = 0;
        q->colorplus = 1.f;
        q->color = xx::RGBA8_White;
        q->texRect.x = uint16_t(uvRect.x - outlineThickness - 1.f);
        q->texRect.y = uint16_t(uvRect.y - outlineThickness - 1.f);
        q->texRect.w = uint16_t(uvRect.w + (outlineThickness + 1.f) * 2.f);
        q->texRect.h = uint16_t(uvRect.h + (outlineThickness + 1.f) * 2.f);
        q->outlineColor = outlineColor;
        q->outlineThickness = outlineThickness;
        return q;
    }

    /**************************************************************************************************/
    // xx_shader_quad_tiling_offset.h
    /**************************************************************************************************/

    void Shader_QuadExData::Fill(UVRect rect_, XY pos_, XY anchor_, XY scale_
        , float radians_, float colorplus_, RGBA8 color_
        , XY tiling_, XY offset_) {
        pos = pos_;
        anchor = anchor_;
        scale = scale_;
        radians = radians_;
        colorplus = colorplus_;
        color = color_;
        texRect = rect_;
        tiling = tiling_;
        offset = offset_;
    }

    void Shader_QuadEx::Init() {

        v = LoadGLVertexShader({ R"(#version 300 es
uniform vec2 uCxy;	// screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosAnchor;
in vec4 aScaleRadiansColorplus;
in vec4 aColor;
in vec4 aTexRect;
in vec4 aTilingOffset;

out vec2 vTexCoord;
flat out float vColorplus;
flat out vec4 vColor;
flat out vec4 vTexRect;

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
vTexCoord = (vec2(aVert.x * aTexRect.z, aTexRect.w - aVert.y * aTexRect.w) + aTilingOffset.zw) * aTilingOffset.xy;
vTexRect = aTexRect;
})"sv });

        f = LoadGLFragmentShader({ R"(#version 300 es
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;

in vec2 vTexCoord;
flat in float vColorplus;
flat in vec4 vColor;
flat in vec4 vTexRect;

out vec4 oColor;

void main() {
vec2 uv = vTexRect.xy + mod(vTexCoord, vTexRect.zw);
vec4 c = vColor * texture(uTex0, uv / vec2(textureSize(uTex0, 0)));
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
        aTilingOffset = glGetAttribLocation(p, "aTilingOffset");
        CheckGLError();

        glGenVertexArrays(1, &va.id);
        glBindVertexArray(va);

        glGenBuffers(1, (GLuint*)&ib);
        static const XY verts[4] = { { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f } };
        glBindBuffer(GL_ARRAY_BUFFER, ib);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
        glEnableVertexAttribArray(aVert);

        glGenBuffers(1, (GLuint*)&vb);
        glBindBuffer(GL_ARRAY_BUFFER, vb);

        glVertexAttribPointer(aPosAnchor, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadExData), 0);  // offsetof(Shader_QuadExData, pos
        glVertexAttribDivisor(aPosAnchor, 1);
        glEnableVertexAttribArray(aPosAnchor);

        glVertexAttribPointer(aScaleRadiansColorplus, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadExData), (GLvoid*)offsetof(Shader_QuadExData, scale));
        glVertexAttribDivisor(aScaleRadiansColorplus, 1);
        glEnableVertexAttribArray(aScaleRadiansColorplus);

        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Shader_QuadExData), (GLvoid*)offsetof(Shader_QuadExData, color));
        glVertexAttribDivisor(aColor, 1);
        glEnableVertexAttribArray(aColor);

        glVertexAttribPointer(aTexRect, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Shader_QuadExData), (GLvoid*)offsetof(Shader_QuadExData, texRect));
        glVertexAttribDivisor(aTexRect, 1);
        glEnableVertexAttribArray(aTexRect);

        glVertexAttribPointer(aTilingOffset, 4, GL_FLOAT, GL_FALSE, sizeof(Shader_QuadExData), (GLvoid*)offsetof(Shader_QuadExData, tiling));
        glVertexAttribDivisor(aTilingOffset, 1);
        glEnableVertexAttribArray(aTilingOffset);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        CheckGLError();
    }

    void Shader_QuadEx::Begin() {
        assert(!GameBase::instance->shader);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0/* + textureUnit*/);
        glUniform1i(uTex0, 0);
        glUniform2f(uCxy, 2 / GameBase::instance->windowSize.x, 2 / GameBase::instance->windowSize.y * GameBase::instance->flipY);
        glBindVertexArray(va);
    }

    void Shader_QuadEx::End() {
        assert(GameBase::instance->shader == this);
        if (count) {
            Commit();
        }
    }

    void Shader_QuadEx::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shader_QuadExData) * count, data.get(), GL_STREAM_DRAW);

        glBindTexture(GL_TEXTURE_2D, lastTextureId);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

        CheckGLError();

        GameBase::instance->drawVerts += count * 6;
        GameBase::instance->drawCall += 1;

        lastTextureId = 0;
        count = 0;
    }

    Shader_QuadExData* Shader_QuadEx::Alloc(GLuint texId, int32_t numQuads) {
        assert(GameBase::instance->shader == this);
        assert(numQuads <= maxNums);
        if (count + numQuads > maxNums || (lastTextureId && lastTextureId != texId)) {
            Commit();
        }
        lastTextureId = texId;
        auto r = &data[count];
        count += numQuads;
        return r;
    }

}
 