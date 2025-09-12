#include "pch.h"
#include "ui_equipbag.h"

namespace UI {

    /*
size: 480, 1080
+-----------------------------+

           helm
        ring amulet
           armor
      weapon1 weapon2
           boots

        ( bag area )
    [] [] [] [] [] [] []
    [] [] [] [] [] [] []
    [] [] [] [] [] [] []
    [] [] [] [] [] [] []
    [] [] [] [] [] [] []

        [trash can]

+-----------------------------+
*/

	void EquipBag::Init(int32_t z_) {
		children.Clear();	// unsafe
        InitDerived<EquipBag>(z_, gg.p1, gg.a1, 1, { gg.designSize.x / 4, gg.designSize.y });

        // border
        Make<xx::Scale9>()->Init(z + 1, 0, 0, size);

        // swallow
        Make<xx::Button>()->Init(z, 0, 0, size).onClicked = [] {
            xx::CoutN("EquipBag clicked");
        };

        // todo


        auto& rl = Make<xx::RichLabel>()->InitBegin(z + 2, { 30, 200 }, 0, 200);
        rl.HAlign(xx::HAligns::Center).Text(U"标题\n", 64, 100, xx::RGBA8_Green)
        .HAlign().Text(U"    正文正文正文正文正文正文正文正文正文.\n", 32, 48, xx::RGBA8_White)
        .InitEnd();

        static constexpr float cMargin{ 15 };
        auto& s9 = Make<xx::Scale9>()->Init(z + 1, rl.position - cMargin, rl.anchor, rl.size + cMargin * 2);
	}
}
