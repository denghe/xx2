#include "pch.h"
#include <iostream>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    xx::GameBase gameBase;  // for BMFont file system

    auto&& cp = std::filesystem::current_path();
    std::cout << "tool: *.fnt -> *_{n}.png & *.cpp(mapping code)\n\nworking dir: " << xx::U8AsString(cp.u8string()) << R"#(

press ENTER to continue...)#";
    std::cin.get();

    for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
        if (!entry.is_regular_file()) continue;
        auto&& p = entry.path();
        if (p.extension() != u8".fnt") continue;

        xx::BMFont bmf;
        auto s = p.u8string();
		std::u8string_view sv(s);
        auto r = bmf.Init((std::string_view&)sv, false);
        if (r != 0) {
			xx::CoutN("failed to load BMFont. fn = ", s, ", r = ", r);
			continue;
        }
        else {
            for (auto& c : bmf.charArray) {
                xx::CoutN(c.x, " ", c.y);
            }
        }
    }

    xx::CoutN("finished! press ENTER to continue...");
    std::cin.get();

    return 0;
}
