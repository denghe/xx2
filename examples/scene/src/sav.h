#pragma once
#include "pch.h"
#include <xx_lmdb.h>
#include <ajson.hpp>

// player data store file
struct Settings {
	int32_t language{};		// i18n::Languages
	bool langSelected{};
	bool mute{};
	float audioVolume{ 1.f }
	, musicVolume{ 0.5f };
	int32_t resolutionsIndex{ 2 };
};

// ...

struct Sav {
	xx::LMDB db;		// for file store
	Settings settings;

	// return 0: success
	int32_t SavInit(std::filesystem::path const& dir_);
	int32_t Load();
	int32_t Save();
};
