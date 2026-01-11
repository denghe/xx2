#include <pch.h>

/*
* template
* PlistFileName.h

#pragma once
#include "pch.h"
struct PlistFileName {
	xx::Frame xxx1;
	xx::Frame xxx2;
	xx::Frame xxx3;
	...
	std::array<xx::Frame, ???> xxxA_;
	std::array<xx::Frame, ???> xxxB_;
	std::array<xx::Frame, ???> xxxC_;
	...

	void Load(std::string picFN_, bool generateMipmap_ = true);
};

///////////////////////////////////////////////////////

* PlistFileName.cpp

include "pch.h"
#include "PlistFileName.h"

void PlistFileName::Load(std::string picFN_, bool generateMipmap_) {
	auto t = LoadTexture(picFN_);
	if(generateMipmap_) t->TryGenerateMipmap();

	this->xxx1 = { t, X, Y, W, H, ANCHOR };
	this->xxx2 = { t, X, Y, W, H, ANCHOR };
	this->xxx3 = { t, X, Y, W, H, ANCHOR };
	...
	this->xxxA_[0] = { t, X, Y, W, H, ANCHOR };
	this->xxxA_[1] = { t, X, Y, W, H, ANCHOR };
	this->xxxA_[2] = { t, X, Y, W, H, ANCHOR };
	...
	this->xxxB_[0] = { t, X, Y, W, H, ANCHOR };
	this->xxxB_[1] = { t, X, Y, W, H, ANCHOR };
	this->xxxB_[2] = { t, X, Y, W, H, ANCHOR };
	...
	this->xxxC_[0] = { t, X, Y, W, H, ANCHOR };
	this->xxxC_[1] = { t, X, Y, W, H, ANCHOR };
	this->xxxC_[2] = { t, X, Y, W, H, ANCHOR };
	...
}


*/

int main() {
	SetConsoleOutputCP(CP_UTF8);
	auto&& cp = std::filesystem::current_path();
	std::cout << "tool: texturepacker cocos2dx *.plist -> *.h & cpp\n\nworking dir: " << cp.string() << R"#(

tips: 
file name can't contains space or dot
texture packer config( switch to advance )
Data:
	Data Format: cocos2d-x
	Data file: ??????.plist
	Trim sprite names: [v]
Texture: ( optional )
	PNG-8 ( indexed )
Layout:
	Max size: ?????
	Size constraints: POT
	Force squared: [v]
	Allow rotation: [ ]
	Detect identical sprites: [ ]
Sprites:
	Trim mode: None
	Shape Padding: 8
	
press ENTER to continue...)#";
	std::cin.get();

	std::unordered_map<std::string, std::vector<std::string>> plists;		// plist file name : keys
	std::map<std::string, std::string> keys;								// key : owner plist file name
	std::unordered_map<std::string, xx::TexturePackerReader::Plist> tps;	// plist file name : data

	for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
		if (!entry.is_regular_file()) continue;
		auto&& p = entry.path();
		if (p.extension() != u8".plist") continue;

		auto plistName = xx::U8AsString(p.filename().u8string());
		auto fullpath = xx::U8AsString(std::filesystem::absolute(p).u8string());
		auto blistPath = fullpath.substr(0, fullpath.size() - 6) + ".blist";

		auto [iter, success] = plists.emplace(plistName, std::vector<std::string>{});
		assert(success);

		xx::Data fd;
		if (int r = xx::ReadAllBytes(p, fd)) {
			std::cerr << "ReadAllBytes failed. r = " << r << " fn = " << p << std::endl;
			return -__LINE__;
		}

		auto&& rtv = tps.emplace(plistName, xx::TexturePackerReader::Plist{});
		assert(rtv.second);
		auto& tp = rtv.first->second;
		if (int r = tp.Load(fd)) {
			std::cerr << "tp.Load failed. r = " << r << " fn = " << p << std::endl;
			return -__LINE__;
		}

		std::cout << "\nhandle file: " << p << std::endl;

		// check file info
		if (tp.metadata.size.width != tp.metadata.size.height) {
			std::cerr << "**************************** bad file size( width == height ): " << p << std::endl;
			return -__LINE__;
		}
		if (!xx::IsPowerOfTwo(tp.metadata.size.width)) {
			std::cerr << "**************************** bad file size( width & height should be 2^n ): " << p << std::endl;
			return -__LINE__;
		}

		for (auto& f : tp.frames) {
			std::cout << "handle frame: " << f.name << std::endl;

			for (auto const& c : f.name) {
				if (c == '.' || c == ' ') {
					std::cerr << "**************************** bad key name( can't contain ' ' or '.' ): " << f.name
						<< "\n**************************** in plist: " << plistName
						<< std::endl;
					return -__LINE__;
				}
			}

			if (auto [iter, success] = keys.emplace(f.name, plistName); !success) {
				std::cerr << "**************************** duplicate res name: " << f.name
					<< "\n**************************** in plist: " << iter->second
					<< "\n**************************** and plist " << plistName
					<< std::endl;
				return -__LINE__;
			}
			iter->second.push_back(f.name);
		}
	}


	for (auto&& [plistFileName, tp] : tps) {
		auto structName = plistFileName.substr(0, plistFileName.size() - 6);

		// group by prefix...._number
		std::map<std::string, std::vector<std::string>> keyGroups;
		std::vector<std::string> keys;

		auto&& allKeys = plists[plistFileName];
		for (auto&& key : allKeys) {
			if (auto idx = key.find_last_of('_'); idx != key.npos) {
				auto k = key.substr(0, idx);
				auto v = key.substr(idx + 1);
				if (v.find_first_not_of("0123456789"sv) != v.npos) goto LabSingle;
				keyGroups[k].push_back(v);
			}
			else {
			LabSingle:
				keys.push_back(key);
			}
		}

		// sort by number
		for (auto&& kv : keyGroups) {
			auto& ss = kv.second;
			std::sort(ss.begin(), ss.end(), [](std::string const& a, std::string const& b)->bool {
				return xx::SvToNumber<int>(a) < xx::SvToNumber<int>(b);
			});
		}

		std::string code, tmp;

		for (auto k : keys) {
			if (keyGroups.contains(k))
				continue;
			if (k[0] >= '0' && k[0] <= '9') k = '_' + k;
			xx::Append(tmp, R"#(
	xx::Frame )#", k, R"#(;)#");
		}
		for (auto&& kv : keyGroups) {
			auto k = kv.first;
			if (k[0] >= '0' && k[0] <= '9') k = '_' + k;
			xx::Append(tmp, R"#(
	std::array<xx::Frame, )#", kv.second.size(), R"#(> )#", k, R"#(_;)#");
		}

		xx::Append(code, R"#(#pragma once
#include "pch.h"
struct )#", structName, R"#( {)#", tmp, R"#(

	void Load(std::string picFN_, bool generateMipmap_ = true);
};
)#");
		// save to file
		auto fn = structName + ".h";
		if (int r = xx::WriteAllBytes((std::u8string&)fn, code.data(), code.size())) {
			std::cerr << "write file failed! r = " << r << std::endl;
			return -__LINE__;
		}

		// todo: cpp
		code.clear();
		tmp.clear();

		for (auto k : keys) {
			auto f = &tp.frames[0];
			for (auto& o : tp.frames) {
				if (o.name == k) {
					f = &o;
					break;
				}
			}
			xx::XY anchor{ 0.5f };
			if (f->anchor.has_value()) anchor = *f->anchor;
			if (k[0] >= '0' && k[0] <= '9') k = '_' + k;
			xx::Append(tmp, R"#(
	this->)#", k, " = { t, ", f->textureRect.x, ", ", f->textureRect.y, ", ", f->textureRect.width, ", ", f->textureRect.height, ", { ", anchor.x, ", ", anchor.y, " } };");
		}

		for (auto&& kv : keyGroups) {
			auto k = kv.first;
			if (k[0] >= '0' && k[0] <= '9') k = '_' + k;
			auto& names = kv.second;
			for(int i = 0; i < names.size(); ++i) {
				auto name = k + "_" + names[i];
				auto f = &tp.frames[0];
				for (auto& o : tp.frames) {
					if (o.name == name) {
						f = &o;
						break;
					}
				}
				xx::XY anchor{ 0.5f };
				if (f->anchor.has_value()) anchor = *f->anchor;
				xx::Append(tmp, R"#(
	this->)#", k, "_[", i, "] = { t, ", f->textureRect.x, ", ", f->textureRect.y, ", ", f->textureRect.width, ", ", f->textureRect.height, ", { ", anchor.x, ", ", anchor.y, " } };");
			}
		}

		xx::Append(code, R"#(#include "pch.h"
#include "game.h"
#include ")#", structName, R"#(.h"
void )#", structName, R"#(::Load(std::string picFN_, bool generateMipmap_) {
	auto t = gg.LoadTexture(picFN_);
	if(generateMipmap_) t->TryGenerateMipmap();
)#", tmp, R"#(
};
)#");
		// save to file
		fn = structName + ".cpp";
		if (int r = xx::WriteAllBytes((std::u8string&)fn, code.data(), code.size())) {
			std::cerr << "write file failed! r = " << r << std::endl;
			return -__LINE__;
		}
	}

	xx::CoutN("finished! press ENTER to continue...");
	std::cin.get();

	return 0;
}
