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
	std::cout << "tool: cut *.png space with shared max content size by center point  \n\nworking dir: " << xx::U8AsString(cp.u8string()) << R"#(

press ENTER to continue...)#";
	std::cin.get();

	std::vector<std::string> fns;

	int32_t mtx{ 999999 }, mty{ 999999 };

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

		// left, top cut size store
		int32_t tx{ 999999 }, ty{ 999999 };

		// search left & right space size
		for (int32_t x = 0; x < w / 2; ++x) {
			for (int32_t y = 0; y < h; ++y) {
				auto a1 = d[(y * w + x) * 4 + 3];
				auto a2 = d[(y * w + (w - x - 1)) * 4 + 3];
				if (a1 || a2) {
					tx = x;
					goto LabStep1;
				}
			}
		}

	LabStep1:
		// search top & bottom space size
		for (int32_t y = 0; y < h / 2; ++y) {
			for (int32_t x = tx; x < w - tx; ++x) {
				auto a1 = d[(y * w + x) * 4 + 3];
				auto a2 = d[((h - y - 1) * w + x) * 4 + 3];
				if (a1 || a2) {
					ty = y;
					goto LabStep2;
				}
			}
		}

	LabStep2:
		if (tx < mtx) mtx = tx;
		if (ty < mty) mty = ty;
	}

	if (mtx > 0 && mtx < 999999 || mty > 0 && mty < 999999) {
		for (auto& fn : fns) {
			auto cfn = fn.c_str();

			// load
			int32_t w, h, comp;
			auto d = stbi_load(cfn, &w, &h, &comp, 0);
			auto dd = xx::MakeSimpleScopeGuard([&d] {stbi_image_free(d); });

			// store
			stbi_write_png(cfn, w - mtx * 2, h - mty * 2, comp, d + (mty * w + mtx) * 4, w * 4);
			std::cout << "cuted file: " << fn << std::endl;
		}
	}
	xx::CoutN("finished! press ENTER to continue...");
	std::cin.get();

	return 0;
}
