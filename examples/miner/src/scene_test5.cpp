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
		auto& bg = gg.tf.circle256;
		gg.Quad().Draw(bg, bg, p, 0.5f, ts * scene->cam.scale, 0.f, 1.f, c);
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
				gg.Quad().Draw(bg, bg, pp, 0.5f, ts * scene->cam.scale * 0.1f, 0.f, 1.f, c);
			}
		}
	}

	/***************************************************************************************/

	void TalentA::LevelUp() {
		assert(canLevelUp);
		assert(level < maxLevel);
		++level;
		assert(scene->currencyA >= 1);
		scene->currencyA -= 1;
	}

	xx::Shared<xx::Node> TalentA::GetInfo() { 
		return {}; 
	}

	void TalentA::Update() {
		visible = true;
		if (level == maxLevel) {
			canLevelUp = false;
			return;
		}
		assert(level < maxLevel);
		canLevelUp = scene->currencyA >= 1;
	}

	/***************************************************************************************/

	void TalentB::LevelUp() {
		assert(canLevelUp);
		assert(level < maxLevel);
		++level;
		assert(scene->currencyB >= 1);
		scene->currencyB -= 1;
	}

	xx::Shared<xx::Node> TalentB::GetInfo() { 
		return {};
	}

	void TalentB::Update() {
		if (!parent->level) {
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
		canLevelUp = scene->currencyB >= 1;
	}

	/***************************************************************************************/

	void TalentC::LevelUp() {
		assert(canLevelUp);
		assert(level < maxLevel);
		++level;
		assert(scene->currencyC >= 1);
		scene->currencyC -= 1;
	}

	xx::Shared<xx::Node> TalentC::GetInfo() {
		return {};
	}

	void TalentC::Update() {
		if (!parent->level) {
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
		canLevelUp = scene->currencyC >= 1;
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

	void Scene::Init() {
		cam.Init(gg.scale, 1.f, gg.designSize / 2);
		ui.Emplace()->InitRoot(gg.scale);

		// todo: ui for currencyABC

		currencyA = 1;
		currencyB = 2;
		currencyC = 2;

		talentScale = 1.f;
		talentBasePos = gg.designSize / 2;
		static constexpr float d{ 300 };

		// fill talents
		{
			auto& t = talents.Emplace().Emplace<TalentA>();
			t->type = TalentA::cType;
			t->id = 0;
			t->parentId = 0;
			t->maxLevel = 1;
			t->pos = 0;
		}
		{
			auto& t = talents.Emplace().Emplace<TalentB>();
			t->type = TalentB::cType;
			t->id = 1;
			t->parentId = 0;
			t->maxLevel = 2;
			t->pos = -d;
		}
		{
			auto& t = talents.Emplace().Emplace<TalentC>();
			t->type = TalentC::cType;
			t->id = 2;
			t->parentId = 0;
			t->maxLevel = 3;
			t->pos = { d, -d };
		}

		// fill talent's parent, scene
		for (auto& t : talents) {
			t->scene = this;
			for (auto& o : talents) {
				if (t.pointer == o.pointer) continue;
				if (o->parentId == t->id) {
					o->parent = t.pointer;
					//t->children.Add(o.pointer);
				}
			}
		}

		// restore player settings
		//SetTalentLevel(0, 1);

		// fill talent's visible
		// todo
	}

	void Scene::Update() {
		// handle inputs
		if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
			gg.MakeScene<Scene_MainMenu>()->Init();
			return;
		}
		// todo: mouse scroll zoom talentScale. drag, click
		if (gg.mouse[GLFW_MOUSE_BUTTON_1](0.2f)) {
			auto mp = (cam.ToLogicPos(gg.mousePos) - talentBasePos) / talentScale;
			xx::CoutN(mp);
			for (auto& o : talents) {
				if (o->canLevelUp) {
					auto d = o->pos - mp;
					if (d.x * d.x + d.y * d.y < 128.f * 128.f) {
						o->LevelUp();
					}
				}
			}
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
		if (gg.mouse[GLFW_MOUSE_BUTTON_LAST + 1]) {	// mouse wheel up
			if (talentScale < 1.f) {
				talentScale += 0.02f;
			}
		}
		if (gg.mouse[GLFW_MOUSE_BUTTON_LAST + 2]) { // mouse wheel down
			if (talentScale > 0.1f) {
				talentScale -= 0.02f;
			}
		}

		for (auto& o : talents) o->Update();
	}

	void Scene::Draw() {
		for (auto& o : talents) o->Draw();
		gg.GLBlendFunc(gg.blendDefault);
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale);
		cam.SetBaseScale(gg.scale);
	}

}
