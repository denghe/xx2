#pragma once
#include "xx_gamebase.h"
#include "xx_shader_quad.h"
#include "xx_shader_spine.h"
#include "xx_shader_texvert.h"

#include "xx_ui_scale9config.h"
#include "xx_embeds.h"
#include "xx_bmfont.h"

namespace xx {

	struct alignas(8) GameBase_shader : GameBase {
		inline static GameBase_shader* GetInstance() {
			return (GameBase_shader*)GameBase::instance;
		}

		// shaders
		Shader_Quad shaderQuad;
		XX_INLINE Shader_Quad& Quad() { return ShaderBegin(shaderQuad); }

		Shader_Spine shaderSpine;
		XX_INLINE Shader_Spine& Spine() { return ShaderBegin(shaderSpine); }

		Shader_TexVert shaderTexVert;
		XX_INLINE Shader_TexVert& TexVert() { return ShaderBegin(shaderTexVert); }
		// ...

		// embed res
		struct {
			Ref<Scale9Config>
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

			Ref<SoundSource>
				ss_ui_focus;
			// ...

			Ref<BMFont> font_sys;
		} embed;

		void BaseGLInit() {
			GameBase::BaseGLInit();

			// init fonts
			embed.font_sys.Emplace();
			embed.font_sys->texs.Emplace(LoadTextureFromData(embeds::png::font_sys));

			{
				auto d = LoadDataFromData(embeds::fnt::font_sys);
				embed.font_sys->Init(d, "font_sys", false);
			}

			// init pngs( texture combined with font )
			auto& ft = embed.font_sys->texs[0];
			embed.ui_s9 = { ft, 944, 1008, 6, 6 };
			embed.ui_button_h = { ft, 944, 992, 6, 6 };
			embed.ui_button_n = { ft, 928, 992, 6, 6 };
			embed.ui_checkbox_0 = { ft, 992, 992, 32, 32 };
			embed.ui_checkbox_1 = { ft, 960, 992, 32, 32 };
			embed.ui_imgbtn_h = { ft, 848, 992, 6, 6 };
			embed.ui_imgbtn_n = { ft, 848, 1008, 6, 6 };
			embed.ui_dropdownlist_icon = { ft, 896, 992, 32, 32 };
			embed.ui_dropdownlist_head = { ft, 864, 992, 32, 32 };
			embed.ui_panel = { ft, 928, 1008, 6, 6 };
			embed.ui_slider_bar = { ft, 832, 1008, 6, 6 };
			embed.ui_slider_block = { ft, 832, 992, 6, 6 };

			embed.shape_dot = { ft, 752, 992, 1, 1 };
			embed.shape_gear = { ft, 768, 992, 32, 32 };
			embed.shape_heart = { ft, 802, 997, 29, 24 };
			embed.icon_flags_.Emplace(ft, 751, 1008, 16, 16);
			embed.icon_flags_.Emplace(ft, 734, 1008, 16, 16);

			// init cfgs
			embed.cfg_s9.Emplace();
			embed.cfg_s9->frame = embed.ui_s9;

			embed.cfg_s9bN.Emplace();
			embed.cfg_s9bN->frame = embed.ui_button_n;

			embed.cfg_s9bH.Emplace(*embed.cfg_s9bN);
			embed.cfg_s9bH->frame = embed.ui_button_h;

			embed.cfg_s9bg.Emplace(*embed.cfg_s9bN);
			embed.cfg_s9bg->frame = embed.ui_panel;

			embed.cfg_sbar.Emplace(*embed.cfg_s9bN);
			embed.cfg_sbar->frame = embed.ui_slider_bar;

			embed.cfg_sblock.Emplace(*embed.cfg_s9bN);
			embed.cfg_sblock->frame = embed.ui_slider_block;
			// ...

			// init sound sources
			embed.ss_ui_focus = LoadSoundSourceFromData(embeds::wav::ui_focus);

			// init shaders
			shaderQuad.Init();
			shaderSpine.Init();
			shaderTexVert.Init();
			// ...
		}

	};

}
