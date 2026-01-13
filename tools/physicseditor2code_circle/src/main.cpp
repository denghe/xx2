#include <pch.h>
#include <iostream>

/*
* template
* XMLFileName.h

#pragma once
#include "pch.h"

struct XMLFileName {
	struct Circle {
		xx::XY pos;
		float r;
	};

	static constexpr std::array<Circle, ?> xxx{ {{ ?, ? }, ?}, ...... };
	......
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

struct Circle {
	float r, x, y;
};

int main() {
	SetConsoleOutputCP(CP_UTF8);
	auto&& cp = std::filesystem::current_path();
	std::cout << "tool: PhysicsEditor's Box2d XML store file ( only circles ) -> *.h\n\nworking dir: " << cp.string() << R"#(
press ENTER to continue...)#";
	std::cin.get();

	for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
		if (!entry.is_regular_file()) continue;
		auto&& p = entry.path();
		if (p.extension() != u8".xml") continue;

		auto fileName = xx::U8AsString(p.filename().u8string());
		auto fullPath = xx::U8AsString(std::filesystem::absolute(p).u8string());
		auto xmlName = fileName.substr(0, fileName.size() - 4);

		auto fd = xx::ReadAllBytes(p);
		if (!fd) {
			std::cerr << "ReadAllBytes failed: " << p << std::endl;
			return __LINE__;
		}

		std::cout << "\nhandle file: " << fullPath << std::endl;

		std::string h;// , cpp;
		static_assert(sizeof(pugi::char_t) == 1);

		xx::AppendFormat(h, R"#(#pragma once
#include "pch.h"

struct {0} {{
	struct Circle {{
		xx::XY pos;
		float r;
	};
)#", xmlName);
		
		pugi::xml_document doc;
		if (auto&& r = doc.load_buffer(fd.buf, fd.len); r.status) { std::cerr << "load_buffer error : " << r.description() << std::endl; return __LINE__; }

		auto bodydef = doc.find_child([](auto& node_) { return strcmp(node_.name(), "bodydef") == 0; });
		if (bodydef.empty()) { std::cerr << "can't find <bodydef>\n"; return __LINE__; }

		auto version = bodydef.attribute("version"sv).value();
		if (version != "1.0"sv) { std::cerr << "<bodydef version is wrong?\n"; return __LINE__; }

		auto metadata = bodydef.find_child([](auto& node_) { return strcmp(node_.name(), "metadata") == 0; });
		if (metadata.empty()) { std::cerr << "can't find <bodydef><metadata>\n"; return __LINE__; }

		auto format = metadata.find_child([](auto& node_) { return strcmp(node_.name(), "format") == 0; });
		if (format.empty()) { std::cerr << "can't find <bodydef><metadata><format>\n"; return __LINE__; }
		if (format.text().as_int() != 1) { std::cerr << "bad <bodydef><metadata><format> version. should be 1.\n"; return __LINE__; }
		auto ptm_ratio = metadata.find_child([](auto& node_) { return strcmp(node_.name(), "ptm_ratio") == 0; });
		if (ptm_ratio.empty()) { std::cerr << "can't find <bodydef><metadata><ptm_ratio>\n"; return __LINE__; }
		auto ptm_ratio_value = ptm_ratio.text().as_float();

		auto bodies = bodydef.find_child([](auto& node_) { return strcmp(node_.name(), "bodies") == 0; });
		if (bodydef.empty()) { std::cerr << "can't find <bodydef><bodies>\n"; return __LINE__; }

		auto bodies_children = bodies.children();
		for (auto body = bodies_children.begin(); body != bodies_children.end(); ++body) {
			if(strcmp(body->name(), "body")) { std::cerr << "<bodydef><bodies>'s children 's name isn't <body> ??\n"; return __LINE__; }
			
			std::string name = body->attribute("name").as_string();
			if (!name.size()) { std::cerr << "<bodydef><bodies><body name= empty ??\n"; return __LINE__; }
			if (name[0] >= '0' && name[0] <= '9') {
				name = "_" + name;
			}

			auto anchorpoint = body->find_child([](auto& node_) { return strcmp(node_.name(), "anchorpoint") == 0; });
			if (anchorpoint.empty()) { std::cerr << "can't find <anchorpoint> in <bodydef><bodies><body>'s children\n"; return __LINE__; }
			std::string_view anchorpoint_value = anchorpoint.text().as_string();

			auto fixtures = body->find_child([](auto& node_) { return strcmp(node_.name(), "fixtures") == 0; });
			if (fixtures.empty()) { std::cerr << "can't find <fixtures> in <bodydef><bodies><body>'s children\n"; return __LINE__; }

			xx::List<Circle> circles;
			auto fixtures_children = fixtures.children();
			for (auto fixture = fixtures_children.begin(); fixture != fixtures_children.end(); ++fixture) {

				auto density = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "density") == 0; });
				if (density.empty()) { std::cerr << "can't find <density> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				std::string_view density_value = density.text().as_string();

				auto friction = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "friction") == 0; });
				if (friction.empty()) { std::cerr << "can't find <friction> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				std::string_view friction_value = friction.text().as_string();

				auto restitution = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "restitution") == 0; });
				if (restitution.empty()) { std::cerr << "can't find <restitution> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				std::string_view restitution_value = restitution.text().as_string();

				auto filter_categoryBits = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "filter_categoryBits") == 0; });
				if (filter_categoryBits.empty()) { std::cerr << "can't find <filter_categoryBits> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				std::string_view filter_categoryBits_value = filter_categoryBits.text().as_string();

				auto filter_groupIndex = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "filter_groupIndex") == 0; });
				if (filter_groupIndex.empty()) { std::cerr << "can't find <filter_groupIndex> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				auto filter_groupIndex_value = filter_groupIndex.text().as_string();

				auto filter_maskBits = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "filter_maskBits") == 0; });
				if (filter_maskBits.empty()) { std::cerr << "can't find <filter_maskBits> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				auto filter_maskBits_value = filter_maskBits.text().as_string();

				auto isSensor = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "isSensor") == 0; });
				bool isSensor_value = !isSensor.empty();

				auto fixture_type = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "fixture_type") == 0; });
				if (fixture_type.empty()) { std::cerr << "can't find <fixture_type> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
				std::string_view fixture_type_value = fixture_type.text().as_string();
				if (fixture_type_value == "POLYGON"sv) {
					auto polygons = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "polygons") == 0; });
					if (polygons.empty()) { std::cerr << "can't find <polygons> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
					auto polygons_children = polygons.children();
					int i{};
					for (auto polygon = polygons_children.begin(); polygon != polygons_children.end(); ++polygon) {
						if (strcmp(polygon->name(), "polygon")) { std::cerr << "<bodydef><bodies><body><fixtures><fixture><polygons>'s children 's name isn't <polygon> ??\n" << std::endl; return __LINE__; }
						// ignore ?
					}
				}
				else if (fixture_type_value == "CIRCLE") {
					auto circle = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "circle") == 0; });
					if (circle.empty()) { std::cerr << "can't find <circle> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
					auto r = circle.attribute("r").as_float();
					auto x = circle.attribute("x").as_float();
					auto y = circle.attribute("y").as_float();
					circles.Emplace(r, x, y);
				}
				else {
					std::cerr << "wrong fixture_type.value:" << fixture_type_value << std::endl;
					return __LINE__;
				}
			}

			if (circles.Empty()) continue;
			xx::Append(h, R"#(
	static constexpr std::array<Circle, )#", circles.len, "> ", name, " { Circle");
			for (auto& c : circles) {
				xx::Append(h, "{{ ", c.x, ", ", c.y, " }, ", c.r, "}, ");
			}
			h.resize(h.size() - 2);
			xx::Append(h, "};");
		}

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
