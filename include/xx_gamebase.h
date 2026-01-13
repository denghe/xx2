#pragma once
#include "xx_gl.h"
#include "xx_grid2daabb.h"
#include "xx_shader_quad.h"
#include "xx_shader_quad_light.h"
#include "xx_shader_spine.h"
#include "xx_shader_texvert.h"
#include "xx_node.h"
#include "xx_bmfont.h"
#include "xx_input.h"
#include "xx_sound.h"

namespace xx {

    struct GameBase {
        inline static struct GameBase* instance{};

        GameBase(GameBase const&) = delete;
        GameBase& operator=(GameBase const&) = delete;
        GameBase() {
            instance = this;
        }
        virtual ~GameBase();

        // user need override
        virtual void Init() {}
        virtual void GLInit() {}
        virtual void Update() {}
        virtual void Delay() {}
        virtual void Stat() {}
        virtual void OnResize(bool modeChanged_) {}
        virtual void OnFocus(bool focused_) {}


        static constexpr XY minSize{ 384, 216 };			// for glfwSetWindowSizeLimits
        std::string title{ "game" };						// window title string( user can init )
        XY designSize{ 1920, 1080 };						// design resolution( user can init )
        XY windowSize{ designSize };						// physics resolution( user can init )
        XY worldMinXY{}, worldMaxXY{};						// for node( worldMinXY = -windowSize / 2,  worldMaxXY = windowSize / 2 );
        XY size{}, size_2{};								// actual design size
        float scale{};										// base scale. actual design size * base scale = physics resolution
        float flipY{ 1 };									// -1: flip  for ogl frame buffer

        /*
         screen anchor points
   ┌───────────────────────────────┐
   │ 7             8             9 │
   │                               │
   │                               │
   │ 4             5             6 │
   │                               │
   │                               │
   │ 1             2             3 │
   └───────────────────────────────┘
*/
        static constexpr XY a7{ 0, 1 }, a8{ 0.5, 1 }, a9{ 1, 1 };
        static constexpr XY a4{ 0, 0.5f }, a5{ 0.5, 0.5f }, a6{ 1, 0.5f };
        static constexpr XY a1{ 0, 0 }, a2{ 0.5, 0 }, a3{ 1, 0 };
        XY p7{}, p8{}, p9{};
        XY p4{}, p5{}, p6{};
        XY p1{}, p2{}, p3{};


        RGBA8 clearColor{};									// for glClearColor
        std::array<uint32_t, 3> blendDefault{ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD };
        std::array<uint32_t, 3> blend{ blendDefault };

        float time{}, delta{};								// usually for ui logic
        int32_t drawVerts{}, drawCall{}, drawFPS{};			// counters
        float drawFPSTimePool{};							// for count drawFPS

        float masterVolume{ 1.f }							// global sound settings
            , audioVolume{ 1.f }
        , musicVolume{ 0.5f };

        XY mousePos{};
        std::array<BtnState, GLFW_MOUSE_BUTTON_LAST + 1 + 4> mouse{};	// +4 for wheel up, down, left, right
        std::array<float, 4> wheelTimeouts{};				// store mouse wheel timeout
        std::array<BtnState, GLFW_KEY_LAST + 1> keyboard{};
        List<JoyState> joys;
        JoyState joy;										// joy = sum(joys) ( easy access for single player )

        bool running{};										// set false: quit app

        bool focused{};										// user readonly
        bool isFullScreen{};								// user readonly
        bool isBorderless{};								// user readonly
        bool minimized{};									// user readonly

        bool mute{};										// global sound settings

        Weak<Node> uiHandler;
        Grid2dAABB uiGrid;
        List<Weak<Node>> uiAutoUpdates;

        Shader* shader{};

        Sound sound;

        std::string rootPath;
        std::vector<std::string> searchPaths;
        std::filesystem::path tmpPath;

        List<std::function<int32_t()>> delayUpdates;		// call after update

        GLFWwindow* wnd{};
#ifdef WIN32
        HANDLE eventForDelay = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif

        List<ZNode> tmpZNodes;	// for node tree sort

        // for easy draw root node only
        void DrawNode(Node* tar) {
            FillZNodes(tmpZNodes, tar);
            OrderByZDrawAndClear(tmpZNodes);
        }

        // embed res
        struct {
            Shared<Scale9Config>
                cfg_s9,
                cfg_s9bN,
                cfg_s9bH,
                cfg_s9bg,
                cfg_sbar,
                cfg_sblock;
            // ...

            TinyFrame
                ui_s9,
                ui_button_h,
                ui_button_n,
                ui_checkbox_0,
                ui_checkbox_1,
                ui_imgbtn_h,
                ui_imgbtn_n,
                ui_dropdownlist_icon,
                ui_dropdownlist_head,
                ui_panel,
                ui_slider_bar,
                ui_slider_block,
                shape_dot,
                shape_gear,
                shape_heart;
            // ...

            List<TinyFrame> icon_flags_;

            Shared<SoLoud::Wav>
                ss_ui_focus;
            // ...

            Shared<BMFont> font_sys;
        } embed;


        // example:
        // GameBase::instance->delayUpdates.Emplace([w = WeakFromThis(this)] { if (!w) return 1; return 0; });
        void ExecuteDelayUpdates();
        void DisableIME();  // call before Run if needed


        // convert dir to search path format
        static std::string ToSearchPath(std::string_view dir);

        // add relative base dir to searchPaths
        void SearchPathAdd(std::string_view dir, bool insertToFront = false);

        // short file name to full path name( with search path )
        std::string GetFullPath(std::string_view fn, bool fnIsFileName = true);

        // read all data by full path
        Data LoadFileDataWithFullPath(std::string_view fp);

        // read all data by short path. return data
        Data LoadFileData(std::string_view fn);

        // auto decompress
        Data LoadDataFromData(uint8_t const* buf, size_t len);
        Data LoadDataFromData(Span d);
        template<size_t len>
        Data LoadDataFromData(const uint8_t(&buf)[len]) {
            return LoadDataFromData(buf, len);
        }

        // auto decompress
        Shared<GLTexture> LoadTextureFromData(void* buf_, size_t len_);
        Shared<GLTexture> LoadTextureFromData(Span d);
        template<size_t len>
        Shared<GLTexture> LoadTextureFromData(const uint8_t(&buf)[len]) {
            return LoadTextureFromData((void*)buf, len);
        }

        Shared<GLTexture> LoadTexture(std::string_view fn_);


        void ResizeCalc();  // for window resize event
        void SetWindowSize(XY siz); // for framebuffer only
        void GLViewport();
        void GLClear(RGBA8 c);
        void GLBlendFunc(std::array<uint32_t, 3> const& args);
        void GLBlendFunc();
        void SetBlendPremultipliedAlpha(bool e_);
        void SetDefaultBlendPremultipliedAlpha(bool e_);

        // internal funcs
		void HandleMouseMove(XY mp_);
        void HandleMouseButtonPressed(int32_t idx);
        void HandleMouseButtonReleased(int32_t idx);
        void HandleMouseWheel(double xoffset, double yoffset);
        void HandleKeyboardPressed(int32_t idx);
        void HandleKeyboardReleased(int32_t idx);
        void HandleJoystickConnected(int jid);
        void HandleJoystickDisconnected(int jid);
        void BaseUpdate();

        void SetMousePointerVisible(bool visible_);
        void SetFullScreenMode(XY size_ = {});
        void SetWindowMode(XY size_ = {});
        void SetBorderlessMode(bool b = true);
        void SleepSecs(double secs);    // example: SleepSecs(cDelta - (glfwGetTime() - time));


        // ogg / wav is supported
        Shared<SoLoud::Wav> LoadSoundSourceFromData(uint8_t* buf, size_t len, bool looping = false);
        Shared<SoLoud::Wav> LoadSoundSource(std::string_view fn, bool looping = false);
        unsigned int GetActiveVoiceCount();
        int PlayAudio(Shared<SoLoud::Wav> const& ss_, float volume_ = 1.f, float pan_ = 0.f, float speed_ = 1.f);
        int PlayMusic(Shared<SoLoud::Wav> const& ss_, float volume_ = 1.f, float pan_ = 0.f, float speed_ = 1.f);
        template<size_t len>
        Shared<SoLoud::Wav> LoadSoundSourceFromData(const uint8_t(&buf)[len], bool looping = false) {
            return LoadSoundSourceFromData((uint8_t*)buf, len, looping);
        }


        int32_t Run();
        void Loop(bool fromEvent);

        template<typename ST>
        ST& ShaderBegin(ST& s) {
            if (shader != &s) {
                ShaderEnd();
                s.Begin();
                shader = &s;
            }
            return s;
        }
        void ShaderEnd();

        Shader_Quad shaderQuad;     // default shader
        Shader_Quad& Quad() { return ShaderBegin(shaderQuad); }

        Shader_QuadLight shaderQuadLight;
        Shader_QuadLight& QuadLight() { return ShaderBegin(shaderQuadLight); }

        Shader_Spine shaderSpine;
        Shader_Spine& Spine() { return ShaderBegin(shaderSpine); }

        Shader_TexVert shaderTexVert;
        Shader_TexVert& TexVert() { return ShaderBegin(shaderTexVert); }
        // ...
    };

}
