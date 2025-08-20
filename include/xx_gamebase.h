#pragma once
#include "xx_task.h"
#include "xx_shader.h"
#include "xx_file.h"
#include "xx_zstd.h"
#include <SFML/Window.hpp>

namespace xx {

	// engine base code
	struct alignas(8) GameBase {
		inline static struct GameBase* instance{};
		GameBase(GameBase const&) = delete;
		GameBase& operator=(GameBase const&) = delete;
		GameBase() {
			instance = this;
		}

		std::u32string title{ U"game" };					// window title string
		XY designSize{ 1920, 1080 };						// design resolution
		XY windowSize{ designSize }, windowSizeBackup{};	// physics resolution
		XY size{}, size_2{};								// actual design size
		/*
				 screen anchor points
		   ┌───────────────────────────────┐
		   │ 7             8             9 │
		   │                               │
		   │                               │
		   │ 4             5             6 │
		   │                               │
		   │                               │
		   │ 1             2             3 │
		   └───────────────────────────────┘
		*/
		static constexpr XY a7{ 0, 1 }, a8{ 0.5, 1 }, a9{ 1, 1 };
		static constexpr XY a4{ 0, 0.5f }, a5{ 0.5, 0.5f }, a6{ 1, 0.5f };
		static constexpr XY a1{ 0, 0 }, a2{ 0.5, 0 }, a3{ 1, 0 };
		XY p7{}, p8{}, p9{};
		XY p4{}, p5{}, p6{};
		XY p1{}, p2{}, p3{};
		float scale{};										// base scale. actual design size * base scale = physics resolution
		float flipY{ 1 };									// -1: flip  for ogl frame buffer

		RGBA8 clearColor{};									// for glClearColor
		std::array<uint32_t, 3> blendDefault{ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD };
		std::array<uint32_t, 3> blend{ blendDefault };
		std::array<uint32_t, 3> blendBackup{};

		double time{}, delta{};								// usually for ui logic
		int32_t drawVerts{}, drawCall{}, drawFPS{};			// counters
		float drawFPSTimePool{};							// for count drawFPS
		Shader* shader{};
		std::string rootPath;
		std::vector<std::string> searchPaths;
		std::filesystem::path tmpPath;
		xx::Task<> baseTask;

		void ResizeCalc() {
			auto& ds = GameBase::instance->designSize;
			auto& ws = GameBase::instance->windowSize;
			if (ws.x / ds.x > ws.y / ds.y) {
				scale = ws.y / GameBase::instance->designSize.y;
				size.y = GameBase::instance->designSize.y;
				size.x = ws.x / scale;
			}
			else {
				scale = ws.x / GameBase::instance->designSize.x;
				size.x = GameBase::instance->designSize.x;
				size.y = ws.y / scale;
			}
			size_2 = size * 0.5f;
			p7 = { -size_2.x, +size_2.y }; p8 = { 0, +size_2.y }; p9 = { +size_2.x, +size_2.y };
			p4 = { -size_2.x, 0 }; p5 = { 0, 0 }; p6 = { +size_2.x, 0 };
			p1 = { -size_2.x, -size_2.y }; p2 = { 0, -size_2.y }; p3 = { +size_2.x, -size_2.y };
		};

		template<typename ST>
		XX_INLINE ST& ShaderBegin(ST& s) {
			if (shader != &s) {
				ShaderEnd();
				s.Begin();
				shader = &s;
			}
			return s;
		}

		XX_INLINE void ShaderEnd() {
			if (shader) {
				shader->End();
				shader = {};
			}
		}

		inline static bool IsAbsolute(std::string_view s) {
			return s[0] == '/' || (s.size() > 1 && s[1] == ':');
		}

		// convert dir to search path format
		inline static std::string ToSearchPath(std::string_view dir) {
			std::string s;

			dir = Trim(dir);
			if (dir.empty()) {
				CoutN("dir is empty");
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
				CoutN("dir is empty");
				throw dir;
			}

			// make sure / at the end
			if (s.back() != '/') {
				s.push_back('/');
			}

			return s;
		}

		// add relative base dir to searchPaths
		void SearchPathAdd(std::string_view dir, bool insertToFront = false) {
			auto s = ToSearchPath(dir);
			if (!IsAbsolute(s)) {
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
		std::string GetFullPath(std::string_view fn, bool fnIsFileName = true) {
			// prepare
			fn = Trim(fn);

			// is absolute path?
			if (IsAbsolute(fn))
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

		// read all data by full path
		Data LoadFileDataWithFullPath(std::string_view fp) {
			Data d;
			if (int32_t r = xx::ReadAllBytes((std::u8string_view&)fp, d)) {
				CoutN("file read error. r = ", r, ", fn = ", fp);
				throw fp;
			}
			if (d.len == 0) {
				CoutN("file content is empty. fn = ", fp);
				throw fp;
			}
			if (d.len >= 4 && d[0] == 0x28 && d[1] == 0xB5 && d[2] == 0x2F && d[3] == 0xFD) {	// zstd
				Data d2;
				ZstdDecompress(d, d2);
				return d2;
			}
			return d;
		}

		// read all data by short path. return data + full path
		std::pair<Data, std::string> LoadFileData(std::string_view const& fn) {
			auto p = GetFullPath(fn);
			if (p.empty()) {
				CoutN("fn can't find: ", fn);
				throw fn;
			}
			auto d = LoadFileDataWithFullPath(p);
			return { std::move(d), std::move(p) };
		}

		// load texture from file
		Ref<GLTexture> LoadTexture(std::string_view fn) {
			auto [d, p] = LoadFileData(fn);
			assert(d);
			return MakeRef<GLTexture>(LoadGLTexture(d, p));
		}

		// first life cycle
		void BaseInit() {
#ifdef WIN32
			SetConsoleOutputCP(CP_UTF8);
			timeBeginPeriod(1);
#endif
			auto currDir = std::filesystem::absolute("./").u8string();
			this->rootPath = this->ToSearchPath((std::string&)currDir);
			this->searchPaths.clear();
			this->searchPaths.push_back(this->rootPath);
		}
	};

}
