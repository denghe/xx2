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
	std::cout << "tool: split special tiled walls png ( 256x348 ) to filename_ byte num \n\nworking dir: " << xx::U8AsString(cp.u8string()) << R"#(

press ENTER to continue...)#";
	std::cin.get();

	int32_t maxH{};

	for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
		if (!entry.is_regular_file()) continue;
		auto&& p = entry.path();
		if (p.extension() != u8".png") continue;

		// load
		auto fn = xx::U8AsString(p.u8string());
		int32_t w, h, comp;
		auto d = stbi_load(fn.c_str(), &w, &h, &comp, 0);
		auto dd = xx::MakeSimpleScopeGuard([&d] {stbi_image_free(d); });
		if (w != 256 || h != 348) continue;

		// 123|123|123|1  |  3|123|123|123
		// 8  |   |  4|8  |  4|8  |  4|8 4
		// 7  |   |  5|7 5|7 5|7 5|7 5|7 5
		// ---+---+---+---+---+---+---+---
		// 199|  7| 31|209| 92|215| 95|223
		// ---+---+---+---+---+---+---+---
		// 1  |   |  3|1 3|1 3|1 3|1 3|1 3
		// 8  |   |  4|8  |  4|8  |  4|8 4
		// 7  |   |  5|7  |  5|765|765|7 5
		// ---+---+---+---+---+---+---+---
		// 193|  0| 28|197| 29|245|125|221
		// ---+---+---+---+---+---+---+---
		// 1  |   |  3|123|123|1 3|123|1 3
		// 8  |   |  4|   |   |8  |   |8 4
		// 765|765|765|  5|7  |7 5|7 5|765
		// ---+---+---+---+---+---+---+---
		// 241|112|124| 23| 71|213| 87|253
		// ---+---+---+---+---+---+---+---
		//   3|   |1  |  3|1  |1 3|1 3|123
		//    |   |   |   |   |   |  4|8 4
		// 7 5|7 5|7 5|765|765|765|7 5|765
		// ---+---+---+---+---+---+---+---
		// 	84| 80| 81|116|113|117| 93|255
		// ---+---+---+---+---+---+---+---
		//   3|1 3|1  |   |   |123|123|123
		//    |   |   |   |   |8  |   |  4
		//   5|7 5|7  |  5|7  |765|765|765
		// ---+---+---+---+---+---+---+---
		// 	20|	85| 65| 16| 64|247|119|127
		// ---+---+---+---+---+---+---+---
		// 1 3|1 3|1 3|  3|1  |  3|1  |   
		//    |   |   |   |   |   |   |   
		//   5|   |7  |   |   |7  |  5|   
		// ---+---+---+---+---+---+---+---
		// 	21|  5| 69|  4|  1| 68| 17|
		// ---+---+---+---+---+---+---+---

		auto basefn = fn.substr(0, fn.length() - 4);
		auto Cut = [&d, &comp, &basefn, &w](int32_t r, int32_t c, uint8_t b, bool includeFront = false) {
			auto sw = 32;
			auto sh = includeFront ? 32 + 26 : 32;
			auto x = 32 * c;
			auto y = (32 + 26) * r;

			// store
			auto sfn = basefn + "_" + std::to_string(b) + ".png";
			stbi_write_png(sfn.c_str(), sw, sh, comp, d + (y * w + x) * comp, w * comp);
			std::cout << "cuted file: " << sfn << std::endl;
		};

		Cut(1, 1, 0);
		Cut(5, 4, 1);
		Cut(5, 3, 4);
		Cut(5, 1, 5);
		Cut(0, 1, 7);
		Cut(4, 3, 16);
		Cut(5, 6, 17);
		Cut(4, 0, 20);
		Cut(5, 0, 21);
		Cut(2, 3, 23);
		Cut(1, 2, 28);
		Cut(1, 4, 29);
		Cut(0, 2, 31);
		Cut(4, 4, 64);
		Cut(4, 2, 65);
		Cut(5, 5, 68);
		Cut(5, 2, 69);
		Cut(2, 4, 71);
		Cut(3, 1, 80);
		Cut(3, 2, 81);
		Cut(3, 0, 84);
		Cut(4, 1, 85);
		Cut(2, 6, 87);
		Cut(0, 4, 92);
		Cut(3, 6, 93);
		Cut(0, 6, 95);
		Cut(2, 1, 112, true);
		Cut(3, 4, 113, true);
		Cut(3, 3, 116, true);
		Cut(3, 5, 117, true);
		Cut(4, 6, 119, true);
		Cut(2, 2, 124, true);
		Cut(1, 6, 125, true);
		Cut(4, 7, 127, true);
		Cut(1, 0, 193);
		Cut(1, 3, 197);
		Cut(0, 0, 199);
		Cut(0, 3, 209);
		Cut(2, 5, 213);
		Cut(0, 5, 215);
		Cut(1, 7, 221);
		Cut(0, 7, 223);
		Cut(2, 0, 241, true);
		Cut(1, 5, 245, true);
		Cut(4, 5, 247, true);
		Cut(2, 7, 253, true);
		Cut(3, 7, 255, true);

		/*
		for code copy

auto WallsMapTo = [this](std::array<xx::TinyFrame, 256>& tar, std::array<xx::Frame, 47> const& src) {
	tar[0] = src[0];
	tar[1] = src[1];
	tar[4] = src[2];
	tar[5] = src[3];
	tar[7] = src[4];
	tar[16] = src[5];
	tar[17] = src[6];
	tar[20] = src[7];
	tar[21] = src[8];
	tar[23] = src[9];
	tar[28] = src[10];
	tar[29] = src[11];
	tar[31] = src[12];
	tar[64] = src[13];
	tar[65] = src[14];
	tar[68] = src[15];
	tar[69] = src[16];
	tar[71] = src[17];
	tar[80] = src[18];
	tar[81] = src[19];
	tar[84] = src[20];
	tar[85] = src[21];
	tar[87] = src[22];
	tar[92] = src[23];
	tar[93] = src[24];
	tar[95] = src[25];
	tar[112] = src[26];
	tar[113] = src[27];
	tar[116] = src[28];
	tar[117] = src[29];
	tar[119] = src[30];
	tar[124] = src[31];
	tar[125] = src[32];
	tar[127] = src[33];
	tar[193] = src[34];
	tar[197] = src[35];
	tar[199] = src[36];
	tar[209] = src[37];
	tar[213] = src[38];
	tar[215] = src[39];
	tar[221] = src[40];
	tar[223] = src[41];
	tar[241] = src[42];
	tar[245] = src[43];
	tar[247] = src[44];
	tar[253] = src[45];
	tar[255] = src[46];
};

WallsMapTo(wallsShadowMaskTiles, pics.tiled_walls_shadowmask_);
WallsMapTo(wallsTiles[0], pics.tiled_walls_1_);
WallsMapTo(wallsTiles[1], pics.tiled_walls_2_);
WallsMapTo(wallsTiles[2], pics.tiled_walls_3_);
...

		*/
	}

	std::cout <<"finished! press ENTER to continue...\n";
	std::cin.get();

	return 0;
}
