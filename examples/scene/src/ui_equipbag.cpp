#include "pch.h"
#include "ui_equipbag.h"
#include "creature.h"
#include "scene_play.h"

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

	void EquipBag::Init(int32_t z_, xx::Weak<Creature> owner_) {
        assert(owner_);
        owner = std::move(owner_);
        TryRegisterAutoUpdate();

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
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize)
            .FillContent(EquipLocations::Helm, &owner->equips[(int32_t)EquipLocations::Helm]);

        // ring
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize)
            .FillContent(EquipLocations::Ring, &owner->equips[(int32_t)EquipLocations::Ring]);

        // amulet
        offset.x += cCellSize.x * 2;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize)
            .FillContent(EquipLocations::Amulet, &owner->equips[(int32_t)EquipLocations::Amulet]);

        // armor
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize)
            .FillContent(EquipLocations::Armor, &owner->equips[(int32_t)EquipLocations::Armor]);

        // weapon1
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize)
            .FillContent(EquipLocations::Weapon1, &owner->equips[(int32_t)EquipLocations::Weapon1]);

        // weapon2
        offset.x += cCellSize.x * 2;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize)
            .FillContent(EquipLocations::Weapon2, &owner->equips[(int32_t)EquipLocations::Weapon2]);

        // boots
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize)
            .FillContent(EquipLocations::Boots, &owner->equips[(int32_t)EquipLocations::Boots]);

        offset.y -= cCellSize.y / 2 + cMargin;
        XY cellsSize{ totalSize.x - cMargin * 2, offset.y - cMargin };
        //Make<EquipBagCell>()->Init(z + 2, { cMargin, offset.y }, { 0, 1 }, cellsSize);

        auto w = int32_t(cellsSize.x + cCellMargin) / int32_t(cCellSize.x + cCellMargin);
        auto h = int32_t(cellsSize.y + cCellMargin) / int32_t(cCellSize.y + cCellMargin);
        assert(w * h == owner->bag.len);

        auto cCellMarginH = (cellsSize.x - cCellSize.x * w) / (w - 1);
        auto cCellMarginV = (cellsSize.y - cCellSize.y * h) / (h - 1);

        XY basePos{ cMargin }, pos{};
        for (int32_t y = 0; y < h; ++y) {
            for (int32_t x = 0; x < w; ++x) {
                pos.x = basePos.x + x * (cCellSize.x + cCellMarginH);
                pos.y = basePos.y + cellsSize.y - y * (cCellSize.y + cCellMarginV);
                auto& bagCell = owner->bag[y * w + x];
                Make<EquipBagCell>()->Init(z + 2, pos, {0,1}, cCellSize)
                    .FillContent(EquipLocations::__MAX__, &bagCell);
            }
        }
	}

    int32_t EquipBag::Update() {
        // todo: auto sync memory?
        // todo: drag & drop

        if (draggingItem) {
            draggingItem->position = gg.mousePos;
            draggingItem->FillTransRecursive();
        }

        // check mouse pos & popup tips panel
        if (gg.uiHandler && gg.uiHandler->typeId == EquipBagCell::cTypeId) {
            if (focusedCell != gg.uiHandler) {
                if (infoPanel) {
                    infoPanel->SwapRemove();
                    xx::CoutN("SwapRemove infoPanel");
                }
                focusedCell = gg.uiHandler;
                infoPanelPopupTime = gg.time + 0.2f;
            }
            else if (gg.time >= infoPanelPopupTime) {
                if (!infoPanel) {
                    auto& equip = *gg.uiHandler.CastRef<EquipBagCell>().equipPtr;
                    if (equip) {
                        infoPanel = equip->GenInfoPanel();
                        //xx::CoutN("pop infoPanel ", infoPanel->worldMinXY);
                    }
                }
                else if (infoPanel->position != gg.mousePos) {
                    // sync infoPanel pos with mouse
                    infoPanel->position = gg.mousePos;
                    infoPanel->FillTransRecursive();
                    //xx::CoutN("move infoPanel ", infoPanel->worldMinXY);
                }
            }
        }
        else {
            focusedCell.Reset();
            infoPanelPopupTime = 0;
            if (infoPanel) {
                infoPanel->SwapRemove();
                xx::CoutN("SwapRemove infoPanel");
            }
        }
        return 0;
    }


    
    EquipBagCell& EquipBagCell::Init(int32_t z_, XY position_, XY anchor_, XY size_) {
        assert(typeId == cTypeId);
        z = z_;
        position = position_;
        anchor = anchor_;
        size = size_;
        cfgNormal = gg.embed.cfg_s9bN;
        cfgHighlight = gg.embed.cfg_s9bH;
        FillTrans();
        auto& cfg = GetCfg();
        Make<xx::Scale9>()->Init(z + 1, 0, 0, size, cfg);

        onClicked = [this] {
            auto& eb = parent.CastRef<EquipBag>();
            if (eb.draggingItem) {
                auto& cell = eb.draggingItem->cell;
                if (cell.TryGetPointer() == this) {
                    // cancel select
                }
                else if (*equipPtr) {   // swap
                    // todo: equip location auth
                    assert(equipItem);
                    assert(cell->equipItem);
                    assert(*cell->equipPtr);
                    assert(*cell->equipPtr != *equipPtr);
                    std::swap(*cell->equipPtr, *equipPtr);
                    equipItem->SwapRemove();
                    cell->equipItem->SwapRemove();
                    FillContent(equipLocation, equipPtr);
                    cell->FillContent(cell->equipLocation, cell->equipPtr);
                }
                else {  // move
                    // todo: equip location auth
                    assert(!equipItem);
                    assert(cell->equipItem);
                    assert(*cell->equipPtr);
                    std::swap(*cell->equipPtr, *equipPtr);
                    cell->equipItem->SwapRemove();
                    FillContent(equipLocation, equipPtr);
                }
                eb.draggingItem->SwapRemove();
            } else if (equipItem) {
                // select
                eb.draggingItem = eb.parent->Make<DraggingItem>();
                eb.draggingItem->Init(xx::WeakFromThis(this), z + 10, gg.mousePos, 0.5, size);
            }
            else {
                // do nothing
            }
        };
        return *this;
    }

    void EquipBagCell::FillContent(EquipLocations equipLocation_, xx::Shared<Equip>* equipPtr_) {
        assert(equipPtr_);
        equipLocation = equipLocation_;
        equipPtr = equipPtr_;
        if (*equipPtr) {
            static constexpr float cMargin{ 5 };
            assert(size.x > cMargin * 2 && size.y > cMargin * 2);
            equipItem = Make<EquipItem>();
            equipItem->Init(z + 1, size * 0.5f, 0.5, size - cMargin * 2);
        }
        else if (equipItem) {
            equipItem->SwapRemove();
        }
    }



    EquipItem& EquipItem::Init(int32_t z_, XY position_, XY anchor_, XY size_) {
        InitDerived<EquipItem>(z_, position_, anchor_, 1, size_);
        assert(parent->typeId == EquipBagCell::cTypeId);
        return *this;
    }

    void EquipItem::Draw() {
        assert(parent);
        assert(parent->typeId == EquipBagCell::cTypeId);
        auto& equip = *parent.CastRef<EquipBagCell>().equipPtr;
        equip->Draw(worldMinXY, {}, worldSize);
    }



    DraggingItem& DraggingItem::Init(xx::Weak<EquipBagCell> cell_, int32_t z_, XY position_, XY anchor_, XY size_) {
        InitDerived<DraggingItem>(z_, position_, anchor_, 1, size_);
        assert(parent->typeId == 0);
        cell = std::move(cell_);
        return *this;
    }

    void DraggingItem::Draw() {
        assert(cell);
        auto& equip = *cell->equipPtr;
        equip->Draw(worldMinXY, {}, worldSize);
    }

}
