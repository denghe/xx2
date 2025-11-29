#include "pch.h"
#include "scene_test5.h"
#include "scene_mainmenu.h"

namespace Test5 {

	void TalentBase::Draw() {
		if (!visible) return;
		auto ts = scene->talentScale;
		auto pos1 = pos * ts;
		auto p = scene->cam.ToGLPos(scene->talentBasePos + pos1);
		auto& fg = gg.tf.talent_[(int32_t)type];
		gg.Quad().Draw(fg, fg, p, 0.5f, scene->talentScale * scene->cam.scale);
		xx::RGBA8 c;
		if (level == maxLevel) {
			c = xx::RGBA8_Blue;
		}
		else if (canLevelUp) {
			c = xx::RGBA8_Green;
		}
		else {
			c = xx::RGBA8_Red;
		}
		float cp;
		if (scene->currTalent == this) cp = 1.f;
		else cp = 0.5f;
		auto& bg = gg.tf.circle256;
		gg.Quad().Draw(bg, bg, p, 0.5f, ts * scene->cam.scale, 0.f, cp, c);
		// draw line
		if (parent) {
			auto pos2 = parent->pos * ts;
			auto d = pos2 - pos1;
			auto dist = std::sqrtf(d.x * d.x + d.y * d.y);
			auto norm = d / dist;
			pos1 += norm * 128.f * ts;
			pos2 -= norm * 128.f * ts;
			dist -= 128.f * ts * 2.f;
			static constexpr int32_t numSteps{ 32 };
			auto step = norm * dist * (1.f / numSteps);
			for (int32_t i = 0; i < numSteps; ++i) {
				auto pp = scene->cam.ToGLPos(scene->talentBasePos + pos1 + step * i);
				gg.Quad().Draw(bg, bg, pp, 0.5f, ts * scene->cam.scale * 0.1f, 0.f, cp, c);
			}
		}
	}

	xx::Shared<xx::Node> TalentBase::GetInfo() {
		assert(level <= maxLevel);
		auto node = xx::MakeShared<xx::Node>();
		xx::Layouter L;
		L.InitBegin(node, 2, {}, { 0.5f, 0 }, 500);
		FillInfo(L);
		L.EndLine();
		if (level == maxLevel) {
			L.Text("MAX", 32, 48);
		}
		else {
			auto& ps = levelPricess[level];
			xx::RGBA8 c;
			for (auto& p : ps) {
				if (scene->currency[(int32_t)p.currencyType] < p.value) c = xx::RGBA8_Red;
				else c = xx::RGBA8_White;
				auto& tf = gg.tf.rocks_[std::to_underlying(p.currencyType)][4];
				L.Image(tf, 48.f).Text(xx::ToString(p.value, " "), 32, 48, c);
			}
		}
		L.EndLine(false).HAlign(xx::HAligns::Right);
		L.Text(xx::ToString(level, "/", maxLevel), 32, 48);
		L.InitEnd();
		node->Make<xx::Scale9>()->Init(1, { -15, -5 }, 0, node->size + XY{ 30, 10 });
		return node;
	}

	void TalentBase::FillInfo(xx::Layouter& L_) {
		assert(false);	// must override
	}

	void TalentBase::LevelUp() {
		assert(canLevelUp);
		assert(level < maxLevel);
		auto& prices = levelPricess[level];
		for (auto& p : prices) {
			auto& c = scene->currency[(int32_t)p.currencyType];
			assert(c >= p.value);
			c -= p.value;
		}
		++level;
	}

	void TalentBase::Update() {
		if (parent && !parent->level) {
			visible = false;
			canLevelUp = false;
			return;
		}
		else {
			visible = true;
		}
		if (level == maxLevel) {
			canLevelUp = false;
			return;
		}
		assert(level < maxLevel);
		auto& prices = levelPricess[level];
		for (auto& p : prices) {
			auto& c = scene->currency[(int32_t)p.currencyType];
			if (c < p.value) {
				canLevelUp = false;
				return;
			}
		}
		canLevelUp = true;
	}

	/***************************************************************************************/

	void TalentA::FillInfo(xx::Layouter& L) {
		L.Text("talent A info", 32, 48);
	}

	/***************************************************************************************/

	void TalentB::FillInfo(xx::Layouter& L) {
		L.Text("talent B info", 32, 48);
	}

	/***************************************************************************************/

	void TalentC::FillInfo(xx::Layouter& L) {
		L.Text("talent C info", 32, 48);
	}

	/***************************************************************************************/

	void Scene::SetTalentLevel(int32_t id_, int32_t level_) {
		for (auto& o : talents) {
			if (o->id == id_) {
				assert(o->maxLevel >= level_);
				o->level = level_;
				return;
			}
		}
		assert(false);
	}

	TalentBase* Scene::FindTalent(XY pos_) {
#if 0
		for (auto& o : talents) {
			if (!o->visible) continue;
			auto d = o->pos - pos_;
			if (d.x * d.x + d.y * d.y < 128.f * 128.f) {
				return o.pointer;
			}
		}
		return {};
#else
		TalentBase* r{};
		auto cri = talentsGrid.PosToCRIndex(pos_);
		talentsGrid.ForeachBy9(cri.y, cri.x, [&](decltype(talentsGrid)::Node& node, float range)->bool {
			if (auto p = node.value; p->visible) {
				auto d = p->pos - pos_;
				if (d.x * d.x + d.y * d.y < 128.f * 128.f) {
					r = p;
					return true;
				}
			}
			return false;
		});
		return r;
#endif
	}

	void Scene::Init() {
		cam.Init(gg.scale, 1.f, gg.designSize / 2);
		ui.Emplace()->InitRoot(gg.scale);
		{
			XY uiPos = gg.p7 + XY{ 20, -20 - 32.f };
			XY a{ 0.f, 0.5f };
			for (int32_t i = 0; i < cNumCurrencyTypes; ++i) {
				auto p = uiPos;
				p.x += i * 250.f;
				ui->Make<xx::Image>()->Init(1, p, a, gg.tf.rocks_[i][4], 64.f);
				p.x += 64.f + 5.f;
				auto lbl = ui->Make<xx::Label>();
				currencyUI[i] = lbl;
				lbl->Init(1, p, a, 48)("0");
			}
		}

		currency[(int32_t)CurrencyTypes::A] = 100;
		currency[(int32_t)CurrencyTypes::B] = 100;
		currency[(int32_t)CurrencyTypes::C] = 100;

		talentScale = 1.f;
		talentBasePos = {};
		static constexpr float d{ 300 };
		auto basePos = gg.designSize / 2;

		// fill talents
		{
			auto& t = talents.Emplace().Emplace<TalentA>();
			t->type = TalentA::cType;
			t->id = 0;
			t->parentId = 0;
			t->maxLevel = 1;
			t->pos = basePos;
			t->levelPricess.Resize(t->maxLevel);
			t->levelPricess[0].Add(TalentPrice{ CurrencyTypes::A, 50 });
		}
		{
			auto& t = talents.Emplace().Emplace<TalentB>();
			t->type = TalentB::cType;
			t->id = 1;
			t->parentId = 0;
			t->maxLevel = 2;
			t->pos = basePos - d;
			t->levelPricess.Resize(t->maxLevel);
			t->levelPricess[0].Adds({ { CurrencyTypes::A, 50 }, { CurrencyTypes::B, 50 } });
			t->levelPricess[1].Adds({ { CurrencyTypes::A, 100 }, { CurrencyTypes::B, 100 } });
		}
		{
			auto& t = talents.Emplace().Emplace<TalentC>();
			t->type = TalentC::cType;
			t->id = 2;
			t->parentId = 0;
			t->maxLevel = 3;
			t->pos = basePos + XY{ d, -d };
			t->levelPricess.Resize(t->maxLevel);
			t->levelPricess[0].Adds({ { CurrencyTypes::B, 50 }, { CurrencyTypes::C, 50 } });
			t->levelPricess[1].Adds({ { CurrencyTypes::B, 100 }, { CurrencyTypes::C, 100 } });
			t->levelPricess[2].Adds({ { CurrencyTypes::B, 200 }, { CurrencyTypes::C, 200 } });
		}

		// init talents grid
		auto numRows = (int32_t)std::ceilf(gg.designSize.y / 256.f);
		auto numCols = (int32_t)std::ceilf(gg.designSize.x / 256.f);
		talentsGrid.Init(256, numRows, numCols);
		talentsGrid.Reserve(talents.len);

		// fill talent's parent, scene, grid
		for (auto& t : talents) {
			assert(t->maxLevel > 0);
			assert(t->level <= t->maxLevel);
			assert(t->levelPricess.len == t->maxLevel);
			t->scene = this;
			talentsGrid.Add(t.pointer);

			for (auto& o : talents) {
				if (t.pointer == o.pointer) continue;
				if (o->parentId == t->id) {
					o->parent = t.pointer;
				}
			}
		}

		// restore player settings
		//SetTalentLevel(0, 1);
	}

	void Scene::Update() {
		// handle inputs
		if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
			gg.MakeScene<Scene_MainMenu>()->Init();
			return;
		}

		// handle mouse drag, click
		auto mp = (cam.ToLogicPos(gg.mousePos) - talentBasePos) / talentScale;
		currTalent = FindTalent(mp);
		if (gg.mouse[GLFW_MOUSE_BUTTON_1]) {
			if (clicking) {
				if (lastMousePos != gg.mousePos) {
					clicking = false;
					talentDragging = true;
				}
			}
			if (talentDragging) {
				talentBasePos += (gg.mousePos - lastMousePos).FlipY();
				lastMousePos = gg.mousePos;
			}
			else if (!lastMBPressed) {	// first time
				lastMBPressed = true;
				if (currTalent) clicking = true;
				else talentDragging = true;
				lastMousePos = gg.mousePos;
			}
		}
		else {
			if (clicking && lastMousePos == gg.mousePos) {
				assert(currTalent);
				if (currTalent->canLevelUp) {
					currTalent->LevelUp();
					gg.PlayAudio(gg.embed.ss_ui_focus);
				}
			}
			clicking = false;
			talentDragging = false;
			lastMBPressed = false;
		}
		bool uiInfoExists{ false };
		if (uiInfo) {
			uiInfo->SwapRemove();
			uiInfoExists = true;
		}
		if (currTalent) {
			if (!uiInfoExists) {
				gg.PlayAudio(gg.embed.ss_ui_focus);
			}
			auto n = currTalent->GetInfo();
			n->position = cam.ToGLPos(talentBasePos + currTalent->pos * talentScale) + XY{ 0, 128.f * talentScale };
			n->FillTransRecursive();
			ui->Add(n);
			uiInfo = n;
		}

		// fixed update
		auto d = float(std::min((float)gg.delta, gg.cMaxDelta) * timeScale);
		timePool += d;
		while (timePool >= gg.cDelta) {
			time += gg.cDelta;
			timePool -= gg.cDelta;
			FixedUpdate();
		}
	}

	void Scene::FixedUpdate() {

		static constexpr xx::FromTo<float> cScaleRange{ 0.2f, 1.f };
		static constexpr float cScaleStep{ 0.035f };
		if (gg.mouse[GLFW_MOUSE_BUTTON_LAST + 1]) {	// mouse wheel up
			if (talentScale < cScaleRange.to) {
				auto mp = (cam.ToLogicPos(gg.mousePos) - talentBasePos) / talentScale;
				auto mp2 = (cam.ToLogicPos(gg.mousePos) - talentBasePos) / (talentScale + cScaleStep);
				talentScale += cScaleStep;
				talentBasePos += (mp2 - mp) * talentScale;
			}
		}
		if (gg.mouse[GLFW_MOUSE_BUTTON_LAST + 2]) { // mouse wheel down
			if (talentScale > cScaleRange.from) {
				auto mp = (cam.ToLogicPos(gg.mousePos) - talentBasePos) / talentScale;
				auto mp2 = (cam.ToLogicPos(gg.mousePos) - talentBasePos) / (talentScale - cScaleStep);
				talentScale -= cScaleStep;
				talentBasePos += (mp2 - mp) * talentScale;
			}
		}

		for (auto& o : talents) o->Update();
	}

	void Scene::Draw() {
		// todo: scissor?
		for (auto& o : talents) o->Draw();

		// sync currency ui text
		for (int32_t i = 0; i < cNumCurrencyTypes; ++i) {
			currencyUI[i]->SetText(xx::ToString(currency[i]));
		}

		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale);
		cam.SetBaseScale(gg.scale);
	}

}
