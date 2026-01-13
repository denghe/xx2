#include "xx_file.h"
#include "xx_embeds.h"
#include "xx_gamebase.h"
#include "xx_spine.h"

namespace xx {
	
    /**************************************************************************************************/
    // xx_game.h
    /**************************************************************************************************/

    // call at Init / Run
    void GameBase::DisableIME() {
        // todo: more platform support
#ifdef WIN32
        ImmDisableIME(0);
#endif
    }

    // example:
    // GameBase::instance->delayUpdates.Emplace([w = WeakFromThis(this)] { if (!w) return 1; return 0; });
    void GameBase::ExecuteDelayUpdates() {
        for (int32_t i = delayUpdates.len - 1; i >= 0; --i) {
            if (delayUpdates[i]()) {
                delayUpdates.SwapRemoveAt(i);
            }
        }
    }

    // for window resize event
    void GameBase::ResizeCalc() {
        worldMinXY = -windowSize * 0.5f;
        worldMaxXY = windowSize * 0.5f;
        if (windowSize.x / designSize.x > windowSize.y / designSize.y) {
            scale = windowSize.y / designSize.y;
            size.y = designSize.y;
            size.x = windowSize.x / scale;
        }
        else {
            scale = windowSize.x / designSize.x;
            size.x = designSize.x;
            size.y = windowSize.y / scale;
        }
        size_2 = size * 0.5f;

        p7 = { -size_2.x, +size_2.y }; p8 = { 0, +size_2.y }; p9 = { +size_2.x, +size_2.y };
        p4 = { -size_2.x, 0 }; p5 = { 0, 0 }; p6 = { +size_2.x, 0 };
        p1 = { -size_2.x, -size_2.y }; p2 = { 0, -size_2.y }; p3 = { +size_2.x, -size_2.y };
    };

    // for framebuffer only
    void GameBase::SetWindowSize(XY siz) {
        if (siz.x < 1.f) siz.x = 1.f;
        if (siz.y < 1.f) siz.y = 1.f;
        windowSize = siz;
        ResizeCalc();
    }

    void GameBase::GLViewport() {
        glViewport(0, 0, (GLsizei)windowSize.x, (GLsizei)windowSize.y);
    }

    void GameBase::GLClear(RGBA8 c) {
        glClearColor(c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f);
        glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void GameBase::GLBlendFunc(std::array<uint32_t, 3> const& args) {
        if (blend != args) {
            ShaderEnd();
            blend = args;
            glBlendFunc(args[0], args[1]);
            glBlendEquation(args[2]);
        }
    }

    void GameBase::GLBlendFunc() {
        blend = blendDefault;
        glBlendFunc(blendDefault[0], blendDefault[1]);
        glBlendEquation(blendDefault[2]);
    }


    void GameBase::SetBlendPremultipliedAlpha(bool e_) {
        if (e_) {
            GLBlendFunc({ GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD });
        }
        else {
            GLBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD });
        }
    }

    void GameBase::SetDefaultBlendPremultipliedAlpha(bool e_) {
        if (e_) {
            blendDefault = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD };
        }
        else {
            blendDefault = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD };
        }
    }

    void GameBase::ShaderEnd() {
        if (shader) {
            shader->End();
            shader = {};
        }
    }
    
    std::string GameBase::ToSearchPath(std::string_view dir) {
        std::string s;

        dir = Trim(dir);
        if (dir.empty()) {
            printf("dir is empty");
            throw dir;
        }

        // replace all \ to /, .\ or ./ to empty
        for (size_t i = 0, siz = dir.size(); i < siz; i++) {
            if (dir[i] == '.' && (dir[i + 1] == '\\' || dir[i + 1] == '/')) {
                ++i;
                continue;
            }
            else if (dir[i] == '\\') {
                s.push_back('/');
            }
            else {
                s.push_back(dir[i]);
            }
        }
        if (s.empty()) {
            printf("dir is empty");
            throw dir;
        }

        // make sure / at the end
        if (s.back() != '/') {
            s.push_back('/');
        }

        return s;
    }

    void GameBase::SearchPathAdd(std::string_view dir, bool insertToFront) {
        auto s = ToSearchPath(dir);
        if (!IsAbsolutePathName(s)) {
            s.insert(0, rootPath);
        }

        if (insertToFront) {
            searchPaths.insert(searchPaths.begin(), std::move(s));
        }
        else {
            searchPaths.push_back(std::move(s));
        }
    }

    // search file by searchPaths + fn. not found return ""
    std::string GameBase::GetFullPath(std::string_view fn, bool fnIsFileName) {
        // prepare
        fn = Trim(fn);

        // is absolute path?
        if (IsAbsolutePathName(fn))
            return std::string(fn);

        // foreach search path find
        for (size_t i = 0, e = searchPaths.size(); i < e; ++i) {
            tmpPath = (std::u8string&)searchPaths[i];
            tmpPath /= (std::u8string_view&)fn;
            if (std::filesystem::exists(tmpPath)) {
                if (fnIsFileName) {
                    if (std::filesystem::is_regular_file(tmpPath)) goto LabReturn;
                }
                else {
                    if (std::filesystem::is_directory(tmpPath)) goto LabReturn;
                }
            }
            continue;
        LabReturn:
            return U8AsString(tmpPath.u8string());
        }
        // not found
        return {};
    }


	Data GameBase::LoadFileDataWithFullPath(std::string_view fp) {
		auto d = ReadAllBytes_sv(fp);
		if (!d) return d;
		TryZstdDecompress(d);
		return d;
	}

	// read all data by short path. return data + full path
	Data GameBase::LoadFileData(std::string_view fn) {
		auto p = GetFullPath(fn);
		if (p.empty()) throw fn;
		return LoadFileDataWithFullPath(p);
	}

	// copy or decompress
	Data GameBase::LoadDataFromData(uint8_t const* buf, size_t len) {
		if (IsZstdCompressedData(buf, len)) {
			Data d;
			ZstdDecompress({ (char*)buf, len }, d);
			return d;
		}
		return { buf, len };
	}

	Data GameBase::LoadDataFromData(Span d) {
		return LoadDataFromData((uint8_t*)d.buf, d.len);
	}

    Shared<GLTexture> GameBase::LoadTextureFromData(void* buf_, size_t len_) {
		if (IsZstdCompressedData(buf_, len_)) {	// zstd
			Data d;
			ZstdDecompress({ (char*)buf_, len_ }, d);
			return MakeShared<GLTexture>(LoadGLTexture(d));
		}
        return MakeShared<GLTexture>(LoadGLTexture(buf_, len_ ));
    }

	Shared<GLTexture> GameBase::LoadTextureFromData(Span d) {
		return LoadTextureFromData(d.buf, d.len);
	}

    Shared<GLTexture> GameBase::LoadTexture(std::string_view fn) {
		auto fp = GetFullPath(fn);
		return LoadTextureFromData(ReadAllBytes_sv(fp));
    }




	GameBase::~GameBase() {
		if (wnd) {
			glfwDestroyWindow(wnd);
			wnd = {};
		}
		glfwTerminate();
	}

	void GameBase::SetMousePointerVisible(bool visible_) {
		glfwSetInputMode(wnd, GLFW_CURSOR, visible_ ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
	}

	void GameBase::SetFullScreenMode(XY size_) {
		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		if (size_.x == 0 || size_.y == 0) {
			size_ = { mode->width, mode->height };
		}
		glfwSetWindowAttrib(wnd, GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowMonitor(wnd, monitor, 0, 0, size_.x, size_.y, mode->refreshRate);
		isFullScreen = true;
		isBorderless = false;
	}

	void GameBase::SetWindowMode(XY size_) {
		//glfwRestoreWindow(wnd);	// known issue: title bar out of screen
		SetBorderlessMode(false);

		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		// todo: min( mode.size, design size )
		if (size_.x == 0 || size_.y == 0) {
			size_ = designSize;
		}
		glfwSetWindowAttrib(wnd, GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowMonitor(wnd, NULL, (mode->width - (int)size_.x) / 2, (mode->height - (int)size_.y) / 2
			, size_.x, size_.y, GLFW_DONT_CARE);
		isFullScreen = false;
		isBorderless = false;
	}

	void GameBase::SetBorderlessMode(bool b) {
		if (b) {
			SetWindowMode();
		}
		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		glfwSetWindowAttrib(wnd, GLFW_DECORATED, GLFW_FALSE);
		glfwSetWindowMonitor(wnd, NULL, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
		isFullScreen = true;
		isBorderless = true;
	}

	// example: SleepSecs(cDelta - (glfwGetTime() - time));
	void GameBase::SleepSecs(double secs) {
#if WIN32
		if (secs > 0) {
			WaitForSingleObjectEx(eventForDelay, uint64_t(secs * 1000.0 + 0.5), FALSE);
		}
#else
		for (; secs > 0.003; secs -= 0.003) Sleep(3);
#endif
	}




	Shared<SoLoud::Wav> GameBase::LoadSoundSourceFromData(uint8_t* buf, size_t len, bool looping) {
		assert(!IsZstdCompressedData(buf, len));
		auto rtv = MakeShared<SoLoud::Wav>();
		auto r = rtv->loadMem(buf, len, false, false);
		assert(!r);
		if (looping) {
			rtv->setLooping(true);
		}
		return rtv;
	}

	Shared<SoLoud::Wav> GameBase::LoadSoundSource(std::string_view fn, bool looping) {
		auto d = LoadFileData(fn);
		assert(d);
		return LoadSoundSourceFromData(d.buf, d.len, looping);
	}

	unsigned int GameBase::GetActiveVoiceCount() {
		return sound.GetActiveVoiceCount();
	}

	int GameBase::PlayAudio(Shared<SoLoud::Wav> const& ss_, float volume_, float pan_, float speed_) {
		if (mute || audioVolume == 0) return 0;
		return sound.Play(ss_, volume_ * audioVolume, pan_, speed_);
	}

	int GameBase::PlayMusic(Shared<SoLoud::Wav> const& ss_, float volume_, float pan_, float speed_) {
		if (mute || musicVolume == 0) return 0;
		return sound.Play(ss_, volume_ * musicVolume, pan_, speed_);
	}






	void GameBase::HandleMouseMove(XY mp_) {
		mousePos = mp_;

		// search
		uiGrid.ForeachPoint(uiGrid.worldSize * 0.5f + mousePos);

		// search results -> tmpZNodes
		for (auto& i : uiGrid.results) {
			auto o = (Node*)uiGrid.NodeAt(i).ud;
			assert(o->typeId >= 10);
			tmpZNodes.Emplace(o->z, o);
		}

		// sort order by z
		std::sort(tmpZNodes.buf, tmpZNodes.buf + tmpZNodes.len
			, ZNode::GreaterThanComparer);

		// try dispatch
		for (auto& zn : tmpZNodes) {
			auto n = ((MouseEventHandlerNode*)zn.n);
			if (!n->PosInScissor(mousePos)) continue;
			if (n->OnMouseMove()) break;
		}
		tmpZNodes.Clear();
	}

	void GameBase::HandleMouseButtonPressed(int32_t idx) {
		// known issue: macos maybe duplicate msg ( when click button switch full screen )
		// search
		uiGrid.ForeachPoint(uiGrid.worldSize * 0.5f + mousePos);

		// search results -> tmpZNodes
		for (auto& i : uiGrid.results) {
			auto o = (Node*)uiGrid.NodeAt(i).ud;
			assert(o->typeId >= 10);
			tmpZNodes.Emplace(o->z, o);
		}

		// sort order by z
		std::sort(tmpZNodes.buf, tmpZNodes.buf + tmpZNodes.len
			, ZNode::GreaterThanComparer);

		for (auto& zn : tmpZNodes) {
			auto n = ((MouseEventHandlerNode*)zn.n);
			if (!n->PosInScissor(mousePos)) continue;
			if (n->OnMouseDown(idx)) break;
		}
		tmpZNodes.Clear();
		mouse[idx].Press();
	}

	void GameBase::HandleMouseButtonReleased(int32_t idx) {
		if (!mouse[idx]) return;
		mouse[idx].Release();
	}

	void GameBase::HandleMouseWheel(double xoffset, double yoffset) {
		if (yoffset > 0) {
			mouse[GLFW_MOUSE_BUTTON_LAST + 1].Press();
			wheelTimeouts[0] = time + 0.05f;
		}
		else if (yoffset < 0) {
			mouse[GLFW_MOUSE_BUTTON_LAST + 2].Press();
			wheelTimeouts[1] = time + 0.05f;
		}
		if (xoffset > 0) {
			mouse[GLFW_MOUSE_BUTTON_LAST + 3].Press();
			wheelTimeouts[2] = time + 0.05f;
		}
		else if (xoffset < 0) {
			mouse[GLFW_MOUSE_BUTTON_LAST + 4].Press();
			wheelTimeouts[3] = time + 0.05f;
		}
	}

	void GameBase::HandleKeyboardPressed(int32_t idx) {
		keyboard[idx].Press();
	}

	void GameBase::HandleKeyboardReleased(int32_t idx) {
		keyboard[idx].Release();
	}

	void GameBase::HandleJoystickConnected(int jid) {
		auto& joy = joys.Emplace();
		joy.Init(&time);
		joy.jid = jid;
		joy.name = glfwGetGamepadName(jid);
	}

	void GameBase::HandleJoystickDisconnected(int jid) {
		for (int i = 0; i < joys.len; ++i) {
			if (joys[i].jid == jid) {
				joys.RemoveAt(i);
				break;
			}
		}
	}

	void GameBase::BaseUpdate() {
		// call ui auto updates
		for (int32_t i = uiAutoUpdates.len - 1; i >= 0; --i) {
			auto& o = uiAutoUpdates[i];
			if (!o || o->Update()) {
				uiAutoUpdates.SwapRemoveAt(i);
			}
		}

		// fill all joystick data
		joy.ClearValues();
		if (joys.len) {
			GLFWgamepadstate gs;
			for (int32_t i = 0; i < joys.len; ++i) {
				auto& j = joys[i];
				auto jid = j.jid;
				if (glfwGetGamepadState(jid, &gs)) {
					for (auto i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; i++) {
						if (gs.buttons[i]) {
							j.btns[i].Press();
							joy.btns[i].Press();
						}
						else {
							j.btns[i].Release();
						}
					}
					for (auto i = 0; i < GLFW_GAMEPAD_AXIS_LAST - 1; i++) {
						auto v = gs.axes[i];
						if (std::abs(v) < 0.08f) j.axes[i] = 0.f;	// 0.08f: death zone
						else {
							j.axes[i] = v;
							joy.axes[i] = v;
						}
					}
					for (auto i = GLFW_GAMEPAD_AXIS_LAST - 1; i <= GLFW_GAMEPAD_AXIS_LAST; i++) {
						auto v = gs.axes[i];
						if (v < -0.9f) j.axes[i] = -1.f;	// 0.9f: death zone
						else {
							j.axes[i] = v;
							joy.axes[i] = v;
						}
					}

					// LT -> L2, RT -> R2
					if (j.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > -0.5f) {
						j.btns[GLFW_GAMEPAD_BUTTON_LEFT_THUMB].Press();
					}
					else {
						j.btns[GLFW_GAMEPAD_BUTTON_LEFT_THUMB].Release();
					}
					if (j.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > -0.5f) {
						j.btns[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB].Press();
					}
					else {
						j.btns[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB].Release();
					}
				}
				else {
					j.ClearValues();
				}
			}

			// LT -> L2, RT -> R2
			if (joy.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > -0.5f) {
				joy.btns[GLFW_GAMEPAD_BUTTON_LEFT_THUMB].Press();
			}
			else {
				joy.btns[GLFW_GAMEPAD_BUTTON_LEFT_THUMB].Release();
			}
			if (joy.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > -0.5f) {
				joy.btns[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB].Press();
			}
			else {
				joy.btns[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB].Release();
			}
		}

		// timeout
		for (int32_t i = 0; i < 4; ++i) {
			if (time < wheelTimeouts[i]) {
				mouse[GLFW_MOUSE_BUTTON_LAST + 1 + i].Press();
			}
			else {
				mouse[GLFW_MOUSE_BUTTON_LAST + 1 + i].Release();
			}
		}
	}



	int32_t GameBase::Run() {
		running = true;
		sound.Init();

#ifdef WIN32
		SetConsoleOutputCP(CP_UTF8);
		timeBeginPeriod(1);
#endif

		for (auto& o : keyboard) o.globalTime = &time;
		for (auto& o : mouse) o.globalTime = &time;
		joy.Init(&time);

		auto currDir = std::filesystem::absolute("./").u8string();
		rootPath = ToSearchPath((std::string&)currDir);
		searchPaths.clear();
		searchPaths.push_back(rootPath);

		uiGrid.Init(64, 128);	// 8k

		Init();	// call user func

		glfwSetErrorCallback([](int error, const char* description) {
			printf("glfwSetErrorCallback error = %d description = %s", error, description);
			throw std::runtime_error(description);
		});

		if (glfwInit() == GLFW_FALSE) return __LINE__;

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_DEPTH_BITS, 0);
		glfwWindowHint(GLFW_STENCIL_BITS, 0);
		//glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

		wnd = glfwCreateWindow((int)windowSize.x, (int)windowSize.y
			, title.c_str(), nullptr, nullptr);
		if (!wnd) return __LINE__;

		glfwMakeContextCurrent(wnd);
		glfwSetWindowUserPointer(wnd, this);

		glfwSetWindowSizeLimits(wnd, minSize.x, minSize.y, GLFW_DONT_CARE, GLFW_DONT_CARE);

		SetWindowMode();	// window pos -> center

		glfwPollEvents();	// fix macos content scale mouse pos issue

		int w{}, h{};
		glfwGetFramebufferSize(wnd, &w, &h);
		windowSize.x = (float)w;
		windowSize.y = (float)h;
		ResizeCalc();

		//glfwSetInputMode(wnd, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
		glfwSwapInterval(0);	// no v-sync by default
		gladLoadGL(glfwGetProcAddress);

		// register event callbacks

		// window focus
		glfwSetWindowFocusCallback(wnd, [](GLFWwindow* wnd, int focused) {
			auto g = (GameBase*)glfwGetWindowUserPointer(wnd);
			g->focused = focused != 0;
			g->OnFocus(g->focused);
		});

		// frame buffer resize
		glfwSetFramebufferSizeCallback(wnd, [](GLFWwindow* wnd, int w, int h) {
			auto g = (GameBase*)glfwGetWindowUserPointer(wnd);
			if (w == 0 || h == 0) {
				g->minimized = true;
			}
			else {
				g->minimized = false;
				g->windowSize.x = (float)w;
				g->windowSize.y = (float)h;
				g->ResizeCalc();
				g->GLViewport();
			}
			g->OnResize(false);
		});

		// mouse move
		glfwSetCursorPosCallback(wnd, [](GLFWwindow* wnd, double x, double y) {
			auto g = (GameBase*)glfwGetWindowUserPointer(wnd);
			g->HandleMouseMove(XY{ x - g->windowSize.x * 0.5f, g->windowSize.y * 0.5f - y });
		});

		// mouse click
		glfwSetMouseButtonCallback(wnd, [](GLFWwindow* wnd, int button, int action, int mods) {
			auto g = (GameBase*)glfwGetWindowUserPointer(wnd);
			if (action) {
				g->HandleMouseButtonPressed(button);
			}
			else {
				g->HandleMouseButtonReleased(button);
			}
		});

		// mouse wheel
		glfwSetScrollCallback(wnd, [](GLFWwindow* wnd, double xoffset, double yoffset) {
			auto g = (GameBase*)glfwGetWindowUserPointer(wnd);
			g->HandleMouseWheel(xoffset, yoffset);
		});

		// keyboard
		glfwSetKeyCallback(wnd, [](GLFWwindow* wnd, int key, int scancode, int action, int mods) {
			if (key < 0) return;    // macos fn key == -1
			auto g = (GameBase*)glfwGetWindowUserPointer(wnd);
			if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && (mods & GLFW_MOD_ALT)) {
				if (g->isFullScreen) {
					g->SetWindowMode();
				}
				else {
					g->SetBorderlessMode();
				}
				g->OnResize(true);
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
			auto g = GameBase::instance;
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
			printf("glGetError() == %d\n", (int)e);
		};

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		// init fonts
		embed.font_sys.Emplace();
		embed.font_sys->texs.Emplace(LoadTextureFromData(embeds::png::font_sys));
		{
			auto d = LoadDataFromData(embeds::fnt::font_sys);
			embed.font_sys->Init(d.buf, d.len, "font_sys", false);
		}

		// init pngs( texture combined with font )
		auto& ft = embed.font_sys->texs[0];
		embed.ui_s9 = { ft, 944, 1008, 6, 6 };
		embed.ui_button_h = { ft, 944, 992, 6, 6 };
		embed.ui_button_n = { ft, 928, 992, 6, 6 };
		embed.ui_checkbox_0 = { ft, 992, 992, 32, 32 };
		embed.ui_checkbox_1 = { ft, 960, 992, 32, 32 };
		embed.ui_imgbtn_h = { ft, 848, 992, 6, 6 };
		embed.ui_imgbtn_n = { ft, 848, 1008, 6, 6 };
		embed.ui_dropdownlist_icon = { ft, 896, 992, 32, 32 };
		embed.ui_dropdownlist_head = { ft, 864, 992, 32, 32 };
		embed.ui_panel = { ft, 928, 1008, 6, 6 };
		embed.ui_slider_bar = { ft, 832, 1008, 6, 6 };
		embed.ui_slider_block = { ft, 832, 992, 6, 6 };

		embed.shape_dot = { ft, 752, 992, 1, 1 };
		embed.shape_gear = { ft, 768, 992, 32, 32 };
		embed.shape_heart = { ft, 802, 997, 29, 24 };
		embed.icon_flags_.Emplace(ft, 734, 1008, 16, 16);
		embed.icon_flags_.Emplace(ft, 751, 1008, 16, 16);

		// init cfgs
		embed.cfg_s9.Emplace();
		embed.cfg_s9->frame = embed.ui_s9;

		embed.cfg_s9bN.Emplace();
		embed.cfg_s9bN->frame = embed.ui_button_n;

		embed.cfg_s9bH.Emplace(*embed.cfg_s9bN);
		embed.cfg_s9bH->frame = embed.ui_button_h;

		embed.cfg_s9bg.Emplace(*embed.cfg_s9bN);
		embed.cfg_s9bg->frame = embed.ui_panel;

		embed.cfg_sbar.Emplace(*embed.cfg_s9bN);
		embed.cfg_sbar->frame = embed.ui_slider_bar;

		embed.cfg_sblock.Emplace(*embed.cfg_s9bN);
		embed.cfg_sblock->frame = embed.ui_slider_block;
		// ...

		// init sound sources
		embed.ss_ui_focus = LoadSoundSourceFromData(embeds::wav::ui_focus);

		// init shaders
		shaderQuad.Init();
		shaderQuadLight.Init();
		shaderSpine.Init();
		shaderTexVert.Init();
		// ...

		gSpineEnv.Init();

		// search exists joys
		for (auto jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++) {
			if (glfwJoystickPresent(jid)) {
				HandleJoystickConnected(jid);
			}
		}


		glfwSwapBuffers(wnd);	// for draw font

		GLInit();	// call user func

		time = glfwGetTime();
		delta = 0.001;


		// window drag title / resize
		// put here avoid macos call it before gl init
		glfwSetWindowRefreshCallback(wnd, [](GLFWwindow* wnd) {
			auto g = (GameBase*)glfwGetWindowUserPointer(wnd);
			g->Loop(true);
			});

		while (running && !glfwWindowShouldClose(wnd)) {
			glfwPollEvents();
			Loop(false);
		}

		return EXIT_SUCCESS;
	}

	void GameBase::Loop(bool fromEvent) {
		GLClear(clearColor);
		GLBlendFunc();

		drawVerts = {};
		drawCall = {};

		BaseUpdate();

		Update();	// call user func

		ExecuteDelayUpdates();

		ShaderEnd();

		drawFPSTimePool += delta;
		++drawFPS;
		if (drawFPSTimePool >= 1.f) {
			Stat();	// call user func
			drawFPSTimePool = {};
			drawFPS = {};
		}

		glfwSwapBuffers(wnd);

		if (!fromEvent) {
			Delay();	// call user func
		}

		auto newTime = glfwGetTime();
		delta = newTime - time;
		time = newTime;
	}
}
