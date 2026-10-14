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

		fs mean wall Frames
		std::array<Frame, 256> walls;
		walls[0] = fs[0];
		walls[1] = fs[1];
		walls[4] = fs[2];
		walls[5] = fs[3];
		walls[7] = fs[4];
		walls[16] = fs[5];
		walls[17] = fs[6];
		walls[20] = fs[7];
		walls[21] = fs[8];
		walls[23] = fs[9];
		walls[28] = fs[10];
		walls[29] = fs[11];
		walls[31] = fs[12];
		walls[64] = fs[13];
		walls[65] = fs[14];
		walls[68] = fs[15];
		walls[69] = fs[16];
		walls[71] = fs[17];
		walls[80] = fs[18];
		walls[81] = fs[19];
		walls[84] = fs[20];
		walls[85] = fs[21];
		walls[87] = fs[22];
		walls[92] = fs[23];
		walls[93] = fs[24];
		walls[95] = fs[25];
		walls[112] = fs[26];
		walls[113] = fs[27];
		walls[116] = fs[28];
		walls[117] = fs[29];
		walls[119] = fs[30];
		walls[124] = fs[31];
		walls[125] = fs[32];
		walls[127] = fs[33];
		walls[193] = fs[34];
		walls[197] = fs[35];
		walls[199] = fs[36];
		walls[209] = fs[37];
		walls[213] = fs[38];
		walls[215] = fs[39];
		walls[221] = fs[40];
		walls[223] = fs[41];
		walls[241] = fs[42];
		walls[245] = fs[43];
		walls[247] = fs[44];
		walls[253] = fs[45];
		walls[255] = fs[46];
		*/
	}

	std::cout <<"finished! press ENTER to continue...\n";
	std::cin.get();

	return 0;
}
