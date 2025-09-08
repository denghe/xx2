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
		auto border = Make<xx::Scale9>();
		border->Init(z + 1, 0, 0, size);

		// swallow bg
		Make<xx::Background>()->Init(z, border.ToWeak()).onOutsideClicked = [] {};

		// center pos
		auto cp = size * 0.5f;
		auto fontSize = 80 - gg.embed.cfg_s9bN->paddings.TopBottom();

		// title
		Make<xx::Label>()->Init(z + 2, cp + XY{ 0, 180 }, 0.5, fontSize * 2)(lang(Strs::language));

		// langs list

		// en
		Make<xx::LabelButton>()->Init(z + 3, cp + XY{ -200, 0 }, 0.5, fontSize)(U"english").onClicked = [this] {
			lang.Set(Languages::en);
			MakeUI();	// unsafe
		};

		// cn
		Make<xx::LabelButton>()->Init(z + 3, cp + XY{ 200, 0 }, 0.5, fontSize)(U"中文").onClicked = [this] {
			lang.Set(Languages::cn);
			MakeUI();	// unsafe
		};

		// ...

		// close
		Make<xx::LabelButton>()->Init(z + 3, cp + XY{ 0, -300 + 30 }, { 0.5f, 0 }, fontSize)(lang(Strs::close)).onClicked = [this] {
			gg.lang.Set(lang.language);
			HandleESC();
		};
	}

	void Settings_Lang::HandleESC() {
		gg.langSelected = true;
		SwapRemove();
	}
}
