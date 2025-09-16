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

        static constexpr float cMargin{ 15 };
        static constexpr XY cCellSize{ 64, 64 };
        static constexpr float cCellMargin{ 5 };
        XY totalSize{ gg.designSize.x / 4, gg.designSize.y };

        InitDerived<EquipBag>(z_, gg.p1, gg.a1, 1, totalSize);
        Make<xx::Scale9>()->Init(z + 1, 0, 0, size);

        //// swallow
        //Make<xx::Button>()->Init(z, 0, 0, size).onClicked = [] {
        //    xx::CoutN("EquipBag clicked");
        //};

        // helm
        XY offset{ totalSize.x / 2, totalSize.y - cMargin - cCellSize.y / 2 };
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize);

        // ring
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize);

        // amulet
        offset.x += cCellSize.x * 2;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize);

        // armor
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize);

        // weapon1
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize);

        // weapon2
        offset.x += cCellSize.x * 2;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize);

        // boots
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize);

        offset.y -= cCellSize.y / 2 + cMargin;
        XY cellsSize{ totalSize.x - cMargin * 2, offset.y - cMargin };
        //Make<EquipBagCell>()->Init(z + 2, { cMargin, offset.y }, { 0, 1 }, cellsSize);

        auto w = int32_t(cellsSize.x + cCellMargin) / int32_t(cCellSize.x + cCellMargin);
        auto h = int32_t(cellsSize.y + cCellMargin) / int32_t(cCellSize.y + cCellMargin);
        auto cCellMarginH = (cellsSize.x - cCellSize.x * w) / (w - 1);
        auto cCellMarginV = (cellsSize.y - cCellSize.y * h) / (h - 1);
        XY basePos{ cMargin }, pos{};
        for (int32_t y = 0; y < h; ++y) {
            for (int32_t x = 0; x < w; ++x) {
                pos.x = basePos.x + x * (cCellSize.x + cCellMarginH);
                pos.y = basePos.y + cellsSize.y - y * (cCellSize.y + cCellMarginV);
                Make<EquipBagCell>()->Init(z + 2, pos, {0,1}, cCellSize);
            }
        }
	}
}
