#include "pch.h"
#include <iostream>

int main() {
	SetConsoleOutputCP(CP_UTF8);
	auto&& cp = std::filesystem::current_path();
	std::cout << "tool: jimeng-2025-10-11-1035-XXXXXXXXXXXXXXXX.png -> XXXXXXXXXXXXXXXX-2025-10-11-1035.png\nworking dir: " << xx::U8AsString(cp.u8string()) << "\npress ENTER continue...\n";
	std::cin.get();

	for (auto&& o : std::filesystem::directory_iterator(cp)) {
		if (!o.is_regular_file()) continue;
		auto&& p = o.path();
		auto&& fn = p.filename();
		auto&& ext = p.extension();
		if (ext != u8".png"sv) continue;
		auto u8fn = fn.u8string();
		if (u8fn.size() < 23) continue;
		if (!u8fn.starts_with(u8"jimeng-")) continue;
		if (u8fn[22] != u8'-') continue;
		auto fn2 = u8fn.substr(23, u8fn.size() - 4 - 23);
		fn2.append(std::u8string_view{ u8fn.data() + 6, 22 - 6 });
		fn2.append(u8".png");
		std::filesystem::rename(cp / p, cp / fn2);
	}

	std::cout << "finished!\n press ENTER continue...\n";
	std::cin.get();

	return 0;
}
