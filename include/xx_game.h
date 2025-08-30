#pragma once
#include "xx_gamebase_font.h"
#include "xx_ui.h"

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
#ifdef WIN32
		HANDLE eventForDelay{};
#endif

		~Game() {
			if (this->wnd) {
				glfwDestroyWindow(this->wnd);
				this->wnd = {};
			}
			glfwTerminate();
		}

		void SetFullScreenMode(XY size_ = {}) {
			auto monitor = glfwGetPrimaryMonitor();
			auto mode = glfwGetVideoMode(monitor);
			if (size_.x == 0 || size_.y == 0) {
				size_ = { mode->width, mode->height };
			}
			glfwSetWindowAttrib(this->wnd, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowMonitor(this->wnd, monitor, 0, 0, size_.x, size_.y, mode->refreshRate);
			this->isFullScreen = true;
			this->isBorderless = false;
		}

		void SetWindowMode(XY size_ = {}) {
			auto monitor = glfwGetPrimaryMonitor();
			auto mode = glfwGetVideoMode(monitor);
			// todo: min( mode.size, design size )
			if (size_.x == 0 || size_.y == 0) {
				size_ = this->designSize;
			}
			glfwRestoreWindow(this->wnd);
			glfwSetWindowAttrib(this->wnd, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowMonitor(this->wnd, NULL, (mode->width - size_.x) / 2, (mode->height - size_.y) / 2
				, size_.x, size_.y, GLFW_DONT_CARE);
			this->isFullScreen = false;
			this->isBorderless = false;
		}

		void SetBorderlessMode() {
			auto monitor = glfwGetPrimaryMonitor();
			auto mode = glfwGetVideoMode(monitor);
			glfwSetWindowAttrib(this->wnd, GLFW_DECORATED, GLFW_FALSE);
			glfwSetWindowMonitor(this->wnd, NULL, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
			this->isFullScreen = false;
			this->isBorderless = true;
		}

		// example: SleepSecs(cDelta - (glfwGetTime() - time));
		XX_INLINE void SleepSecs(double secs) {
#if WIN32
			if (secs > 0) {
				WaitForSingleObjectEx(eventForDelay, uint64_t(secs * 1000.0 + 0.5), FALSE);
			}
#else
			for (; secs > 0.003f; secs -= 0.003f) Sleep(3);
#endif
		}

		int32_t Run() {
			this->running = true;
			this->BaseInit();														// lifeCycle 1 ( once )

			if constexpr (Has_Init<Derived>) {
				((Derived*)this)->Init();											// lifeCycle 2 ( once )
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

			glfwSetWindowSizeLimits(this->wnd, this->minSize.x, this->minSize.y, GLFW_DONT_CARE, GLFW_DONT_CARE);

			SetWindowMode();	// window pos -> center

			int w{}, h{};
			glfwGetFramebufferSize(this->wnd, &w, &h);
			this->windowSize.x = (float)w;
			this->windowSize.y = (float)h;
			this->ResizeCalc();

			//glfwSetInputMode(this->wnd, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
			glfwSwapInterval(0);	// no v-sync by default
			gladLoadGL(glfwGetProcAddress);

			// register event callbacks

			// window focus
			glfwSetWindowFocusCallback(this->wnd, [](GLFWwindow* wnd, int focused) {
				auto g = (Game*)glfwGetWindowUserPointer(wnd);
				g->focused = focused != 0;
			});

			// window resize
			glfwSetFramebufferSizeCallback(this->wnd, [](GLFWwindow* wnd, int w, int h) {
				auto g = (Game*)glfwGetWindowUserPointer(wnd);
				if (w == 0 || h == 0) {
					g->minimized = true;
					return;
				}
				g->minimized = false;
				g->windowSize.x = (float)w;
				g->windowSize.y = (float)h;
				g->ResizeCalc();
				g->GLViewport();
				if constexpr (Has_OnResize<Derived>) {
					((Derived*)g)->OnResize();
				}
			});

			// window drag title / resize( windows only? )
			glfwSetWindowRefreshCallback(this->wnd, [](GLFWwindow* wnd) {
				auto g = (Game*)glfwGetWindowUserPointer(wnd);
				g->Loop(true);
			});

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
				if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && (mods & GLFW_MOD_ALT)) {
					if (g->isBorderless) {
						g->SetWindowMode();
					}
					else {
						g->SetBorderlessMode();
					}
					return;
				}
				if (action == GLFW_RELEASE) {
					g->HandleKeyboardReleased(key);
				}
				else if (action == GLFW_PRESS) {
					g->HandleKeyboardPressed(key);
				}
				// else action == GLFW_REPEAT
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

			// dump & cleanup glfw3 error
			while (auto e = glGetError()) {
				CoutN("glGetError() == ", (int)e);
			};

			this->BaseGLInit();														// lifeCycle 3 ( once )
			glfwSwapBuffers(this->wnd);	// for draw font

			if constexpr (Has_GLInit<Derived>) {
				((Derived*)this)->GLInit();											// lifeCycle 4 ( once )
			}

			this->time = glfwGetTime();
			this->delta = 0.001;
#ifdef WIN32
			eventForDelay = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif

			if constexpr (Has_Task<Derived>) {
				this->baseTask = ((Derived*)this)->Task();
			}

			while (this->running && !glfwWindowShouldClose(this->wnd)) {		// ( loop )
				glfwPollEvents();													// lifeCycle 5
				Loop(false);												// lifeCycle 6,7,8,9,10
			}

		LabEnd:
			return EXIT_SUCCESS;
		}

		void Loop(bool fromEvent) {
			this->GLClear(this->clearColor);
			this->GLBlendFunc();

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

			auto newTime = glfwGetTime();
			this->delta = newTime - this->time;
			this->time = newTime;
		}

	};
}
