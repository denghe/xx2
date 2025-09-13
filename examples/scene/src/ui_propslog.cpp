#include "pch.h"
#include "ui_propslog.h"

namespace UI {

    /*
size: 480, 1080
+-----------------------------+
prop name: value
prop name: value
prop name: value
prop name: value
prop name: value
prop name: value
+-----------------------------+
logs
logs
logs
logs
logs
logs
+-----------------------------+
*/

	void PropsLog::Init(int32_t z_) {
		children.Clear();	// unsafe
        InitDerived<PropsLog>(z_, gg.p3, gg.a3, 1, { gg.designSize.x / 4, gg.designSize.y });

        // border
        Make<xx::Scale9>()->Init(z + 1, 0, 0, size);

        // swallow
        Make<xx::Button>()->Init(z, 0, 0, size).onClicked = [] {
            xx::CoutN("PropsLog clicked");
        };

        // todo



        auto rl = Make<xx::Node>();
        xx::Layouter{}.InitBegin(rl, z + 2, { 30, 200 }, 0, 200)
            .HAlign(xx::HAligns::Center).Text(U"标题\n", 64, 100, xx::RGBA8_Green)
            .HAlign().Image(gg.res.player)
            .Text(U" 正文正文正文正文正文正文正文正文正文.\n", 32, 48, xx::RGBA8_White)
            .InitEnd();
        static constexpr float cMargin{ 15 };
        auto& s9 = Make<xx::Scale9>()->Init(z + 1, rl->position - cMargin
            , rl->anchor, rl->size + cMargin * 2);
	}
}
