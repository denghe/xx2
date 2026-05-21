#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <filesystem>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main() {
	SetConsoleOutputCP(CP_UTF8);
	auto&& cp = std::filesystem::current_path();
	std::cout << "tool: cut *.png bottom space with shared max content size\n\nworking dir: " << xx::U8AsString(cp.u8string()) << R"#(

press ENTER to continue...)#";
	std::cin.get();

	std::vector<std::string> fns;

	int32_t maxH{};

	for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
		if (!entry.is_regular_file()) continue;
		auto&& p = entry.path();
		if (p.extension() != u8".png") continue;

		// load
		int32_t w, h, comp;
		auto fn = xx::U8AsString(p.u8string());
		auto cfn = fn.c_str();
		fns.push_back(fn);

		auto d = stbi_load(cfn, &w, &h, &comp, 0);
		auto dd = xx::MakeSimpleScopeGuard([&d] {stbi_image_free(d); });

		// search bottom space size
		for (int32_t y = h - 1; y >= 0; --y) {
			for (int32_t x = 0; x < w; ++x) {
				auto a1 = d[(y * w + x) * 4 + 3];
				if (a1) {
					if (y + 1 > maxH) maxH = y + 1;
					goto LabNext;
				}
			}
		}

	LabNext:;
	}

	if (maxH) {
		for (auto& fn : fns) {
			auto cfn = fn.c_str();

			// load
			int32_t w, h, comp;
			auto d = stbi_load(cfn, &w, &h, &comp, 0);
			auto dd = xx::MakeSimpleScopeGuard([&d] {stbi_image_free(d); });

			if (maxH < h) {
				// store
				stbi_write_png(cfn, w, maxH, comp, d, w * 4);
				std::cout << "cuted file: " << fn << std::endl;
			}
		}
	}
	xx::CoutN("finished! press ENTER to continue...");
	std::cin.get();

	return 0;
}
