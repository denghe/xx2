#pragma once
#include "pch.h"
#include <xx_lmdb.h>
#include <ajson.hpp>

// player data store file
struct Settings {
	static constexpr xx::XY cResolutions[5] = {
	{ 1280,720 }
	, { 1366,768 }
	, { 1920,1080 }
	, { 2560,1440 }
	, { 3840,2160 }
	};

	int32_t language{};		// i18n::Languages

	bool langSelected{}
		, mute{}
		, isFullScreen{}
		, isBorderless{};

	int32_t resolutionsIndex{ 2 };

	float masterVolume{ 1.f }
		, audioVolume{ 1.f }
		, musicVolume{ 0.5f };
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
