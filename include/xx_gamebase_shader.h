#pragma once
#include "xx_gamebase.h"
#include "xx_shader_quad.h"
#include "xx_ui_scale9config.h"
#include "xx_default_res.h"
#include "xx_bmfont.h"

namespace xx {

	// shader support
	struct alignas(8) GameBase_shader : GameBase {
		inline static GameBase_shader* GetInstance() {
			return (GameBase_shader*)GameBase::instance;
		}


		Shader_Quad shaderQuad;
		XX_INLINE Shader_Quad& Quad() { return ShaderBegin(shaderQuad); }
		// ...


		struct {
			Ref<Scale9Config>
				s9bN, 
				s9bH, 
				s9bg, 
				sbar, 
				sblock;
			// ...
		} defaultCfg;

		struct {
			Ref<GLTexture>
				ui_button,
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
				ui_slider_block
				// ...
				;
		} defaultRes;

		struct {
			Ref<BMFont> sys;
		} defaultFonts;

		void BaseGLInit() {
			GameBase::BaseGLInit();

			// init pngs
			defaultRes.ui_button = LoadTextureFromData(defaults::res::png::ui_button);
			defaultRes.ui_button_h = LoadTextureFromData(defaults::res::png::ui_button_h);
			defaultRes.ui_button_n = LoadTextureFromData(defaults::res::png::ui_button_n);
			defaultRes.ui_checkbox_0 = LoadTextureFromData(defaults::res::png::ui_checkbox_0);
			defaultRes.ui_checkbox_1 = LoadTextureFromData(defaults::res::png::ui_checkbox_1);
			defaultRes.ui_imgbtn_h = LoadTextureFromData(defaults::res::png::ui_imgbtn_h);
			defaultRes.ui_imgbtn_n = LoadTextureFromData(defaults::res::png::ui_imgbtn_n);
			defaultRes.ui_dropdownlist_icon = LoadTextureFromData(defaults::res::png::ui_dropdownlist_icon);
			defaultRes.ui_dropdownlist_head = LoadTextureFromData(defaults::res::png::ui_dropdownlist_head);
			defaultRes.ui_panel = LoadTextureFromData(defaults::res::png::ui_panel);
			defaultRes.ui_slider_bar = LoadTextureFromData(defaults::res::png::ui_slider_bar);
			defaultRes.ui_slider_block = LoadTextureFromData(defaults::res::png::ui_slider_block);
			// ...

			// init cfgs
			defaultCfg.s9bN.Emplace();
			defaultCfg.s9bN->frame = defaultRes.ui_button_n;

			defaultCfg.s9bH.Emplace(*defaultCfg.s9bN);
			defaultCfg.s9bH->frame = defaultRes.ui_button_h;

			defaultCfg.s9bg.Emplace(*defaultCfg.s9bN);
			defaultCfg.s9bg->frame = defaultRes.ui_panel;

			defaultCfg.sbar.Emplace(*defaultCfg.s9bN);
			defaultCfg.sbar->frame = defaultRes.ui_slider_bar;

			defaultCfg.sblock.Emplace(*defaultCfg.s9bN);
			defaultCfg.sblock->frame = defaultRes.ui_slider_block;
			// ...

			// init fonts
			{
				defaultFonts.sys.Emplace();
				defaultFonts.sys->texs.Emplace(LoadTextureFromData(defaults::res::png::font_sys));

				auto d = LoadDataFromData(defaults::res::fnt::font_sys);
				defaultFonts.sys->Init(d, "font_sys", false);
			}

			// init shaders
			shaderQuad.Init();
			// ...
		}
	};

}
