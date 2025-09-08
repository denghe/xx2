#include "pch.h"
#include "ui_equipbag.h"

namespace UI {

    /*
size: 480, 1080
+-----------------------------+

           head
        ring amulet
           armor
        hand1 hand2
           foot


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
        Make<xx::Scale9>()->Init(z, 0, 0, size);

        // swallow
        Make<xx::Button>()->Init(z, 0, 0, size).onClicked = [] {
            xx::CoutN("EquipBag clicked");
        };

	}
}
