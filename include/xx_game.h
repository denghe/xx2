#pragma once
#include "xx_gamebase_font.h"
#include "xx_ui_ext.h"

namespace xx {
	// derived member checkers
	template<typename T> concept Has_Init = requires(T t) { { t.Init() } -> std::same_as<void>; };
	template<typename T> concept Has_GLInit = requires(T t) { { t.GLInit() } -> std::same_as<void>; };
	template<typename T> concept Has_Task = requires(T t) { { t.Task() } -> std::same_as<xx::Task<>>; };
	template<typename T> concept Has_Update = requires(T t) { { t.Update() } -> std::same_as<void>; };
	template<typename T> concept Has_Delay = requires(T t) { { t.Delay() } -> std::same_as<void>; };
	template<typename T> concept Has_Stat = requires(T t) { { t.Stat() } -> std::same_as<void>; };
	template<typename T> concept Has_OnResize = requires(T t) { { t.OnResize() } -> std::same_as<void>; };


	// for game scene logic
	struct SceneBase {
		virtual ~SceneBase() {}
		virtual void Update() {};
		virtual void Draw() {};
		virtual void OnResize() {};
	};


	// example: struct Game : xx::Game<Game> { ...
	template<typename Derived, typename BaseType = GameBase_font>
	struct Game : BaseType {
		~Game() {
			if (this->wnd) {
				glfwDestroyWindow(this->wnd);
				this->wnd = {};
			}
			glfwTerminate();
		}

		int32_t Run() {
			this->running = true;
			this->BaseInit();														// lifeCycle 1

			if constexpr (Has_Init<Derived>) {
				((Derived*)this)->Init();											// lifeCycle 2
			}

			glfwSetErrorCallback([](int error, const char* description) {
				CoutN("glfwSetErrorCallback error = ", error, " description = ", description);
				throw description;
			});

			if (glfwInit() == GLFW_FALSE) return __LINE__;

			glfwDefaultWindowHints();
			glfwWindowHint(GLFW_DEPTH_BITS, 0);
			glfwWindowHint(GLFW_STENCIL_BITS, 0);
			//glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

			this->wnd = glfwCreateWindow((int)this->windowSize.x, (int)this->windowSize.y
				, this->title.c_str(), nullptr, nullptr);
			if (!this->wnd) return __LINE__;

			glfwMakeContextCurrent(this->wnd);
			glfwSetWindowUserPointer(this->wnd, this);

			int w{}, h{};
			glfwGetFramebufferSize(this->wnd, &w, &h);
			this->windowSize.x = (float)w;
			this->windowSize.y = (float)h;
			this->ResizeCalc();

			//glfwSetInputMode(this->wnd, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
			glfwSwapInterval(0);	// no v-sync by default
			gladLoadGL(glfwGetProcAddress);

			// resize
			glfwSetFramebufferSizeCallback(this->wnd, [](GLFWwindow* wnd, int w, int h) {
				auto g = (Game*)glfwGetWindowUserPointer(wnd);
				g->windowSize.x = (float)w;
				g->windowSize.y = (float)h;
				g->ResizeCalc();
				g->GLViewport();
			});

#ifdef WIN32
			// drag title / resize( windows only? )
			glfwSetWindowRefreshCallback(this->wnd, [](GLFWwindow* wnd) {
				auto g = (Game*)glfwGetWindowUserPointer(wnd);
				g->Loop(true);
			});
#endif

			// mouse move
			glfwSetCursorPosCallback(this->wnd, [](GLFWwindow* wnd, double x, double y) {
				auto g = (Game*)glfwGetWindowUserPointer(wnd);
				g->HandleMouseMove(XY{ x - g->windowSize.x * 0.5f, g->windowSize.y * 0.5f - y });
			});

			// mouse click
			glfwSetMouseButtonCallback(this->wnd, [](GLFWwindow* wnd, int button, int action, int mods) {
				auto g = (Game*)glfwGetWindowUserPointer(wnd);
				if (action) {
					g->HandleMouseButtonPressed(button);
				}
				else {
					g->HandleMouseButtonReleased(button);
				}
			});

			// mouse wheel
			glfwSetScrollCallback(this->wnd, [](GLFWwindow* wnd, double xoffset, double yoffset) {
				auto g = (Game*)glfwGetWindowUserPointer(wnd);
				g->HandleMouseWheel(xoffset, yoffset);
			});

			// keyboard
			glfwSetKeyCallback(this->wnd, [](GLFWwindow* wnd, int key, int scancode, int action, int mods) {
				// action 1: down  0: up  2: repeat
				if (key < 0) return;    // macos fn key == -1
				auto g = (Game*)glfwGetWindowUserPointer(wnd);
				if (action == 0) {	// down
					g->HandleKeyboardReleased(key);
				}
				else if (action == 1) {	// up
					g->HandleKeyboardPressed(key);
				}
				// else action == 2  repeat
				// mods 0b 0011 win alt ctrl shift
			});

			// joystick
			glfwSetJoystickCallback([](int jid, int event) {
				auto g = GameBase_font::GetInstance();
				if (event == GLFW_CONNECTED) {
					g->HandleJoystickConnected(jid);
				}
				else if (event == GLFW_DISCONNECTED) {
					g->HandleJoystickDisconnected(jid);
				}
			});

			// ...

			// search exists joys
			for (auto jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++) {
				if (glfwJoystickPresent(jid)) {
					this->joys.Emplace().jid = jid;
				}
			}

			// dump & cleanup glfw3 error
			while (auto e = glGetError()) {
				CoutN("glGetError() == ", (int)e);
			};

			this->BaseGLInit();														// lifeCycle 3
			glfwSwapBuffers(this->wnd);

			if constexpr (Has_GLInit<Derived>) {
				((Derived*)this)->GLInit();											// lifeCycle 4
			}

			this->time = NowSteadyEpochSeconds();
			this->delta = 0.00001;

			if constexpr (Has_Task<Derived>) {
				this->baseTask = ((Derived*)this)->Task();							// lifeCycle 5
			}

			while (this->running && !glfwWindowShouldClose(this->wnd)) {
				glfwPollEvents();
				Loop(false);
			}

		LabEnd:
			return EXIT_SUCCESS;
		}

		void Loop(bool fromEvent) {
			this->GLClear(this->clearColor);
			this->GLBlendFunc();

			this->delta = NowSteadyEpochSeconds(this->time);
			this->drawVerts = {};
			this->drawCall = {};

			this->BaseUpdate();

			if constexpr (Has_Task<Derived>) {
				this->baseTask();													// lifeCycle 6 ( loop )
			}
			if constexpr (Has_Update<Derived>) {
				((Derived*)this)->Update();											// lifeCycle 7 ( loop )
			}

			this->ExecuteDelayFuncs();												// lifeCycle 8 ( loop )

			this->ShaderEnd();

			this->drawFPSTimePool += this->delta;
			++this->drawFPS;
			if (this->drawFPSTimePool >= 1.f) {
				if constexpr (Has_Stat<Derived>) {
					((Derived*)this)->Stat();										// lifeCycle 9 ( loop )
				}
				this->drawFPSTimePool = {};
				this->drawFPS = {};
			}

			glfwSwapBuffers(this->wnd);

			if (!fromEvent) {
				if constexpr (Has_Delay<Derived>) {
					((Derived*)this)->Delay();										// lifeCycle 10 ( loop )
				}
			}

		}

	};
}
