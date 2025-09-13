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

	void PropsLog::Init(int32_t z_) {
		children.Clear();	// unsafe

        auto totalSize = XY{ gg.designSize.x / 4, gg.designSize.y };
        static constexpr float cMargin{ 15 };

        InitDerived<PropsLog>(z_, gg.p3, gg.a3, 1, totalSize);



        xx::Layouter L;

        // props border
        Make<xx::Scale9>()->Init(z + 1, { 0, size.y * 0.5f }, 0, { size.x, size.y * 0.5f });

        auto props = Make<xx::Node>();
        //L.InitBegin(props, z + 2, { 0,  })
        // todo




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
