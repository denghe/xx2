#include "pch.h"
#include "ui_equipbag.h"
#include "player.h"
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

[auto equip]            [sort]
        ( bag area )
    [] [] [] [] [] [] []
    [] [] [] [] [] [] []
    [] [] [] [] [] [] []
    [] [] [] [] [] [] []
    [] [] [] [] [] [] []

        [trash can]?

+-----------------------------+
*/

	void EquipBag::Init(int32_t z_, xx::Weak<Player> owner_) {
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

        // swallow
        auto& swallow = Make<xx::Button>()->Init(z, 0, 0, size);
        swallow.onClicked = [] {};
        swallow.onFocus = [] {};

        // helm
        XY offset{ totalSize.x / 2, totalSize.y - cMargin - cCellSize.y / 2 };
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize
            , EquipLocations::Helm, &owner->equips[(int32_t)EquipLocations::Helm]);

        // ring
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize
            , EquipLocations::Ring, &owner->equips[(int32_t)EquipLocations::Ring]);

        // amulet
        offset.x += cCellSize.x * 2;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize
            , EquipLocations::Amulet, &owner->equips[(int32_t)EquipLocations::Amulet]);

        // armor
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize
            , EquipLocations::Armor, &owner->equips[(int32_t)EquipLocations::Armor]);

        // weapon1
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize
            , EquipLocations::Weapon1, &owner->equips[(int32_t)EquipLocations::Weapon1]);

        // weapon2
        offset.x += cCellSize.x * 2;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize
            , EquipLocations::Weapon2, &owner->equips[(int32_t)EquipLocations::Weapon2]);

        // boots
        offset -= cCellSize;
        Make<EquipBagCell>()->Init(z + 2, offset, 0.5f, cCellSize
            , EquipLocations::Boots, &owner->equips[(int32_t)EquipLocations::Boots]);

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
                Make<EquipBagCell>()->Init(z + 2, pos, {0,1}, cCellSize
                    , EquipLocations::__MAX__, &bagCell);
            }
        }
	}

    int32_t EquipBag::Update() {
        // sync dragging pos
        if (draggingItem) {
            if (auto& equip = *draggingItem->cell->equipPtr; equip) {
                draggingItem->position = draggingItem->ToParentLocalPos(gg.mousePos);
                draggingItem->FillTransRecursive();
            }
            else {
                draggingItem->SwapRemove();
            }
        }

        // handle tips panel
        if (gg.uiHandler && gg.uiHandler->typeId == EquipBagCell::cTypeId) {
            if (focusedCell != gg.uiHandler) {
                if (infoPanel) {
                    infoPanel->SwapRemove();
                }
                focusedCell = gg.uiHandler;
                infoPanelPopupTime = gg.time + 0.2f;
            }
            else if (gg.time >= infoPanelPopupTime) {
                if (!infoPanel) {
                    if (auto& equip = *gg.uiHandler.CastRef<EquipBagCell>().equipPtr; equip) {
                        infoPanel = equip->GenInfoPanel();
                    }
                }
                if (infoPanel) {
                    auto mp = infoPanel->ToParentLocalPos(gg.mousePos);
                    if (mp.y - infoPanel->size.y < gg.designSize.y * -0.5f) {   // avoid panel out of screen
                        mp.y = gg.designSize.y * -0.5f + infoPanel->size.y;
                    }
                    if (infoPanel->position != mp) {
                        infoPanel->position = mp;
                        infoPanel->FillTransRecursive();
                    }
                }
            }
        }
        else {
            focusedCell.Reset();
            infoPanelPopupTime = 0;
            if (infoPanel) {
                infoPanel->SwapRemove();
            }
        }
        return 0;
    }


    
    EquipBagCell& EquipBagCell::Init(int32_t z_, XY position_, XY anchor_, XY size_
        , EquipLocations equipLocation_, xx::Shared<Equip>* equipPtr_) {
        assert(typeId == cTypeId);
        Button::Init(z_, position_, anchor_, size_, gg.embed.cfg_s9bN, gg.embed.cfg_s9bH);
        assert(equipPtr_);
        equipLocation = equipLocation_;
        equipPtr = equipPtr_;
        static constexpr float cMargin{ 5 };
        assert(size.x > cMargin * 2 && size.y > cMargin * 2);
        Make<CellItem>()->Init(z_ + 1, size * 0.5f, 0.5, size - cMargin * 2);

        onClicked = [this] {
            auto& eb = parent.CastRef<EquipBag>();
            if (eb.draggingItem) {
                auto& cell = eb.draggingItem->cell;
                if (cell.TryGetPointer() == this) {
                    // do nothing (cancel select)
                }
                else if (*equipPtr) {
                    // swap
                    if (equipLocation == EquipLocations::__MAX__) {
                        // bag : ?
                        if (cell->equipLocation == EquipLocations::__MAX__) {
                            // bag : bag
                            std::swap(*cell->equipPtr, *equipPtr);
                            eb.owner->SaveData();
                        }
                        else {
                            // bag : equips
                            if (cell->equipLocation == (*equipPtr)->cfg->location) {
                                // location auth success
                                std::swap(*cell->equipPtr, *equipPtr);
                                eb.owner->SaveData();
                                eb.owner->CalcProps();
                            }
                            // else { // do nothing (cancel select)
                        }
                    }
                    else {
                        // equips : ?
                        if (cell->equipLocation == EquipLocations::__MAX__) {
                            // equips : bag
                            if (equipLocation == (*cell->equipPtr)->cfg->location) {
                                // location auth success
                                std::swap(*cell->equipPtr, *equipPtr);
                                eb.owner->SaveData();
                                eb.owner->CalcProps();
                            }
                            // else { // do nothing (cancel select)
                        }
                        // else { // equips : equips // do nothing (cancel select)
                    }
                }
                else {
                    // move
                    if (equipLocation == EquipLocations::__MAX__) {
                        // bag : ?
                        std::swap(*cell->equipPtr, *equipPtr);
                        eb.owner->SaveData();
                    }
                    else {
                        // equips : ?
                        if (cell->equipLocation == EquipLocations::__MAX__) {
                            // equips : bag
                            if (equipLocation == (*cell->equipPtr)->cfg->location) {
                                // location auth success
                                std::swap(*cell->equipPtr, *equipPtr);
                                eb.owner->SaveData();
                                eb.owner->CalcProps();
                            }
                            // else { // do nothing (cancel select)
                        }
                        // else { // equips : equips // do nothing (cancel select)
                    }
                }
                eb.draggingItem->SwapRemove();
            } else if (*equipPtr) {
                // select
                eb.draggingItem = eb.parent->Make<DraggingItem>();
                eb.draggingItem->Init(xx::WeakFromThis(this), z + 10, gg.mousePos, 0.5, size);
            }
            // else { // do nothing
        };

        onClicked2 = [this] {
            auto& eb = parent.CastRef<EquipBag>();
            if (eb.draggingItem) {
                eb.draggingItem->SwapRemove();
            }
            else if (auto& equip = *equipPtr; equip) {
                if ((int32_t)equipLocation <= (int32_t)EquipLocations::__EQUIPED_MAX__) {
                    // unequip
                    if (auto c = eb.owner->FindFirstEmptyBagCell(); c) {
                        std::swap(*c, *equipPtr);
                        eb.owner->SaveData();
                        eb.owner->CalcProps();
                    }
                }
                else {
                    if ((int32_t)equip->cfg->location <= (int32_t)EquipLocations::__EQUIPED_MAX__) {
                        // equip
                        std::swap(eb.owner->equips[(int32_t)equip->cfg->location], equip);
                        eb.owner->SaveData();
                        eb.owner->CalcProps();
                    }
                }
            }
        };
        return *this;
    }



    CellItem& CellItem::Init(int32_t z_, XY position_, XY anchor_, XY size_) {
        InitDerived<CellItem>(z_, position_, anchor_, 1, size_);
        assert(parent->typeId == EquipBagCell::cTypeId);
        return *this;
    }

    void CellItem::Draw() {
        assert(parent->typeId == EquipBagCell::cTypeId);
        auto& ebc = parent.CastRef<EquipBagCell>();
        if (auto& equip = *ebc.equipPtr; equip) {
            auto& eb = ebc.parent.CastRef<EquipBag>();
            float colorplus;
            if (eb.draggingItem && eb.draggingItem->cell.pointer() == &ebc) colorplus = 0.5f;
            else colorplus = 1.f;
            gg.Quad().Draw(gg.res.cell_bg1, gg.res.cell_bg1, worldMinXY, {}
                , worldSize / gg.res.cell_bg1.Size(), 0, colorplus
                , EquipQualityColors[(int32_t)equip->cfg->quality]);
            // todo: shadow effect
            equip->Draw(worldMinXY, {}, worldSize, colorplus);
        }
    }



    DraggingItem& DraggingItem::Init(xx::Weak<EquipBagCell> cell_, int32_t z_, XY position_, XY anchor_, XY size_) {
        InitDerived<DraggingItem>(z_ + 1, position_, anchor_, 1, size_);
        assert(parent->typeId == 0);
        cell = std::move(cell_);
        assert(cell->equipPtr);
        auto& equip = *cell->equipPtr;
        if (equip->cfg->location <= EquipLocations::__EQUIPED_MAX__) {
            // search target cell & make border
            for (auto& c : cell->parent->children) {
                if (c->typeId == EquipBagCell::cTypeId) {
                    auto& ebc = c.Cast<EquipBagCell>();
                    assert(ebc->equipLocation != EquipLocations::__MAX__);
                    if (ebc->equipLocation == equip->cfg->location) {
                        border = ebc->parent->Make<xx::Scale9>();
                        border->Init(z_, ebc->position, ebc->anchor, ebc->size, gg.embed.cfg_s9);
                        break;
                    }
                }
            }
        }

        return *this;
    }

    void DraggingItem::Draw() {
        assert(cell);
        auto& equip = *cell->equipPtr;
        equip->Draw(worldMinXY, {}, worldSize, 1);
    }

    DraggingItem::~DraggingItem() {
        if (border) border->SwapRemove();
    }

}
