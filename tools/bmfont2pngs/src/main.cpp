#include "pch.h"
#include <iostream>

int main() {
	SetConsoleOutputCP(CP_UTF8);
	xx::GameBase gameBase;  // for BMFont Init

	auto&& cp = std::filesystem::current_path();
	std::cout << "tool: *.fnt -> *_{n}.png & *.cpp(mapping code)\n\nworking dir: " << xx::U8AsString(cp.u8string()) << R"#(

press ENTER to continue...)#";
	std::cin.get();

	for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
		if (!entry.is_regular_file()) continue;
		auto&& p = entry.path();
		if (p.extension() != u8".fnt") continue;

		// load fnt
		xx::BMFont bmf;
		auto s = p.u8string();
		std::u8string_view sv(s);
		auto r = bmf.Init((std::string_view&)sv, false);
		if (r != 0) {
			xx::CoutN("failed to load BMFont. fn = ", s, ", r = ", r);
			return __LINE__;
		}

		// load texs
		xx::List<xx::STBImage> imgs;
		imgs.Reserve(bmf.texFNs.len);
		for (auto& o : bmf.texFNs) {
			auto d = xx::ReadAllBytes_sv(o);
			auto& img = imgs.Emplace();
			img.Fill(d);
			if (img.comp != 4) {
				xx::CoutN("failed to load img comp != 4. fn = ", o);
				return __LINE__;
			}
		}

		// make sub dir for output
		auto outDir = p.parent_path() / p.stem();
		if (!std::filesystem::exists(outDir)) {
			std::error_code ec;
			std::filesystem::create_directory(outDir, ec);
		}

		// write char's pngs
		xx::Data buf;

		auto WritePNG = [&](xx::STBImage& img, std::string const& fn, xx::BMFontChar& c) {
			xx::CoutN(fn);
			auto cfn = fn.data();
			buf.Clear();
			buf.Resize(c.width * c.height * 4);
			for (int32_t y = 0; y < c.height; ++y) {
				for (int32_t x = 0; x < c.width; ++x) {
					XYi pos{ c.x + x, c.y + y };
					auto idx1 = pos.y * img.w + pos.x;
					auto idx2 = (y * c.width + x) * 4;
					(uint32_t&)buf[idx2] = (uint32_t&)img[idx1 * 4];
				}
			}
			stbi_write_png(cfn, c.width, c.height, 4, buf.buf, c.width * 4);
		};

		for (int32_t i = 0; i < 256; ++i) {
			auto& c = bmf.charArray[i];
			if (c.width == 0 || c.height == 0) continue;
			WritePNG(imgs[c.page], xx::ToString(outDir, "/", i, ".png"), c);
		}

		for (auto& [k, v] : bmf.charMap) {
			WritePNG(imgs[v.page], xx::ToString(outDir, "/", k, ".png"), v);
		}
	}

	xx::CoutN("finished! press ENTER to continue...");
	std::cin.get();

	return 0;
}
