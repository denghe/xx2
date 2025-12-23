#include <pch.h>

/*
* template
* XMLFileName.h

#pragma once
#include "pch.h"
struct XMLFileName {
	static constexpr XY xxxPolygonData[] { {?,?}, {?,?}, ... };
	static constexpr XY xxxPolygonData[] { {?,?}, {?,?}, ... };
	...
};


XML content example:

<?xml version="1.0" encoding="UTF-8"?>
<!-- created with http://www.physicseditor.de -->
<bodydef version="1.0">
	<bodies>
		<body name="10">
			<anchorpoint>0.5000,0.5000</anchorpoint>
			<fixtures>
				<fixture>
					<density>2</density>
					<friction>0</friction>
					<restitution>0</restitution>
					<filter_categoryBits>1</filter_categoryBits>
					<filter_groupIndex>0</filter_groupIndex>
					<filter_maskBits>65535</filter_maskBits>
					<fixture_type>POLYGON</fixture_type>
					<isSensor/>
					<polygons>
						<polygon>  44.0000, -189.0000  ,  68.0000, -185.0000  ,  ...... </polygon>
						......
					</polygons>
				</fixture>
				<fixture>
					<density>2</density>
					<friction>0</friction>
					<restitution>0</restitution>
					<filter_categoryBits>1</filter_categoryBits>
					<filter_groupIndex>0</filter_groupIndex>
					<filter_maskBits>65535</filter_maskBits>
					<fixture_type>CIRCLE</fixture_type>
					<circle r="20.000" x="247.728" y="-98.000"/>
				</fixture>
				......
			</fixtures>
		</body>
		......
	</bodies>
	<metadata>
		<format>1</format>
		<ptm_ratio>32</ptm_ratio>
	</metadata>
</bodydef>
*/

int main() {
	SetConsoleOutputCP(CP_UTF8);
	auto&& cp = std::filesystem::current_path();
	std::cout << "tool: PhysicsEditor's Box2d XML store file -> *.h\n\nworking dir: " << cp.string() << R"#(
press ENTER to continue...)#";
	std::cin.get();

	for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
		if (!entry.is_regular_file()) continue;
		auto&& p = entry.path();
		if (p.extension() != u8".xml") continue;

		auto fileName = xx::U8AsString(p.filename().u8string());
		auto fullPath = xx::U8AsString(std::filesystem::absolute(p).u8string());

		xx::Data fd;
		if (int r = xx::ReadAllBytes(p, fd)) {
			std::cerr << "ReadAllBytes failed. r = " << r << " fn = " << p << std::endl;
			return -__LINE__;
		}

		std::cout << "\nhandle file: " << fullPath << std::endl;

		std::string code;
		static_assert(sizeof(pugi::char_t) == 1);
		
		pugi::xml_document doc;
		if (auto&& r = doc.load_buffer(fd.buf, fd.len); r.status) { std::cerr << "load_buffer error : " << r.description() << std::endl; return __LINE__; }

		auto bodydef = doc.find_child([](auto& node_) { return strcmp(node_.name(), "bodydef") == 0; });
		if (bodydef.empty()) { std::cerr << "can't find <bodydef>\n"; return __LINE__; }

		auto version = bodydef.attribute("version"sv).value();
		if (version != "1.0"sv) { std::cerr << "<bodydef version is wrong?\n"; return __LINE__; }

		auto bodies = bodydef.find_child([](auto& node_) { return strcmp(node_.name(), "bodies") == 0; });
		if (bodydef.empty()) { std::cerr << "can't find <bodydef><bodies>\n"; return __LINE__; }

		auto bodies_children = bodies.children();
		for (auto body = bodies_children.begin(); body != bodies_children.end(); ++body) {
			if(strcmp(body->name(), "body")) { std::cerr << "<bodydef><bodies>'s children 's name isn't <body> ??\n"; return __LINE__; }

			auto anchorpoint = body->find_child([](auto& node_) { return strcmp(node_.name(), "anchorpoint") == 0; });
			if (anchorpoint.empty()) { std::cerr << "can't find <anchorpoint> in <bodydef><bodies><body>'s children\n"; return __LINE__; }

			auto fixtures = body->find_child([](auto& node_) { return strcmp(node_.name(), "fixtures") == 0; });
			if (fixtures.empty()) { std::cerr << "can't find <fixtures> in <bodydef><bodies><body>'s children\n"; return __LINE__; }

			auto fixtures_children = fixtures.children();
			for (auto fixture = fixtures_children.begin(); fixture != fixtures_children.end(); ++fixture) {

				auto density = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "density") == 0; });
				if (density.empty()) { std::cerr << "can't find <density> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				auto density_value = density.text().as_float();

				auto friction = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "friction") == 0; });
				if (friction.empty()) { std::cerr << "can't find <friction> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				auto friction_value = friction.text().as_float();

				auto restitution = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "restitution") == 0; });
				if (restitution.empty()) { std::cerr << "can't find <restitution> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				auto restitution_value = restitution.text().as_float();

				auto filter_categoryBits = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "filter_categoryBits") == 0; });
				if (filter_categoryBits.empty()) { std::cerr << "can't find <filter_categoryBits> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				auto filter_categoryBits_value = filter_categoryBits.text().as_uint();

				auto filter_groupIndex = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "filter_groupIndex") == 0; });
				if (filter_groupIndex.empty()) { std::cerr << "can't find <filter_groupIndex> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				auto filter_groupIndex_value = filter_groupIndex.text().as_uint();

				auto filter_maskBits = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "filter_maskBits") == 0; });
				if (filter_maskBits.empty()) { std::cerr << "can't find <filter_maskBits> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				auto filter_maskBits_value = filter_maskBits.text().as_uint();

				auto isSensor = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "isSensor") == 0; });
				bool isSensor_value = !isSensor.empty();

				auto fixture_type = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "fixture_type") == 0; });
				if (fixture_type.empty()) { std::cerr << "can't find <fixture_type> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				std::string_view fixture_type_value = fixture_type.text().as_string();
				xx::CoutN(fixture_type_value);
				if (fixture_type_value == "POLYGON"sv) {
					auto polygons = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "polygons") == 0; });
					if (polygons.empty()) { std::cerr << "can't find <polygons> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
					auto polygons_children = polygons.children();
					for (auto polygon = polygons_children.begin(); polygon != polygons_children.end(); ++polygon) {
						if (strcmp(polygon->name(), "polygon")) { std::cerr << "<bodydef><bodies><body><fixtures><fixture><polygons>'s children 's name isn't <polygon> ??\n" << std::endl; return __LINE__; }
						std::string_view sv = polygon->text().as_string();
						//xx::CoutN(sv);
						std::string_view x, y;
						//xx::XY p;
						do {
							x = xx::Trim(xx::SplitOnce(sv, ","));
							if (!x.size()) { std::cerr << "<polygon> data </polygon> read error\n"; return __LINE__; }
							//p.x = xx::SvToNumber(s, 0.f);
							y = xx::Trim(xx::SplitOnce(sv, ","));
							if (!y.size()) { std::cerr << "<polygon> data </polygon> read error\n"; return __LINE__; }
							//p.y = xx::SvToNumber(s, 0.f);
							// todo
							//xx::CoutN(x, " ", y);
						} while (sv.size());
					}
					// todo
				}
				else if (fixture_type_value == "CIRCLE") {
					auto circle = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "circle") == 0; });
					if (circle.empty()) { std::cerr << "can't find <circle> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
					std::string_view r = circle.attribute("r").as_string();
					std::string_view x = circle.attribute("x").as_string();
					std::string_view y = circle.attribute("y").as_string();
					// todo
					xx::CoutN(r, " ", x, " ", y);
				}
				else {
					std::cerr << "wrong fixture_type.value:" << fixture_type_value << std::endl;
					return __LINE__;
				}
			}
		}

		auto metadata = bodies.next_sibling();
		assert(metadata.name() == "metadata"sv);
		//auto format = metadata


#if 0
		// save to file
		auto outPath = fullPath.substr(0, fullPath.size() - 4) + ".h";
		if (int r = xx::WriteAllBytes((std::u8string&)outPath, code.data(), code.size())) {
			std::cerr << "write file failed! r = " << r << std::endl;
			return -__LINE__;
		}
#endif
	}

	xx::CoutN("finished! press ENTER to continue...");
	std::cin.get();

	return 0;
}
