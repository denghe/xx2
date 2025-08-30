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
	xx::Node::Init(z_, gg.p5, gg.a5, 1, { 800, 600 });
	lang.SetLanguage(gg.lang.language);
	Refresh();
	return *this;
}

void UILangChoosePanel::Refresh() {
	children.Clear();

	// border
	auto c = MakeChildren<xx::Scale9>();
	c->Init(z + 1, 0, 0, gg.cfg.s9bg->borderScale, size / gg.cfg.s9bg->borderScale, *gg.cfg.s9bg);


	auto cp = size * 0.5f;	// center pos

	// title
	MakeChildren<xx::Label>()->Init(z + 2, cp + XY{ 0, 180 }, 0.5, 3)
		.SetText(lang(Strs::language));

	// lang btns
	MakeChildren<xx::LabelButton>()->Init(z + 3, cp + XY{ -200, 0 }, 0.5
		, gg.cfg.s9bN, gg.cfg.s9bH, U"english")
		.onClicked = [this] {
		this->lang.SetLanguage(Languages::en);
		this->Refresh();	// unsafe
	};

	// 
	MakeChildren<xx::LabelButton>()->Init(z + 3, cp + XY{ 200, 0 }, 0.5
		, gg.cfg.s9bN, gg.cfg.s9bH, U"中文")
		.onClicked = [this] {
		this->lang.SetLanguage(Languages::cn);
		this->Refresh();	// unsafe
	};

	// close
	MakeChildren<xx::LabelButton>()->Init(z + 3, cp + XY{ 0, -300 + 30 }, { 0.5f, 0 }
		, gg.cfg.s9bN, gg.cfg.s9bH, lang(Strs::close))
		.onClicked = [this] {
		this->onClose(lang.language);	// unsafe
	};

	// bg
	auto bg = MakeChildren<xx::Background>();
	bg->Init(z, c.ToWeak()).onOutsideClicked = [] {};	// swallow
}

