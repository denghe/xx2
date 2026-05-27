#include "pch.h"
#include "main.h"

Game gg;

int32_t main() {
	return gg.Run();
}

void Game::Init() {
	title = "curve editor( workDir/curves.json, export to bin,h,cpp )";
	windowSize = designSize = cDesignSize;
}

void Game::Delay() {
	SleepSecs(cDelta - (glfwGetTime() - time));	// for power saving
}

void Game::GLInit() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(wnd, true);
	ImGui_ImplOpenGL3_Init("#version 300 es");

	io.Fonts->ClearFonts();
	ImFontConfig cfg;
	cfg.SizePixels = 26.f;
	auto&& imfnt = io.Fonts->AddFontDefault(&cfg);
	// auto&& imfnt = io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 24, {}, io.Fonts->GetGlyphRangesChineseFull());
	io.Fonts->Build();
	io.FontDefault = imfnt;
	io.IniFilename = nullptr;

	ui.Emplace()->InitRoot(scale);
	(uiText1 = ui->Make<xx::Label>())->Init(2, p7 + XY{ 500, 0 }, a7, 32.f);
	(uiText2 = ui->Make<xx::Label>())->Init(2, p7 + XY{ 500, -30 }, a7, 32.f);
	uiText1->SetText("Arrow Up/Down: move line; Z/X: Zoom in/out;  W/S: select prev/next line;");
	uiText2->SetText("A/D: add/remove selected or last point; mouse: select & drag; go: matrix translate");
	fb.Init();
	lsPoint.FillCirclePoints({}, pointRadius, {}, 16);
	meListener.Init(GLFW_MOUSE_BUTTON_1);
	LoadData();
	exportFileName = gg.GetFullPath("res", false) + "/curves";	// do not include extension
	MakeNewLineName();
}

void Game::OnResize(bool modeChanged_) {
	uiText1->position = p7 + XY{ 500, 0 };
	uiText2->position = p7 + XY{ 500, -30 };
	ui->Resize(scale);
}

void Game::Update() {
	// imgui prepare
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// imgui content fill
	do {
		// logic update & draw
		if (!msg.has_value() && !exporting) {
			UpdateLogic();
		}
		DrawNode(ui);

		if (msg.has_value()) {
			ImGuiDrawWindow_Error();
			break;
		}
		if (exporting) {
			ImGuiDrawWindow_Export();
			break;
		}
		ImGuiDrawWindow_LeftTop0();
		ImGuiDrawWindow_LeftTop();
		ImGuiDrawWindow_LeftBottom0();
		ImGuiDrawWindow_LeftBottom();
		// ...
	} while (false);
	// imgui commit
	ImGui::Render();

	// imgui draw
	if (auto d = ImGui::GetDrawData()) {
		ImGui_ImplOpenGL3_RenderDrawData(d);
	}
}

void Game::OnExit() {
	auto&& io = ImGui::GetIO();
	io.Fonts->ClearFonts();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Game::DrawIcon(MovePathStore::Line& line) {
	cps.clear();
	for (int32_t i = 0, e = line.points.size(); i < e; ++i) {
		auto& p = line.points[i];
		xx::XY pos{ (float)p.x, -(float)p.y };	// flip y for render to texture
		cps.emplace_back(pos, (float)p.tension / 100.f, std::max((int32_t)p.numSegments, 10));
	}
	mp.Clear();
	mp.FillCurve(line.isLoop, cps);

	if (mp.totalDistance > 1.f) {
		mpc.Init(mp);
		line.tex = fb.Draw({ 48,32 }, true, xx::RGBA8{}, [&] {
			gg.flipY = 1;	// do not flip y for imgui
			auto& ps = ls.Clear().SetPoints();
			for (auto& p : mpc.points) {
				ps.emplace_back(p.pos);
				if (ps.size() == 65535) {		// batch commit
					ls.Draw();
					ls.Clear();
					ps.emplace_back(p.pos);
				}
			}
			ls.Draw();
		});
	}
}

void Game::LoadData() {
	if (auto p = gg.GetFullPath("curves.json"sv); p.empty()) {
		// todo: create default data and save to file
		//p = 
		auto cp = std::filesystem::current_path();
		cp /= "curves.json";
		fileName = cp.string();
		std::string_view s = R"##({"designWidth":1920,"designHeight":1080,"safeLength":128,"lines":[]})##";
		data = {};
		ajson::load_from_buff(data, s.data(), s.size());
	}
	else {
		auto d = gg.LoadFileDataWithFullPath(p);
		fileName = std::move(p);
		data = {};
		ajson::load_from_buff(data, (char*)d.buf, d.len);
	}

	ls.SetPosition({}).SetScale(0.016).SetColor({ 255,255,127,255 });
	for (auto& line : data.lines) {
		if (line.points.size() < 2) {
			line.tex.Reset();
			continue;
		}
		DrawIcon(line);
	}
}

void Game::SaveData() {
	ajson::save_to_file(data, fileName.c_str());
	LoadData();
}

std::string Game::ReplaceName(std::string name_) {
	std::string name(name_);
	for (auto& c : name) {
		if (c == ' ' || c == '?' || c == '<' || c == '>' || c == '.'
			|| c == '+' || c == '-' || c == '*' || c == '/'
			) {
			c = '_';
		}
	}
	return name;
}

void Game::ExportData() {
	auto idx = exportFileName.find_last_of('/');
	if (idx == std::string::npos) {
		msg = xx::ToString("invalid export file name: ", exportFileName);
		return;
	}
	auto fn = exportFileName.substr(idx + 1);
	auto cn = ReplaceName(fn);

	// export .h
	std::string code;
	xx::AppendFormat(code, R"#(#include <xx_curvemovepath.h>

struct {0} {{
)#", cn);
	int index = 0;
	for(auto& line : data.lines) {
		xx::AppendFormat(code, R"#(
	xx::MovePathCache {0}; // {1}: {2})#", ReplaceName(line.name), index++, line.name);
	}

	xx::AppendFormat(code, R"#(

	uint32_t designWidth{{{0}}, designHeight{{{1}}, safeLength{{{2}};

	// fill contents
	void Init(float stepDistance_ = 1.f);

	// array access
	xx::MovePathCache& operator[](size_t index_);
};
)#", data.designWidth, data.designHeight, data.safeLength);

	auto r = xx::WriteAllBytes(exportFileName + ".h", (char*)code.data(), code.size());
	if (r) {
		msg = xx::ToString("WriteAllBytes to file: ", exportFileName, " error! r = ", r);
		return;
	}

	// export .cpp
	code.clear();
	xx::AppendFormat(code, R"#(#include "pch.h"
#include "{0}.h"

void {0}::Init(float stepDistance_) {{
	std::vector<xx::CurvePoint> tmpCps;
	xx::MovePath tmpMp;
)#", cn);
	for (auto& line : data.lines) {
		xx::AppendFormat(code, R"#(
	// {0};
	tmpCps.clear();)#", line.name);
		for (auto& p : line.points) {
			xx::AppendFormat(code, R"#(
	tmpCps.push_back({{ {{{0}, {1}}, {2}, {3} });)#", p.x, p.y, p.tension / 100.f, p.numSegments);
		}
		xx::AppendFormat(code, R"#(
	tmpMp.Clear();
	tmpMp.FillCurve({0}, tmpCps);
	this->{1}.Init(tmpMp, stepDistance_);
)#", line.isLoop ? "true" : "false", ReplaceName(line.name));
	}
	xx::AppendFormat(code, R"#(
}

xx::MovePathCache& {0}::operator[](size_t index_) {{
	assert(index_ < {1});
	return ((xx::MovePathCache*)this)[index_];
}
)#", cn, data.lines.size());
	r = xx::WriteAllBytes(exportFileName + ".cpp", (char*)code.data(), code.size());
	if (r) {
		msg = xx::ToString("WriteAllBytes to file: ", exportFileName, " error! r = ", r);
		return;
	}

	// export .bin
	// format: lines size + Line{ name size + name content + 1 byte isloop + points size + Point{ x,y,t,n }... }...
	xx::Data d;
	d.Write(data);
	r = xx::WriteAllBytes(exportFileName + ".bin", (char*)d.buf, d.len);
	if (r) {
		msg = xx::ToString("WriteAllBytes to file: ", exportFileName, " error! r = ", r);
		return;
	}

	// success
	msg = xx::ToString("export success!");
	exporting = false;
}

void Game::ImGuiDrawWindow_Error() {
	ImVec2 p = ImGui::GetMainViewport()->Pos;
	p.x += (gg.windowSize.x - errPanelSize.x) / 2;
	p.y += (gg.windowSize.y - errPanelSize.y) / 2;
	ImGui::SetNextWindowPos(p);
	ImGui::SetNextWindowSize(ImVec2(errPanelSize.x, errPanelSize.y));
	ImGui::Begin("dialog", nullptr, ImGuiWindowFlags_NoMove |
		//ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize);

	ImGui::Text(msg->c_str());

	ImGui::Dummy({ 0.0f, 62.0f });
	ImGui::Separator();
	ImGui::Dummy({ 0.0f, 5.0f });

	ImGui::Dummy({ 0.0f, 0.0f });
	ImGui::SameLine(ImGui::GetWindowWidth() - (ImGui::GetStyle().ItemSpacing.x + 90 + 10));
	if (ImGui::Button("close", { 90, 35 })) {
		msg.reset();
	}
	ImGui::End();
}

void Game::ImGuiDrawWindow_Export() {
	ImVec2 p = ImGui::GetMainViewport()->Pos;
	p.x += (gg.windowSize.x - exportPanelSize.x) / 2;
	p.y += (gg.windowSize.y - exportPanelSize.y) / 2;
	ImGui::SetNextWindowPos(p);
	ImGui::SetNextWindowSize(ImVec2(exportPanelSize.x, exportPanelSize.y));
	ImGui::Begin("export", nullptr, ImGuiWindowFlags_NoMove |
		//ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize);

	ImGui::Text("please input the output file name:");
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##exportFileName", &exportFileName);

	ImGui::Dummy({ 0.0f, 26.0f });
	ImGui::Separator();
	ImGui::Dummy({ 0.0f, 5.0f });

	ImGui::Dummy({ 0.0f, 0.0f });
	ImGui::SameLine(ImGui::GetWindowWidth() - (ImGui::GetStyle().ItemSpacing.x + 120 + 10 + 120));
	if (ImGui::Button("cancel", { 120, 35 })) {
		exporting = false;
	}
	ImGui::SameLine(ImGui::GetWindowWidth() - (ImGui::GetStyle().ItemSpacing.x + 120));
	if (ImGui::Button("export", { 120, 35 })) {
		ExportData();
	}

	ImGui::End();
}


void Game::ImGuiDrawWindow_LeftTop0() {
	ImVec2 p = ImGui::GetMainViewport()->Pos;
	p.x += margin;
	p.y += margin;
	ImGui::SetNextWindowPos(p);
	ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, leftCmdPanelHeight1));

	ImGui::Begin("linescmds", nullptr, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize);
	
	auto&& sgBegin = xx::MakeSimpleScopeGuard([] {
		ImGui::End(); 
	});

	if (ImGui::Button("reload")) {
		LoadData();
		if (GetSelectedLineIndex() == -1) {
			selectedLineName.clear();
			ClearEditState();
		}
	}
	ImGui::SameLine({}, 40);
	if (ImGui::Button("save")) {
		SaveData();
	}
	ImGui::SameLine({}, 38);
	if (ImGui::Button("export")) {
		exporting = true;
	}
	if (selectedLineName.size()) {
		ImGui::SameLine({}, 60);
		if (ImGui::Button( "clone")) {
			if (CreateNewLine()) {
				auto&& l = GetSelectedLine();
				auto&& t = data.lines.back();
				t.isLoop = l->isLoop;
				t.points = l->points;
				SelectLine(t.name);
			}
		}
	}

	ImGui::Text("%s", "name:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(286);
	ImGui::InputText("##newLineName", &newLineName);
	ImGui::SameLine();
	if (ImGui::Button("create")) {
		if (CreateNewLine()) {
			SelectLine(data.lines.back().name);
		}
	}

}

void Game::ImGuiDrawWindow_LeftTop() {
	ImVec2 p = ImGui::GetMainViewport()->Pos;
	p.x += margin;
	p.y += margin + leftCmdPanelHeight1;
	auto&& h = (gg.windowSize.y - margin * 3 - (leftCmdPanelHeight1 + leftCmdPanelHeight2)) / 2;
	ImGui::SetNextWindowPos(p);
	ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, h));
	ImGui::Begin("lines", nullptr, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize);

	constexpr int32_t numCols = 3;
	if (ImGui::BeginTable("linesstable", numCols, {}, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 7))) {
		ImGui::TableSetupColumn("select", ImGuiTableColumnFlags_WidthFixed, 60);
		ImGui::TableSetupColumn("line name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("delete", ImGuiTableColumnFlags_WidthFixed, 32);
		ImGui::TableHeadersRow();

		int32_t rowId{}, removeRowId{ -1 };
		for (auto& p : data.lines) {

			ImGui::TableNextRow();

			int32_t n = 0;

			ImGui::TableSetColumnIndex(n);
			ImGui::PushID(rowId * numCols + n);
			ImGui::PushStyleColor(ImGuiCol_Button, p.name == selectedLineName ? pressColor : normalColor);
			auto&& sg = xx::MakeScopeGuard([] { ImGui::PopStyleColor(1); });

			if (p.tex) {
				if (ImGui::ImageButton(p.name.data(), ImTextureID((GLuint)*p.tex), {48,32})) {
					SelectLine(p.name);
				}
			} else {
				if (ImGui::Button("==>")) {
					SelectLine(p.name);
				}
			}
			ImGui::PopID();

			++n;
			ImGui::TableSetColumnIndex(n);
			ImGui::PushID(rowId * numCols + n);
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::Text(p.name.c_str());
			ImGui::PopID();

			++n;
			ImGui::TableSetColumnIndex(n);
			ImGui::PushID(rowId * numCols + n);
			if (ImGui::Button("X")) {
				removeRowId = rowId;
			}
			ImGui::PopID();

			++rowId;
		}
		ImGui::EndTable();

		if (removeRowId >= 0) {
			auto&& iter = data.lines.begin() + removeRowId;
			if (iter->name == selectedLineName) {
				selectedLineName.clear();
				ClearEditState();
			}
			data.lines.erase(iter);
		}
	}

	ImGui::End();
}

void Game::ImGuiDrawWindow_LeftBottom0() {
	ImVec2 p = ImGui::GetMainViewport()->Pos;
	p.x += margin;
	auto&& h = (gg.windowSize.y - margin * 3 - (leftCmdPanelHeight1 + leftCmdPanelHeight2)) / 2;
	p.y += margin * 2 + leftCmdPanelHeight1 + h;
	ImGui::SetNextWindowPos(p);
	ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, leftCmdPanelHeight2));
	ImGui::Begin("pointscmds", nullptr, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize);

	if (selectedLineName.size()) {
		auto&& line = GetLineByName(selectedLineName);

		ImGui::SetNextItemWidth(296);
		ImGui::InputText("##newLineName2", &changeLineName);
		ImGui::SameLine();
		if (ImGui::Button("change name")) {
			if (changeLineName.empty()) {
				msg = "line name can't be null";
			}
			bool found = false;
			for (auto& l : data.lines) {
				if (&l == line) continue;
				if (l.name == changeLineName) {
					msg = "line name already exists";
					found = true;
					break;
				}
			}
			if (!found) {
				selectedLineName = line->name = changeLineName;
			}
		}


		ImGui::Checkbox("loop", &line->isLoop);
		ImGui::SameLine({}, 28);
		if (ImGui::Button("new point")) {
			line->points.emplace_back();
		}
		ImGui::SameLine({}, 28);
		if (ImGui::Button("flipX")) {
			for (auto& p : line->points) {
				p.x = -p.x;
			}
		}
		ImGui::SameLine({}, 28);
		if (ImGui::Button("flipY")) {
			for (auto& p : line->points) {
				p.y = -p.y;
			}
		}

		// offset
		ImGui::Text("o");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(65);
		ImGui::InputFloat("##afPosx", &afPos.x, {}, {}, "%.0f", ImGuiInputTextFlags_CharsDecimal);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(65);
		ImGui::InputFloat("##afPosy", &afPos.y, {}, {}, "%.0f", ImGuiInputTextFlags_CharsDecimal);
		ImGui::SameLine();
		// scale
		ImGui::Text("s");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(65);
		ImGui::InputFloat("##afScalex", &afScale.x, {}, {}, "%.2f", ImGuiInputTextFlags_CharsDecimal);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(65);
		ImGui::InputFloat("##afScaley", &afScale.y, {}, {}, "%.2f", ImGuiInputTextFlags_CharsDecimal);
		ImGui::SameLine();
		// rotate
		ImGui::Text("r");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(65);
		ImGui::InputFloat("##afAngle", &afAngle, {}, {}, "%.2f", ImGuiInputTextFlags_CharsDecimal);
		ImGui::SameLine();
		// affine matrix apply
		if (ImGui::Button("go")) {
			auto&& at = xx::AffineTransform::MakePosScaleRadians(afPos, afScale, afAngle * M_PI / 360.f);
			for (auto& p : line->points) {
				auto&& pos = at.Apply(xx::XY{ (float)p.x, (float)p.y });
				p.x = pos.x;
				p.y = pos.y;
			}
		}
	}

	ImGui::End();
}

void Game::ImGuiDrawWindow_LeftBottom() {
	ImVec2 p = ImGui::GetMainViewport()->Pos;
	p.x += margin;
	auto&& h = (gg.windowSize.y - margin * 3 - (leftCmdPanelHeight1 + leftCmdPanelHeight2)) / 2;
	p.y += margin * 2 + leftCmdPanelHeight1 + leftCmdPanelHeight2 + h;
	ImGui::SetNextWindowPos(p);
	ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, h));
	ImGui::Begin("points", nullptr, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize);

	if (selectedLineName.size()) {
		auto&& line = GetLineByName(selectedLineName);
		assert(line);

		constexpr int32_t numCols = 7;
		if (ImGui::BeginTable("pointstable", numCols, {}, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 7))) {

			ImGui::TableSetupColumn("select", ImGuiTableColumnFlags_WidthFixed, 32);
			ImGui::TableSetupColumn("insert", ImGuiTableColumnFlags_WidthFixed, 32);
			ImGui::TableSetupColumn("x", ImGuiTableColumnFlags_WidthFixed, 80);
			ImGui::TableSetupColumn("y", ImGuiTableColumnFlags_WidthFixed, 80);
			ImGui::TableSetupColumn("tension", ImGuiTableColumnFlags_WidthFixed, 70);
			ImGui::TableSetupColumn("numSegments", ImGuiTableColumnFlags_WidthFixed, 70);
			ImGui::TableSetupColumn("delete", ImGuiTableColumnFlags_WidthFixed, 32);
			ImGui::TableHeadersRow();

			int32_t rowId{}, removeRowId{ -1 };
			for (int32_t e = line->points.size(); rowId < e; ++rowId) {
				auto& p = line->points[rowId];

				ImGui::TableNextRow();
				ImGui::PushStyleColor(ImGuiCol_Button, selectedPointIdex == rowId ? pressColor : normalColor);

				int32_t n = 0;
				ImGui::TableSetColumnIndex(n);
				ImGui::PushID(rowId * numCols + n);
				if (ImGui::Button(">")) {
					selectedPointIdex = rowId;
				}
				ImGui::PopID();

				++n;
				ImGui::TableSetColumnIndex(n);
				ImGui::PushID(rowId * numCols + n);
				if (ImGui::Button("+")) {
					line->points.insert(line->points.begin() + rowId, MovePathStore::Point{});
					selectedPointIdex = -1;
				}
				ImGui::PopID();

				++n;
				ImGui::TableSetColumnIndex(n);
				ImGui::PushID(rowId * numCols + n);
				ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::InputInt("##", &p.x, 0, 0, ImGuiInputTextFlags_CharsDecimal);
				ImGui::PopID();

				++n;
				ImGui::TableSetColumnIndex(n);
				ImGui::PushID(rowId * numCols + n);
				ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::InputInt("##", &p.y, 0, 0, ImGuiInputTextFlags_CharsDecimal);
				ImGui::PopID();

				++n;
				ImGui::TableSetColumnIndex(n);
				ImGui::PushID(rowId * numCols + n);
				ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::InputInt("##", &p.tension, 0, 0, ImGuiInputTextFlags_CharsDecimal);
				ImGui::PopID();

				++n;
				ImGui::TableSetColumnIndex(n);
				ImGui::PushID(rowId * numCols + n);
				ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::InputInt("##", &p.numSegments, 0, 0, ImGuiInputTextFlags_CharsDecimal);
				ImGui::PopID();

				++n;
				ImGui::TableSetColumnIndex(n);
				ImGui::PushID(rowId * numCols + n);
				if (ImGui::Button("X")) {
					removeRowId = rowId;
					selectedPointIdex = -1;
				}
				ImGui::PopID();

				ImGui::PopStyleColor(1);
			}
			ImGui::EndTable();

			if (removeRowId >= 0) {
				line->points.erase(line->points.begin() + removeRowId);
			}
		}
	}

	ImGui::End();
}





int32_t Game::UpdateLogic() {

	if (!ImGui::IsAnyItemActive()) {

		if (gg.keyboard[GLFW_KEY_W] && KeyboardGCDCheck()) {
			if (selectedLineName.empty()) {
				if (data.lines.size()) {
					SelectLine(data.lines[0].name);
				}
			} else {
				auto&& i = GetSelectedLineIndex();
				assert(i != -1);
				if (i > 0) {
					SelectLine(data.lines[i - 1].name);
				}
			}
			ClearEditState();
		}

		if (gg.keyboard[GLFW_KEY_S] && KeyboardGCDCheck()) {
			if (selectedLineName.empty()) {
				if (data.lines.size()) {
					SelectLine(data.lines[0].name);
				}
			} else {
				auto&& i = GetSelectedLineIndex();
				assert(i != -1);
				if (i < (int32_t)data.lines.size() - 1) {
					SelectLine(data.lines[i + 1].name);
				}
			}
			ClearEditState();
		}

		if (gg.keyboard[GLFW_KEY_UP] && KeyboardGCDCheck()) {
			if (selectedLineName.size()) {
				if (auto&& i = GetSelectedLineIndex(); i > 0) {
					std::swap(data.lines[i], data.lines[i - 1]);
				}
			}
		}

		if (gg.keyboard[GLFW_KEY_DOWN] && KeyboardGCDCheck()) {
			if (selectedLineName.size()) {
				if (auto&& i = GetSelectedLineIndex(); i < (int32_t)data.lines.size() - 1) {
					std::swap(data.lines[i], data.lines[i + 1]);
				}
			}
		}

	}

	auto&& line = GetSelectedLine();

	if (!ImGui::IsAnyItemActive() &&
		gg.mousePos.x > -gg.windowSize.x / 2 + leftPanelWidth + margin) {		// limit mouse pos in  edit area

		if (gg.keyboard[GLFW_KEY_Z] && KeyboardGCDCheck()) {
			zoom += 0.05;
			if (zoom >= 2) {
				zoom = 2;
			}
		}

		if (gg.keyboard[GLFW_KEY_X] && KeyboardGCDCheck()) {
			zoom -= 0.05;
			if (zoom <= 0.2) {
				zoom = 0.2;
			}
		}

		if (gg.keyboard[GLFW_KEY_A] && KeyboardGCDCheck()) {
			if (line) {
				auto&& xy = (gg.mousePos - offset) / zoom;
				auto& p = line->points.emplace_back();
				p.x = xy.x;
				p.y = xy.y;
			}
		}

		if (gg.keyboard[GLFW_KEY_D] && KeyboardGCDCheck()) {
			if (line && line->points.size()) {
				if (selectedPointIdex >= 0) {
					line->points.erase(line->points.begin() + selectedPointIdex);
					selectedPointIdex = -1;
				} else {
					line->points.pop_back();
					if (selectedPointIdex >= (int32_t)line->points.size()) {
						selectedPointIdex = -1;
					}
				}
			}
		}
	}

	if (!line) return 0;

	meListener.Update();
	int32_t i = 0;
	while (meListener.eventId && i < line->points.size()) {
		dc.point = &line->points[i];
		dc.idx = i;
		meListener.Dispatch(&dc);
		++i;
	}


	// draw begin
	xx::XY offset{ (leftPanelWidth + margin) / 2, 0 };
	ls.SetScale(zoom).SetPosition(offset);

	// draw bg
	float dw = data.designWidth, dh = data.designHeight, sl = data.safeLength;
	float dw2 = dw / 2, dh2 = dh / 2;
	float r1 = std::sqrt(dw2 * dw2 + dh2 * dh2);
	float r2 = r1 + sl;
	float w = dw + sl, h = dh + sl;
	float w2 = w / 2, h2 = h / 2;
	ls.SetColor({ 255,127,127,255 }).SetPoints({ {-r2, 0}, {r2, 0} }).Draw();
	ls.SetPoints({ {0, -r2}, {0, r2} }).Draw();
	ls.SetColor({ 127,127,255,255 }).FillBoxPoints({}, { dw, dh }).Draw();
	ls.SetColor({ 127,255,127,255 }).FillCirclePoints({}, r1).Draw();
	ls.SetColor({ 255,255,127,255 }).FillCirclePoints({}, r2).Draw();


	// draw points
	cps.clear();
	for (int32_t i = 0, e = line->points.size(); i < e; ++i) {
		auto& p = line->points[i];
		xx::XY pos{ (float)p.x, (float)p.y };
		cps.emplace_back(pos, (float)p.tension / 100.f, (int32_t)p.numSegments);
		lsPoint.SetPosition(pos * zoom + offset).SetColor(i == selectedPointIdex ? xx::RGBA8{ 255,0,0,255 } : i == 0 ? xx::RGBA8{ 0,255,255,255 } : xx::RGBA8{ 255,255,255,255 }).Draw();
	}
	if (line->points.size() < 2) return 0;
	mp.Clear();
	mp.FillCurve(line->isLoop, cps);

	// draw body segments
	if (mp.totalDistance > 1.f) {
		mpc.Init(mp);

		auto& ps = ls.SetColor({ 255,255,255,255 }).SetPosition(offset).SetScale(zoom).Clear().SetPoints();
		for (auto& p : mpc.points) {
			ps.emplace_back(p.pos);
			if (ps.size() == 65535) {		// batch commit
				ls.Draw();
				ls.Clear();
				ps.emplace_back(p.pos);
			}
		}
		ls.Draw();
	}

	return 0;
}

void Game::SelectLine(std::string_view const& name) {
	if (selectedLineName == name) return;
	selectedLineName = name;
	if (selectedLineName.empty()) return;
	changeLineName = name;
	ClearEditState();
}

int32_t Game::GetLineIndexByName(std::string_view const& name) {
	for (size_t i = 0, e = data.lines.size(); i < e; i++) {
		if (name == data.lines[i].name) return i;
	}
	return -1;
}

int32_t Game::GetSelectedLineIndex() {
	return GetLineIndexByName(selectedLineName);
}
MovePathStore::Line* Game::GetLineByName(std::string_view const& name) {
	for (size_t i = 0, e = data.lines.size(); i < e; i++) {
		if (name == data.lines[i].name) return &data.lines[i];
	}
	return {};
}
MovePathStore::Line* Game::GetSelectedLine() {
	return GetLineByName(selectedLineName);
}

bool Game::CreateNewLine() {
	if (newLineName.empty()) {
		msg = "line name can't be null";
		return false;
	}
	bool found = false;
	for (auto& l : data.lines) {
		if (l.name == newLineName) {
			msg = "line name already exists";
			return false;
		}
	}
	data.lines.emplace_back().name = newLineName;
	MakeNewLineName();
	return true;
}

void Game::MakeNewLineName() {
	if (data.lines.empty()) {
		newLineName = "c1";
	} else {
		newLineName.clear();
		int32_t v{};

		auto&& n = data.lines.back().name;
		int32_t i, e = (int32_t)n.size() - 1;
		for (i = e; i >= 0; --i) {
			auto& c = n[i];
			if (c < '0' || c > '9') break;
		}
		if (i == -1) {			// n is number
			xx::SvToNumber(n, v);
			xx::Append(newLineName, v + 1);
		} else if (i == e) {	// suffix has not number
			newLineName = n + "1";
		} else {
			xx::SvToNumber(std::string_view(n.data() + i + 1), v);
			xx::Append(newLineName, std::string_view(n.data(), i + 1), v + 1);
		}
	}
}


int32_t Game::KeyboardGCDCheck() {
	if (keyboardGCDNowSecs < gg.time) return SetKeyboardGCD();
	return 0;
}
int32_t Game::SetKeyboardGCD() {
	keyboardGCDNowSecs = gg.time + keyboardGCD;
	return 1;
}

void Game::ClearEditState() {
	selectedPointIdex = -1;
	dc.idx = -1;
	meListener.handler = {};
}

bool DragableCircle::HandleMouseDown(DragableCircleMouseEventListener& L) {
	if (idx == -1) return false;
	auto&& dx = gg.offset.x + point->x * gg.zoom - L.downPos.x;
	auto&& dy = gg.offset.y + point->y * gg.zoom - L.downPos.y;
	if (dx * dx + dy * dy < Game::pointRadius * Game::pointRadius) {
		pos = xx::XY{ (float)point->x, (float)point->y };
		gg.selectedPointIdex = idx;
		return true;
	}
	return false;
}

int32_t DragableCircle::HandleMouseMove(DragableCircleMouseEventListener& L) {
	auto o = (gg.mousePos - L.lastPos) / gg.zoom;
	pos += o;
	point->x = pos.x;
	point->y = pos.y;
	return 0;
}

void DragableCircle::HandleMouseUp(DragableCircleMouseEventListener& L) {
	point = {};
}
