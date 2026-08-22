#include "xx_gamebase.h"

namespace xx {

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

}
 