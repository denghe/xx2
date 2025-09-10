#pragma once
#include "xx_gamebase.h"
#include "xx_shader_quad.h"
#include "xx_ui_scale9config.h"
#include "xx_embeds.h"
#include "xx_bmfont.h"
#include "xx_sound.h"

namespace xx {

	// shader support
	struct alignas(8) GameBase_shader : GameBase {
		inline static GameBase_shader* GetInstance() {
			return (GameBase_shader*)GameBase::instance;
		}

		// sound system
		Sound sound;

		// shaders
		Shader_Quad shaderQuad;
		XX_INLINE Shader_Quad& Quad() { return ShaderBegin(shaderQuad); }
		// ...

		// embed res
		struct {
			Ref<Scale9Config>
				cfg_s9bN, 
				cfg_s9bH, 
				cfg_s9bg, 
				cfg_sbar, 
				cfg_sblock;
			// ...

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
				ui_slider_block;
			// ...

			Ref<SoundSource>
				ss_ui_focus;
			// ...

			Ref<BMFont> font_sys;
		} embed;

		void BaseGLInit() {
			GameBase::BaseGLInit();

			// init pngs
			embed.ui_button = LoadTextureFromData(embeds::png::ui_button);
			embed.ui_button_h = LoadTextureFromData(embeds::png::ui_button_h);
			embed.ui_button_n = LoadTextureFromData(embeds::png::ui_button_n);
			embed.ui_checkbox_0 = LoadTextureFromData(embeds::png::ui_checkbox_0);
			embed.ui_checkbox_1 = LoadTextureFromData(embeds::png::ui_checkbox_1);
			embed.ui_imgbtn_h = LoadTextureFromData(embeds::png::ui_imgbtn_h);
			embed.ui_imgbtn_n = LoadTextureFromData(embeds::png::ui_imgbtn_n);
			embed.ui_dropdownlist_icon = LoadTextureFromData(embeds::png::ui_dropdownlist_icon);
			embed.ui_dropdownlist_head = LoadTextureFromData(embeds::png::ui_dropdownlist_head);
			embed.ui_panel = LoadTextureFromData(embeds::png::ui_panel);
			embed.ui_slider_bar = LoadTextureFromData(embeds::png::ui_slider_bar);
			embed.ui_slider_block = LoadTextureFromData(embeds::png::ui_slider_block);
			// ...

			// init cfgs
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

			// init fonts
			embed.font_sys.Emplace();
			embed.font_sys->texs.Emplace(LoadTextureFromData(embeds::png::font_sys));

			{
				auto d = LoadDataFromData(embeds::fnt::font_sys);
				embed.font_sys->Init(d, "font_sys", false);
			}

			// init oggs
			embed.ss_ui_focus = LoadSoundFromData((uint8_t*)embeds::ogg::ui_focus, _countof(embeds::ogg::ui_focus), false);

			// init shaders
			shaderQuad.Init();
			// ...

			// init sound
			sound.Init();
		}

		// d: ogg
		XX_INLINE Ref<SoundSource> LoadSoundFromData(uint8_t* buf, size_t len, bool looping = false) {
			assert(!IsCompressedData(buf, len));
			auto rtv = MakeRef<SoundSource>();
			rtv->wav.loadMem(buf, len, false, false);
			if (looping) {
				rtv->wav.setLooping(true);
			}
			return rtv;
		}

		XX_INLINE Ref<SoundSource> LoadSound(std::string_view fn, bool looping = false) {
			auto [d, p] = LoadFileData(fn);
			assert(d);
			return LoadSoundFromData(d.buf, d.len, looping);
		}

		XX_INLINE int PlayAudio(Ref<SoundSource> const& ss_, float volume_ = 1.f, float pan_ = 0.f, float speed_ = 1.f) {
			if (mute || audioVolume == 0) return 0;
			return sound.Play(ss_, volume_ * audioVolume, pan_, speed_);
		}

		XX_INLINE int PlayMusic(Ref<SoundSource> const& ss_, float volume_ = 1.f, float pan_ = 0.f, float speed_ = 1.f) {
			if (mute || musicVolume == 0) return 0;
			return sound.Play(ss_, volume_ * musicVolume, pan_, speed_);
		}
	};

}
