#include <pch.h>

/*
* template
* XMLFileName.h

#pragma once
#include "pch.h"

struct XMLFileName {
	using XY = xx::XY;

	static constexpr XY xxx[] { {?,?}, {?,?}, ... };
	......
};




txt content example:

Simple plain text exporter for demo purposes

See http://www.grantlee.org/apidox/for_themers.html for more information

Custom values (from the <bodies> section)
	Grav: 9.81

List of all bodies:

	Name:               rock1_1
	Custom values (from the <bodies> section):
		Identifier:     0
		FixedRotation:  NO

		AnchorPointRel: { 0.500,0.500 }
		AnchorPointAbs: { 29.000,27.000 }
		Size:           { 58.000,54.000 }

	List of all fixtures:
		----------------------------------------------------------
		fixtureType:             POLYGON

		Custom values (from the <fixtures> section):
			density:             2
			friction:            0
			restitution:         0
			isSensor:            NO
			filter_groupIndex:   0
			filter_categoryBits: 1
			filter_maskBits:     65535

		Hull polygon:
			 (20, -25)  , (28, -9)  , (22, 13)  , (2, 27)  , (-27, 16)  , (-25, -12)  , (4, -26)

		Convex sub polygons:
			 (-25, -12)  , (4, -26)  , (28, -9)  , (22, 13)  , (2, 27)  , (-27, 16)
			 (28, -9)  , (4, -26)  , (20, -25)

......
*/

int main() {
	SetConsoleOutputCP(CP_UTF8);
	auto&& cp = std::filesystem::current_path();
	std::cout << "tool: PhysicsEditor's Simple plain text store file -> *.h & .cpp\n\nworking dir: " << cp.string() << R"#(

only read Hull polygon data from .txt files in current dir.

press ENTER to continue...)#";
	std::cin.get();

	for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
		if (!entry.is_regular_file()) continue;
		auto&& p = entry.path();
		if (p.extension() != u8".txt") continue;

		auto fileName = xx::U8AsString(p.filename().u8string());
		auto fullPath = xx::U8AsString(std::filesystem::absolute(p).u8string());
		auto xmlName = fileName.substr(0, fileName.size() - 4);

		auto fd = xx::ReadAllBytes(p);
		if (!fd) {
			std::cerr << "ReadAllBytes failed: " << p << std::endl;
			return __LINE__;
		}

		std::string_view sv(fd);
		if(!sv.starts_with("Simple plain text exporter for demo purposes\r\n")) continue;

		std::cout << "\nhandle file: " << fullPath << std::endl;

		std::string h, name, data;
		xx::AppendFormat(h, R"#(#pragma once
#include "pch.h"

struct {0} {{
	using XY = xx::XY;
)#", xmlName);

	LabRetry:
		auto s = xx::Trim(xx::SplitOnce(sv, "\n"));
		if (s.starts_with("Name:")) {
			name = xx::Trim(s.substr(10));
		}
		else if (s.starts_with("Hull polygon:")) {
			data = xx::Trim(xx::SplitOnce(sv, "\n"));
			// replace () to {}
			for (auto& c : data) {
				if (c == '(') c = '{';
				else if (c == ')') c = '}';
			}
			xx::AppendFormat(h, R"#(
	static constexpr XY {0}[] {{ {1} };)#", name, data);
		}
		if (!sv.empty()) goto LabRetry;

		xx::Append(h, R"#(
};
)#");

#if 1
		// save to file
		{
			auto outPath = fullPath.substr(0, fullPath.size() - 4) + ".h";
			if (int r = xx::WriteAllBytes((std::u8string&)outPath, h.data(), h.size())) {
				std::cerr << "write file failed! r = " << r << std::endl;
				return __LINE__;
			}
		}
#else
		xx::CoutN(h);
#endif
	}

	xx::CoutN("finished! press ENTER to continue...");
	std::cin.get();

	return 0;
}
