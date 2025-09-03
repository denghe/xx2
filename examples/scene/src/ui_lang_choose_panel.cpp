#include "pch.h"
#include "ui_lang_choose_panel.h"

/*
+-----------------------------+
           Language

   english              中文

	        [CLOSE]
+-----------------------------+
*/

UILangChoosePanel& UILangChoosePanel::Init(int32_t z_) {
	xx::Node::InitDerived<UILangChoosePanel>(z_, gg.p5, gg.a5, 1, { 800, 600 });
	lang.Set(gg.lang.language);	// copy current setting
	MakeUI();
	return *this;
}

void UILangChoosePanel::MakeUI() {
	children.Clear();	// unsafe

	// border
	auto c = MakeChildren<xx::Scale9>();
	c->Init(z + 1, 0, 0, size);


	auto cp = size * 0.5f;	// center pos
	auto fontSize = 80 - gg.defaultCfg.s9bN->paddings.TopBottom();

	// title
	MakeChildren<xx::Label>()->Init(z + 2, cp + XY{ 0, 180 }, 0.5, fontSize * 2)
		.SetText(lang(Strs::language));

	// lang btns
	MakeChildren<xx::LabelButton>()->Init(z + 3, cp + XY{ -200, 0 }, 0.5, fontSize)(U"english").onClicked = [this] {
		this->lang.Set(Languages::en);
		this->MakeUI();	// unsafe
	};

	// 
	MakeChildren<xx::LabelButton>()->Init(z + 3, cp + XY{ 200, 0 }, 0.5, fontSize)(U"中文").onClicked = [this] {
		this->lang.Set(Languages::cn);
		this->MakeUI();	// unsafe
	};

	// close
	MakeChildren<xx::LabelButton>()->Init(z + 3, cp + XY{ 0, -300 + 30 }, { 0.5f, 0 }, fontSize)(lang(Strs::close)).onClicked = [this] {
		this->onClose(lang.language);	// unsafe
	};

	// bg
	auto bg = MakeChildren<xx::Background>();
	bg->Init(z, c.ToWeak()).onOutsideClicked = [] {};	// swallow
}
