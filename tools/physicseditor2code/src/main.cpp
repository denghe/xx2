#include <pch.h>

/*
* template
* XMLFileName.h

#pragma once
#include "pch.h"	// #include <xx_box2d.h>  using XY = xx::XY;

struct XMLFileName {
	static constexpr float ptm_ratio{ ? };
	struct {
		static constexpr XY anchorpoint{ ?,? };
		struct {
			static constexpr float density{ ? };
			static constexpr float friction{ ? };
			static constexpr float restitution{ ? };
			static constexpr uint32_t filter_categoryBits{ ? };
			static constexpr uint32_t filter_groupIndex{ ? };
			static constexpr uint32_t filter_maskBits{ ? };
			static constexpr bool isSensor{ ? };
			static constexpr XY polygons1[] { {?,?}, {?,?}, ... };
			static constexpr XY polygons2[] { {?,?}, {?,?}, ... };
			......
		} fixture1;
		struct {
			static constexpr float density{ ? };
			static constexpr float friction{ ? };
			static constexpr float restitution{ ? };
			static constexpr uint32_t filter_categoryBits{ ? };
			static constexpr uint32_t filter_groupIndex{ ? };
			static constexpr uint32_t filter_maskBits{ ? };
			static constexpr bool isSensor{ ? };
			static constexpr float r{ ? }, x{ ? }, y{ ? };	// CIRCLE
		} fixture2;
		......
		void Init(b2BodyId const& id_, float radius_ = 1.f);
	} xxx;
	......
};

* template
* XMLFileName.cpp

#include "pch.h"
#include "XMLFileName.h"

void ::XMLFileName::xxx::Init(b2BodyId const& id_, float radius_) {
	auto def = b2DefaultShapeDef();
	// fixture1
	{
		def.density = ::XMLFileName::xxx::fixture1::density;
		def.material.friction = ::XMLFileName::xxx::fixture1::friction;
		def.material.restitution = ::XMLFileName::xxx::fixture1::restitution;
		// todo: filter_categoryBits  filter_groupIndex  filter_maskBits  isSensor

		{
			auto hull = b2ComputeHull((b2Vec2*)::XMLFileName::xxx::polys.buf, _countof(::XMLFileName::xxx::polys.buf));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		}
		......
	}
	......
}

......




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
	std::cout << "tool: PhysicsEditor's Box2d XML store file -> *.h & .cpp\n\nworking dir: " << cp.string() << R"#(
press ENTER to continue...)#";
	std::cin.get();

	for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
		if (!entry.is_regular_file()) continue;
		auto&& p = entry.path();
		if (p.extension() != u8".xml") continue;

		auto fileName = xx::U8AsString(p.filename().u8string());
		auto fullPath = xx::U8AsString(std::filesystem::absolute(p).u8string());
		auto xmlName = fileName.substr(0, fileName.size() - 4);

		xx::Data fd;
		if (int r = xx::ReadAllBytes(p, fd)) {
			std::cerr << "ReadAllBytes failed. r = " << r << " fn = " << p << std::endl;
			return __LINE__;
		}

		std::cout << "\nhandle file: " << fullPath << std::endl;

		std::string h, cpp;
		static_assert(sizeof(pugi::char_t) == 1);

		xx::Append(h, R"#(#pragma once
#include "pch.h"	// #include <xx_box2d.h>  using XY = xx::XY;

struct {)#");
		
		xx::AppendFormat(cpp, R"#(#include "pch.h"
#include "{0}.h"
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

		xx::AppendFormat(h, R"#(
	static constexpr float ptm_ratio{{ {0} };		// need call b2SetLengthUnitsPerMeter(  ????  );
)#", ptm_ratio_value);

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
			xx::AppendFormat(h, R"#(
	struct {{
		static constexpr XY anchorpoint{{ {0} };)#", anchorpoint_value);

			auto fixtures = body->find_child([](auto& node_) { return strcmp(node_.name(), "fixtures") == 0; });
			if (fixtures.empty()) { std::cerr << "can't find <fixtures> in <bodydef><bodies><body>'s children\n"; return __LINE__; }

			xx::Append(h, R"#(
		struct {)#");

			xx::AppendFormat(cpp, R"#(
void ::{0}::{1}::Init(b2BodyId const& id_, float radius_) {{
	auto def = b2DefaultShapeDef();)#", xmlName, name);

			int j{1};
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

				xx::AppendFormat(h, R"#(
			static constexpr float density{{ {0} };
			static constexpr float friction{{ {1} };
			static constexpr float restitution{{ {2} };
			static constexpr uint32_t filter_categoryBits{{ {3} };
			static constexpr uint32_t filter_groupIndex{{ {4} };
			static constexpr uint32_t filter_maskBits{{ {5} };
			static constexpr bool isSensor{{ {6} };)#"
					, density_value
					, friction_value
					, restitution_value
					, filter_categoryBits_value
					, filter_groupIndex_value
					, filter_maskBits_value
					, isSensor_value ? "true":"false"
				);

				xx::AppendFormat(cpp, R"#(
	// fixture{0}
	{{
		def.density = ::{1}::{2}::fixture{0}::density;
		def.material.friction = ::{1}::{2}::fixture{0}::friction;
		def.material.restitution = ::{1}::{2}::fixture{0}::restitution;)#"
					, j, xmlName, name);
		// todo: filter_categoryBits  filter_groupIndex  filter_maskBits  isSensor

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

						++i;
						xx::AppendFormat(h, R"#(
			static constexpr XY polygons{0}[] {{)#", i);

						std::string_view sv = polygon->text().as_string();
						do {
							auto x = xx::SvToNumber( xx::Trim(xx::SplitOnce(sv, ",")), 0.f) / ptm_ratio_value;
							auto y = xx::SvToNumber( xx::Trim(xx::SplitOnce(sv, ",")), 0.f) / ptm_ratio_value;
							xx::AppendFormat(h, "{{ {0}, {1} }, ", x, -y);	// flip y
						} while (sv.size());
						h.resize(h.size() - 2);	// remove last ", "
						xx::Append(h, "};");

						xx::AppendFormat(cpp, R"#(
		{{
			auto hull = b2ComputeHull((b2Vec2*)::{1}::{2}::fixture{0}::polygons{3}, _countof(::{1}::{2}::fixture{0}::polygons{3}));
			auto polygon = b2MakePolygon(&hull, radius_);
			b2CreatePolygonShape(id_, &def, &polygon);
		})#", j, xmlName, name, i);

					}


				}
				else if (fixture_type_value == "CIRCLE") {
					auto circle = fixture->find_child([](auto& node_) { return strcmp(node_.name(), "circle") == 0; });
					if (circle.empty()) { std::cerr << "can't find <circle> in <bodydef><bodies><body><fixtures><fixture>'s children\n"; return __LINE__; }
					std::string_view r = circle.attribute("r").as_string();
					std::string_view x = circle.attribute("x").as_string();
					std::string_view y = circle.attribute("y").as_string();
					// todo
					xx::AppendFormat(h, R"#(
			static constexpr float circleRXY[] {{ {0}, {1}, {2} })#", r, x, y);
					xx::AppendFormat(cpp, R"#(
		{{
			auto circle = b2Circle{{ .center = {{ ::{1}::{2}::fixture{0}::circleRXY[1], ::{1}::{2}::fixture{0}::circleRXY[2] }, .radius = ::{1}::{2}::fixture{0}::circleRXY[0] };
			b2CreateCircleShape(b2body, &def, &circle);
		})#", j, xmlName, name);
				}
				else {
					std::cerr << "wrong fixture_type.value:" << fixture_type_value << std::endl;
					return __LINE__;
				}

				xx::AppendFormat(h, R"#(
		} fixture{0};)#", j);
				xx::Append(cpp, R"#(
	})#");
				++j;
			}

			xx::AppendFormat(h, R"#(
	} {0};)#", name);
			xx::Append(cpp, R"#(
})#");
		}

		xx::AppendFormat(h, R"#(
} {0};
)#", xmlName);

#if 1
		// save to file
		{
			auto outPath = fullPath.substr(0, fullPath.size() - 4) + ".h";
			if (int r = xx::WriteAllBytes((std::u8string&)outPath, h.data(), h.size())) {
				std::cerr << "write file failed! r = " << r << std::endl;
				return __LINE__;
			}
		}
		{
			auto outPath = fullPath.substr(0, fullPath.size() - 4) + ".cpp";
			if (int r = xx::WriteAllBytes((std::u8string&)outPath, cpp.data(), cpp.size())) {
				std::cerr << "write file failed! r = " << r << std::endl;
				return __LINE__;
			}
		}
#else
		xx::CoutN(h);
		xx::CoutN(cpp);
#endif
	}

	xx::CoutN("finished! press ENTER to continue...");
	std::cin.get();

	return 0;
}
