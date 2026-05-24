#pragma once
#include "pch.h"

// data store file json structs
namespace MovePathStore {
	struct Point {
		int x{}, y{}, tension{ 20 }, numSegments{ 100 };
	};
	struct Line {
		std::string name;
		bool isLoop{};
		std::vector<Point> points;
		xx::Shared<xx::GLTexture> tex;
	};
	struct Data {
		uint32_t designWidth{}, designHeight{}, safeLength{};
		std::vector<Line> lines;
	};
}

// for export MovePathStore
namespace xx {
	template<typename T>
	struct DataFuncs<T, std::enable_if_t<std::is_same_v<::MovePathStore::Data, std::decay_t<T>>>> {
		template<bool needReserve = true> static inline void Write(Data& d, T const& in) {
			d.Write(in.lines);
		}
	};
	template<typename T>
	struct DataFuncs<T, std::enable_if_t<std::is_same_v<::MovePathStore::Line, std::decay_t<T>>>> {
		template<bool needReserve = true> static inline void Write(Data& d, T const& in) {
			d.Write(in.name, in.isLoop, in.points);
		}
	};
	template<typename T>
	struct DataFuncs<T, std::enable_if_t<std::is_same_v<::MovePathStore::Point, std::decay_t<T>>>> {
		template<bool needReserve = true> static inline void Write(Data& d, T const& in) {
			d.Write(in.x, in.y, in.tension, in.numSegments);
		}
	};
}

AJSON(::MovePathStore::Point, x, y, tension, numSegments);
AJSON(::MovePathStore::Line, name, isLoop, points);
AJSON(::MovePathStore::Data, designWidth, designHeight, safeLength, lines);


// copy from old engine. // todo: replace
template<typename Handler>
struct MouseEventListener {
	int btn{};

	XY downPos{}, lastPos{};
	double downTime{};

	uint8_t lastState{};
	Handler handler{};
	int eventId{};	// 0: no event   1: down  2: move  3: up  4: cancel?

	void Init(int btn) {
		this->btn = btn;
	}

	// eventId > 0: need Dispatch
	void Update();

	// eventId > 0: need Dispatch next handler
	template<typename H>
	void Dispatch(H&& h) {
		assert(!handler);
		// todo: switch case ?
		if (h->HandleMouseDown(*this)) {
			handler = std::forward<H>(h);
			eventId = {};
		}
	}
};

struct DragableCircle;
using DragableCircleMouseEventListener = MouseEventListener<DragableCircle*>;

struct DragableCircle {
	bool HandleMouseDown(DragableCircleMouseEventListener& L);
	int32_t HandleMouseMove(DragableCircleMouseEventListener& L);
	void HandleMouseUp(DragableCircleMouseEventListener& L);
	MovePathStore::Point* point{};
	xx::XY pos{};
	int32_t idx{};
};


struct Game : xx::GameBase {
	static constexpr XY cDesignSize{ 1920, 1080 };
	static constexpr float cFps{ 120 };
	static constexpr float cDelta{ 1.f / cFps };
	static constexpr float cMaxDelta{ 0.1f };

	xx::Shared<xx::Node> ui;
	xx::Weak<xx::Label> uiText1, uiText2;
	xx::FrameBuffer fb;
	xx::MovePathCache mpc;
	xx::LineStrip ls;


	std::optional<std::string> msg;
	bool exporting{};
	std::string exportFileName;

	MovePathStore::Data data;
	std::string fileName, newLineName, changeLineName, selectedLineName;
	int32_t selectedPointIdex{ -1 };
	xx::LineStrip lsPoint;
	xx::MovePath mp;
	std::vector<xx::CurvePoint> cps;
	double zoom{ 0.3 }, keyboardGCD{ 0.2 }, keyboardGCDNowSecs{};

	// affine trans
	xx::XY afPos{}, afScale{ 1, 1 };
	float afAngle{};

	DragableCircleMouseEventListener meListener;
	DragableCircle dc;

	inline static const float leftPanelWidth{ 480 }, margin{ 10 }, leftCmdPanelHeight1{ 80 }, leftCmdPanelHeight2{ 120 }, pointRadius{10.f};
	inline static const xx::XY errPanelSize{ 1200, 200 }, offset{ (leftPanelWidth + margin) / 2, 0 };
	inline static const xx::XY exportPanelSize{ 1200, 200 };

	inline static const ImVec4 normalColor{ 0, 0, 0, 1.0f };
	inline static const ImVec4 pressColor{ 0.5f, 0, 0, 1.0f };
	inline static const ImVec4 releaseColor{ 0, 0.5f, 0, 1.0f };

	void Init() override;
	void GLInit() override;
	void OnExit() override;
	void Update() override;
	int32_t UpdateLogic();
	void Delay() override;
	//void Stat() override;
	void OnResize(bool modeChanged_) override;
	//void OnFocus(bool focused_) override;

	void ImGuiUpdate();
	void ImGuiDrawWindow_Error();

	void ImGuiDrawWindow_LeftTop0();
	void ImGuiDrawWindow_LeftTop();

	void ImGuiDrawWindow_LeftBottom0();
	void ImGuiDrawWindow_LeftBottom();

	void ImGuiDrawWindow_Export();

	void DrawIcon(MovePathStore::Line& line);

	void LoadData();
	void SaveData();
	void ExportData();
	static std::string ReplaceName(std::string name_);

	void SelectLine(std::string_view const& name);
	int32_t GetLineIndexByName(std::string_view const& name);
	int32_t GetSelectedLineIndex();
	MovePathStore::Line* GetLineByName(std::string_view const& name);
	MovePathStore::Line* GetSelectedLine();
	bool CreateNewLine();
	void MakeNewLineName();
	int32_t KeyboardGCDCheck();
	int32_t SetKeyboardGCD();
	void ClearEditState();
};

extern Game gg;

template<typename Handler>
void MouseEventListener<Handler>::Update() {
	if (auto&& state = gg.mouse[btn]; lastState != state) {
		lastState = state;
		if (state) {	// down
			assert(!handler);
			lastPos = downPos = gg.mousePos;
			downTime = gg.time;
			eventId = 1;	// need search handler
		}
		else {	// up
			if (handler) {
				handler->HandleMouseUp(*this);
				handler = {};
				eventId = {};
			}
		}
	}
	else {
		if (handler && lastPos != gg.mousePos) {	// move
			if ((eventId = handler->HandleMouseMove(*this))) {
				handler = {};
				// todo: if eventId == 1 { lastPos = downPos = engine.mousePosition; downTime = engine.nowSecs; }
				if (eventId == 4) {
					eventId = 0;
				}
			}
			else {
				lastPos = gg.mousePos;
			}
		}
	}
}
