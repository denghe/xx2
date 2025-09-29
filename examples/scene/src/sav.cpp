#include "pch.h"
#include "sav.h"

AJSON(Settings
    , language

    , langSelected
    , mute
    , isFullScreen
    , isBorderless

    , resolutionsIndex

    , masterVolume
    , audioVolume
    , musicVolume

);

int32_t Sav::SavInit(std::filesystem::path const& dir_) {
    return db.Init(dir_);
}

int32_t Sav::Load() {
    if (auto v = db.Load("Settings"); v.size()) {
        ajson::load_from_buff(settings, v.data(), v.size());
    }
    else if (!db.NotFound()) return __LINE__;
    return 0;
}

int32_t Sav::Save() {
    std::stringstream ss;
    ajson::save_to(ss, settings);
    auto s = ss.str();
    return db.Save("Settings", s);
}
