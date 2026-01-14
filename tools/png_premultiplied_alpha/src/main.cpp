#include "pch.h"
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main() {
	SetConsoleOutputCP(CP_UTF8);
	auto&& cp = std::filesystem::current_path();
	std::cout << "tool: *.png -> *.pnga ( premultiplied alpha )\n\nworking dir: " << cp.string() << R"#(

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
		auto fullpath = xx::U8AsString(std::filesystem::absolute(p).u8string());
		fns.Emplace(std::move(fileName), std::move(fullpath));
	}

	for(auto& f : fns) {
		xx::CoutN("handle ", f.fileName, ":");
		auto outNamePrefix = f.fileName.substr(0, f.fileName.size() - 4);

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
		for (int y = 0; y < h; ++y) {
			for(int x = 0; x < w; ++x) {
				auto& px = img.At(y * w + x);
				if (px.a == 255) continue;
				else if (px.a == 0) {
					px.r = px.g = px.b = 0;
				}
				else {
					px.r = (uint8_t)(px.r * px.a * (1.f / 255.f));
					px.g = (uint8_t)(px.g * px.a * (1.f / 255.f));
					px.b = (uint8_t)(px.b * px.a * (1.f / 255.f));
				}
			}
		}
		stbi_write_png((outNamePrefix + ".pnga").c_str(), w, h, 4, img.buf, 0);
	}

	xx::CoutN("finished! press ENTER to continue...");
	std::cin.get();

	return 0;
}
