#include "pch.h"
#include "ui_settings_lang.h"

/*
+-----------------------------+
           Language

   english              中文

	        [CLOSE]
+-----------------------------+
*/

namespace UI {

	void Settings_Lang::Init(int32_t z_) {
		escHandler = true;
		lang.Set(gg.lang.language);
		xx::Node::InitDerived<Settings_Lang>(z_, gg.p5, gg.a5, 1, { 800, 600 });
		MakeUI();
	}

	void Settings_Lang::MakeUI() {
		children.Clear();	// unsafe

		// border
		auto c = MakeChildren<xx::Scale9>();
		c->Init(z + 1, 0, 0, size);

		auto cp = size * 0.5f;	// center pos
		auto fontSize = 80 - gg.defaultCfg.s9bN->paddings.TopBottom();

		// title
		MakeChildren<xx::Label>()->Init(z + 2, cp + XY{ 0, 180 }, 0.5, fontSize * 2)
			.SetText(lang(Strs::language));

		// langs list
		MakeChildren<xx::LabelButton>()->Init(z + 3, cp + XY{ -200, 0 }, 0.5, fontSize)(U"english").onClicked = [this] {
			lang.Set(Languages::en);
			MakeUI();	// unsafe
		};

		// 
		MakeChildren<xx::LabelButton>()->Init(z + 3, cp + XY{ 200, 0 }, 0.5, fontSize)(U"中文").onClicked = [this] {
			lang.Set(Languages::cn);
			MakeUI();	// unsafe
		};

		// close
		MakeChildren<xx::LabelButton>()->Init(z + 3, cp + XY{ 0, -300 + 30 }, { 0.5f, 0 }, fontSize)(gg.lang(Strs::close)).onClicked = [this] {
			gg.lang.Set(lang.language);
			HandleESC();
		};

		// bg
		auto bg = MakeChildren<xx::Background>();
		bg->Init(z, c.ToWeak()).onOutsideClicked = [] {};	// swallow
	}
}
