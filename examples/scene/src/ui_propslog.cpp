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
...
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
        InitDerived<PropsLog>(z_, gg.p3, gg.a3, 1, totalSize);
        xx::Layouter L;

        // props
        auto props = Make<xx::Node>();

        // props items
        static constexpr float fontSize{ 32 }, lineHeight{ 48 };
        L.InitBegin(props, z + 2, { cMargin, cMargin + contentSize.y }, { 0, 1 }, contentSize.x);
        L.Text(gg.lang(Strs::healthMax), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->healthMax), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::healthRegeneration), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->healthRegeneration), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::manaMax), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->manaMax), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::manaRegeneration), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->manaRegeneration), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::defense), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->defense), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::dodge), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->dodge), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::damage), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->damage), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::criticalChance), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->criticalChance), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::criticalDamage), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->criticalDamage), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::movementSpeed), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->movementSpeed), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::lucky), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->lucky), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.Text(gg.lang(Strs::harvestFactor), fontSize, lineHeight).EndLine(false);
        L.Text(xx::ToString(creature->harvestFactor), fontSize, lineHeight).EndLine(xx::HAligns::Right);
        L.InitEnd();

        // props border
        Make<xx::Scale9>()->Init(z + 1, props->position + XY{ -cMargin, cMargin }, props->anchor, props->size + cMargin * 2);


        // todo: logs

        // logs
        //XY logsSize{ contentSize.x, contentSize.y -  };
        //Make<xx::Scale9>()->Init(z + 1, 0, 0, { size.x, size.y * 0.5f });

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
