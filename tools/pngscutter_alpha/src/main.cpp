#include "pch.h"
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main() {
	SetConsoleOutputCP(CP_UTF8);
	auto&& cp = std::filesystem::current_path();
	std::cout << "tool: *.png -> *_{n}.png\n\nworking dir: " << cp.string() << R"#(

search content by non alpha area

press ENTER to continue...)#";
	std::cin.get();

	struct FN {
		std::string fileName, fullpath;
	};
	xx::List<FN> fns;

	for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
		if (!entry.is_regular_file()) continue;
		auto&& p = entry.path();
		if (p.extension() != u8".png") continue;

		auto fileName = xx::U8AsString(p.filename().u8string());
		if (fileName.contains("_cutalpha_")) continue;
		auto fullpath = xx::U8AsString(std::filesystem::absolute(p).u8string());
		fns.Emplace(std::move(fileName), std::move(fullpath));
	}

	for(auto& f : fns) {
		xx::CoutN("handle ", f.fileName, ":");
		auto outNamePrefix = f.fileName.substr(0, f.fileName.size() - 4) + "_cutalpha_";

		auto d = xx::ReadAllBytes(f.fullpath);
		if (!d.len) {
			xx::CoutN(" read file failed! ", f.fullpath);
			return __LINE__;
		}
		xx::STBImage img;
		img.Fill(d);
		if (img.comp != 4) {
			xx::CoutN(f.fileName, " have no ALPHA channel! ignore!");
			continue;
		}

		auto w = (uint32_t)img.w;
		auto h = (uint32_t)img.h;
		int32_t n{};
		xx::List<uint32_t> ps;
		std::unordered_set<uint32_t> set;
	LabRetry:;
		// search first non alpha point
		for (int32_t y = 0; y < h; ++y) {
			for (int32_t x = 0; x < w; ++x) {
				auto idx = y * w + x;
				auto& c = img.At(idx);
				if (c.a) {
					set.insert(idx);
					ps.Emplace(idx);
					goto LabNext;
				}
			}
		}

	LabNext:;
		// add all neighbor point indexs ( recursive )
		auto TryAdd = [&](int32_t nidx) {
			if (!set.contains(nidx) && img.At(nidx).a) {
				set.insert(nidx);
				ps.Emplace(nidx);
			}
		};
		xx::XYi minXY{ std::numeric_limits<int32_t>::max() }, maxXY{ std::numeric_limits<int32_t>::min() };
		for (int32_t i = 0; i < ps.len; ++i) {
			auto idx = ps[i];
			auto y = idx / w;
			auto x = idx - w * y;
			if ((int32_t)x < minXY.x) minXY.x = (int32_t)x;
			if ((int32_t)x > maxXY.x) maxXY.x = (int32_t)x;
			if ((int32_t)y < minXY.y) minXY.y = (int32_t)y;
			if ((int32_t)y > maxXY.y) maxXY.y = (int32_t)y;
			// X??
			// ?O?
			// ???
			if (y > 0 && x > 0) TryAdd((y - 1) * w + x - 1);
			// ?X?
			// ?O?
			// ???
			if (y > 0) TryAdd((y - 1) * w + x);
			// ??X
			// ?O?
			// ???
			if (y > 0) TryAdd((y - 1) * w + x + 1);
			// ???
			// XO?
			// ???
			if (x > 0) TryAdd(y * w + x - 1);
			// ???
			// ?OX
			// ???
			if (x < w - 1) TryAdd(y * w + x + 1);
			// ???
			// ?O?
			// X??
			if (y < h - 1 && x > 0) TryAdd((y + 1) * w + x - 1);
			// ???
			// ?O?
			// ?X?
			if (y < h - 1) TryAdd((y + 1) * w + x);
			// ???
			// ?O?
			// ??X
			if (y < h - 1 && x < w - 1) TryAdd((y + 1) * w + x + 1);
		}

		// create sub png & write
		if (!set.empty()) {
			assert(ps.len == set.size());
			auto sw = maxXY.x - minXY.x + 1;
			auto sh = maxXY.y - minXY.y + 1;
			// ignore small size subs
			if (sw >= 16 && sh >= 16) {
				xx::CoutN("find out sub content: num pixels = ", ps.len, " width = ", sw, " height = ", sh);
				auto obuf = std::make_unique<uint32_t[]>(sw * sh);
				for (auto idx : ps) {
					auto& c = (uint32_t&)img.At(idx);
					auto y = idx / w;
					auto x = idx - w * y;
					assert(y < h);
					assert(x < w);
					auto oy = y - minXY.y;
					auto ox = x - minXY.x;
					assert(oy < sh);
					assert(ox < sw);
					obuf[oy * sw + ox] = c;
					c = 0;
				}
				stbi_write_png((outNamePrefix + xx::ToString(n) + ".png").c_str(), sw, sh, 4, obuf.get(), 0);
				++n;
			}
			else {
				for (auto idx : ps) {
					auto& c = (uint32_t&)img.At(idx);
					c = 0;
				}
			}
			set.clear();
			ps.Clear();
			goto LabRetry;
		}
	}

	xx::CoutN("finished! press ENTER to continue...");
	std::cin.get();

	return 0;
}
