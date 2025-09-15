#include "pch.h"
#include "ui_propslog.h"

namespace UI {

    /*
size: 480, 540
+-----------------------------+
prop name                value
prop name                value
prop name                value
prop name                value
prop name                value
prop name                value
+-----------------------------+

size: 480, 540
+-----------------------------+


player picked up xxxxxx !
player picked up 5 coins!
player level up to 2!
+-----------------------------+
*/

	void PropsLog::Init(int32_t z_, xx::Weak<Creature> creature_) {
        creature = std::move(creature_);
		children.Clear();	// unsafe

        static constexpr float cMargin{ 15 };
        XY totalSize{ gg.designSize.x / 4, gg.designSize.y };
        auto contentSize = totalSize - cMargin * 2;
        XY propsSize{ contentSize.x, contentSize.y * 0.5f };
        XY propsPos{ cMargin, cMargin + contentSize.y };
        // todo: logs

        xx::Layouter L;
        InitDerived<PropsLog>(z_, gg.p3, gg.a3, 1, totalSize);

        // props border
        Make<xx::Scale9>()->Init(z + 1, { 0, size.y * 0.5f }, 0, { size.x, size.y * 0.5f });

        // todo
        auto props = Make<xx::Node>();
        L.InitBegin(props, z + 2, propsPos, { 0, 1 }, contentSize.x);
        L.Text(gg.lang(Strs::healthMax), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->healthMax), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::healthRegeneration), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->healthRegeneration), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::manaMax), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->manaMax), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::manaRegeneration), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->manaRegeneration), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::defense), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->defense), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::dodge), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->dodge), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::damage), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->damage), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::criticalChance), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->criticalChance), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::criticalDamage), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->criticalDamage), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::movementSpeed), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->movementSpeed), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::lucky), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->lucky), 32, 40).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::harvestFactor), 32, 40).EndLine(false);
        L.Text(xx::ToString(creature->harvestFactor), 32, 40).EndLine(xx::HAligns::Right);
        L.InitEnd();


        // logs
        Make<xx::Scale9>()->Init(z + 1, 0, 0, { size.x, size.y * 0.5f });

        // todo

        //auto rl = Make<xx::Node>();
        //xx::Layouter{}.InitBegin(rl, z + 2, { 30, 200 }, 0, 200)
        //    .HAlign(xx::HAligns::Center).Text(U"标题\n", 64, 100, xx::RGBA8_Green)
        //    .HAlign().Image(gg.res.player)
        //    .Text(U" 正文正文正文正文正文正文正文正文正文.\n", 32, 48, xx::RGBA8_White)
        //    .InitEnd();
        //static constexpr float cMargin{ 15 };
        //auto& s9 = Make<xx::Scale9>()->Init(z + 1, rl->position - cMargin
        //    , rl->anchor, rl->size + cMargin * 2);
	}
}
