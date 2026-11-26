#include <pch.h>
#include <ajson.hpp>
#include <iostream>

/*
* template
* xxtpFileName.h

#pragma once
#include <xx_frame.h>

struct XxtpFileName {
	xx::Frame xxx1;
	xx::Frame xxx2;
	xx::Frame xxx3;
	...
	std::array<xx::Frame, ???> xxxA_;
	std::array<xx::Frame, ???> xxxB_;
	std::array<xx::Frame, ???> xxxC_;
	...
	xx::FromTo<XY> _scale9_xxx1;
	xx::FromTo<XY> _scale9_xxx2;
	xx::FromTo<XY> _scale9_xxx3;
	...

	xx::Shared<xx::GLTexture> Load(std::filesystem::path const& rootPath_, bool generateMipmap_ = true);
};

///////////////////////////////////////////////////////

* xxtpFileName.cpp

#include <xx_gamebase.h>
#include "xxtpFileName.h"

xx::Shared<xx::GLTexture> XxtpFileName::Load(std::filesystem::path const& rootPath_, bool generateMipmap_ = true) {
	auto p = rootPath_ / "?????????????.png";
	auto t = xx::GameBase::instance->LoadTexture(xx::U8AsString(p.u8string()));
	if(generateMipmap_) t->TryGenerateMipmap();

	static constexpr xx::UVRect uvrects[] = {
		{X, Y, W, H},
		{X, Y, W, H},
		{X, Y, W, H},
		// ...
	};

	static constexpr xx::UVRect uvtrims[] = {
		{},
		{oX, oY, sW, sH},
		{},
		// ...
	};

	static constexpr xx::XY anchors[] = {
		{ AX, AY },
		{ AX, AY },
		{ AX, AY },
		// ...
	};

	static constexpr xx::FromTo<xx::XY> scale9s[] = {
		{{ ox, oy }, { cw, ch }},
		{{ ox, oy }, { cw, ch }},
		{{ ox, oy }, { cw, ch }},
		// ...
	};

	for (int32_t i = 0; i < ????????; i++) {
		auto& f = ((xx::Frame*)this)[i];
		f.tex = t;
		f.uvRect = uvrects[i];
		f.anchor = anchors[i];
		f.uvTrim = uvtrims[i];
		f.extrude = ??????????????????;
	}

	for (int32_t i = 0; i < ????????; i++) {
		(&this->)_scale9_ first var)[i] = scale9s[i];
	}

	return t;
}


data example:

{
  "texture":"test32.png",
  "size":{"w":128,"h":128},
  "extrude":1,
  "frames":[
	{"name":"32","uv":{"x":1,"y":37,"w":32,"h":32},"scale9":{"x":2,"y":2,"w":28,"h":28}},
	{"name":"34","uv":{"x":1,"y":1,"w":34,"h":34},"pivot":{"x":0.000000,"y":0.000000},"scale9":{"x":3,"y":3,"w":28,"h":28}},
	{"name":"p2","uv":{"x":1,"y":71,"w":15,"h":10},"trim":{"oX":10,"oY":7,"sW":34,"sH":34}},
	{"name":"pivottest","uv":{"x":1,"y":71,"w":15,"h":10},"trim":{"oX":10,"oY":7,"sW":34,"sH":34},"pivot":{"x":0.500000,"y":0.750000}}
  ]
}

*/

struct TPUV {
	int32_t x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 };
};

struct TPScale9 {
	int32_t x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 };
	operator bool() const {
		return x != 0 || y != 0 || w != 0 || h != 0;
	}
};

struct TPPivot {
	float x{ 0.5f }, y{ 0.5f };
	operator bool() const {
		return x != 0.5f || y != 0.5f;
	}
};

struct TPTrim {
	int32_t oX{ 0 }, oY{ 0 }, sW{ 0 }, sH{ 0 };
	operator bool() const {
		return sW != 0 || sH != 0;
	}
};

struct TPNode {
	std::string name;
	TPUV uv;
	TPTrim trim;
	TPPivot pivot, pivotBak;
	TPScale9 scale9;
};

struct TPSize {
	int32_t w{}, h{};
};

struct TP {
	std::string texture;
	TPSize size;
	int32_t extrude{0};
	std::vector<TPNode> frames;
};

AJSON(TPUV, x, y, w, h);
AJSON(TPScale9, x, y, w, h);
AJSON(TPTrim, oX, oY, sW, sH);
AJSON(TPPivot, x, y);
AJSON(TPNode, name, uv, trim, pivot, scale9);
AJSON(TPSize, w, h);
AJSON(TP, texture, size, extrude, frames);


int main() {
	SetConsoleOutputCP(CP_UTF8);
	auto&& cp = std::filesystem::current_path();
	std::cout << "tool: texturepacker xx *.xxtp -> *.h & cpp\n\nworking dir: " << xx::U8AsString(cp.u8string()) << R"#(
more help: see res/readme.txt
press ENTER to continue...)#";
	std::cin.get();

	std::unordered_map<std::string, std::vector<std::string>> files;		// file name : keys
	std::unordered_map<std::string, TP> tps;								// file name : data

	for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
		if (!entry.is_regular_file()) continue;
		auto&& p = entry.path();
		if (p.extension() != u8".xxtp") continue;

		auto fileName = xx::U8AsString(p.filename().u8string());
		auto fullpath = xx::U8AsString(std::filesystem::absolute(p).u8string());

		auto [iter, success] = files.emplace(fileName, std::vector<std::string>{});
		assert(success);

		auto fd = xx::ReadAllBytes(p);
		if (!fd) {
			std::cerr << "ReadAllBytes failed: " << p << std::endl;
			return __LINE__;
		}

		auto&& rtv = tps.emplace(fileName, TP{});
		assert(rtv.second);
		auto& tp = rtv.first->second;

		ajson::load_from_buff(tp, (char*)fd.buf, fd.len);
		std::cout << "\nhandle file: " << p << std::endl;

		//// ensure file size
		//if (tp.size.w != tp.size.h) {
		//	std::cerr << "**************************** bad file size. require: width == height: " << p << std::endl;
		//	return __LINE__;
		//}
		//if (!std::has_single_bit((uint32_t)tp.size.w)) {
		//	std::cerr << "**************************** bad file size( width & height should be 2^n ): " << p << std::endl;
		//	return __LINE__;
		//}

		// prepare
		for (auto& f : tp.frames) {
			auto& k = f.name;
			std::cout << "handle frame: " << k << std::endl;
			if (k[0] >= '0' && k[0] <= '9') k = '_' + k;
			for (auto& c : k) {
				// todo: more replace here
				if (c == '/' || c == '.' || c == ' ') c = '_';
			}
			iter->second.push_back(k);
			f.pivotBak = f.pivot;
			if (f.trim) {
				// calculate new pivot
				f.pivot.x = (f.pivot.x * f.trim.sW - f.trim.oX) / f.uv.w;
				auto oy = /* f.trim.y */ f.trim.sH - f.trim.oY - f.uv.h;  // for opengl flip Y
				f.pivot.y = (f.pivot.y * f.trim.sH - oy) / f.uv.h;
			}
		}
	}


	for (auto&& [plistFileName, tp] : tps) {
		auto structName = plistFileName.substr(0, plistFileName.size() - 5);	// 5: .xxtp
		auto codeFileName = structName;
		structName[0] = std::toupper((uint8_t)structName[0]);

		// group by prefix...._number
		std::map<std::string, std::vector<std::string>> keyGroups;
		std::vector<std::string> keys;

		auto&& allKeys = files[plistFileName];
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
		std::string uvrectsCode, uvtrimsCode, anchorsCode, scale9sCode;
		int32_t count{};

		auto HandleTPNode = [&](TPNode* f) {
			++count;
			xx::Append(uvrectsCode, R"#(
	{ )#", f->uv.x, ", ", f->uv.y, ", ", f->uv.w, ", ", f->uv.h, " },");
			if (f->trim) {
				xx::Append(uvtrimsCode, R"#(
	{ )#", f->trim.oX, ", ", f->trim.oY, ", ", f->trim.sW, ", ", f->trim.sH, " },");
			}
			else {
				xx::Append(uvtrimsCode, R"#(
	{}, )#");
			}
			if (f->pivot.x == f->pivot.y) {
				xx::Append(anchorsCode, R"#(
	{ )#", f->pivot.x, " },");
			}
			else {
				xx::Append(anchorsCode, R"#(
	{ )#", f->pivot.x, ", ", f->pivot.y, " },");
			}
			if (f->trim) {
				xx::Append(anchorsCode, " // src povit: ", f->pivotBak.x, ", ", f->pivotBak.y);
			}
			if (f->scale9) {
				xx::Append(scale9sCode, R"#(
	static constexpr xx::FromTo<xx::XY> _scale9_)#", f->name, "{ {", f->scale9.x, ", ", f->scale9.y, "}, {", f->scale9.w, ", ", f->scale9.h, "} };");
			}
		};

		for (auto& k : keys) {
			if (keyGroups.contains(k))
				continue;
			auto f = &tp.frames[0];
			for (auto& o : tp.frames) {
				if (o.name == k) {
					f = &o;
					break;
				}
			}
			HandleTPNode(f);
			xx::Append(tmp, R"#(
	xx::Frame )#", k, R"#(;)#");
		}

		for (auto&& kv : keyGroups) {
			auto& k = kv.first;
			auto& names = kv.second;
			xx::Append(tmp, R"#(
	std::array<xx::Frame, )#", kv.second.size(), R"#(> )#", k, R"#(_;)#");
			for (int i = 0; i < names.size(); ++i) {
				auto name = kv.first + "_" + names[i];
				auto f = &tp.frames[0];
				for (auto& o : tp.frames) {
					if (o.name == name) {
						f = &o;
						break;
					}
				}
				HandleTPNode(f);
			}
		}

		xx::Append(code, R"#(#pragma once
#include <xx_frame.h>

struct )#", structName, R"#( {)#", tmp, scale9sCode, R"#(

	xx::Shared<xx::GLTexture> Load(std::filesystem::path const& rootPath_, bool generateMipmap_ = true);
};
)#");
		// save to file
		auto fn = codeFileName + ".h";
		if (int r = xx::WriteAllBytes((std::u8string&)fn, code.data(), code.size())) {
			std::cerr << "write file failed! r = " << r << std::endl;
			return __LINE__;
		}

		code.clear();
		tmp.clear();

		xx::Append(tmp, R"#(
	static constexpr xx::UVRect uvrects[] = {)#", uvrectsCode, R"#(
	};

	static constexpr xx::UVRect uvtrims[] = {)#", uvtrimsCode, R"#(
	};

	static constexpr xx::XY anchors[] = {)#", anchorsCode, R"#(
	};

	for (int32_t i = 0; i < )#", count, R"#(; i++) {
		auto& f = ((xx::Frame*)this)[i];
		f.tex = t;
		f.uvRect = uvrects[i];
		f.uvTrim = uvtrims[i];
		f.anchor = anchors[i];
		f.extrude = )#", tp.extrude, R"#(;
	}
)#");

		xx::Append(code, R"#(#include <xx_gamebase.h>
#include ")#", codeFileName, R"#(.h"

xx::Shared<xx::GLTexture> )#", structName, R"#(::Load(std::filesystem::path const& rootPath_, bool generateMipmap_) {
	auto t = xx::GameBase::instance->LoadTexture(xx::U8AsString((rootPath_ / ")#", tp.texture, R"#(").u8string()));
	if (generateMipmap_) t->TryGenerateMipmap();
)#", tmp, R"#(
	return t;
};
)#");
		// save to file
		fn = codeFileName + ".cpp";
		if (int r = xx::WriteAllBytes((std::u8string&)fn, code.data(), code.size())) {
			std::cerr << "write file failed! r = " << r << std::endl;
			return __LINE__;
		}
	}
	xx::CoutN("finished! press ENTER to continue...");
	std::cin.get();

	return 0;
}
