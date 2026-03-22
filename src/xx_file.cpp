#include <xx_file.h>
#include <fstream>

namespace xx {

    Data ReadAllBytes(std::filesystem::path const& path) {
		std::ifstream f(path, std::ifstream::binary);
		if (!f) return {};													// not found? no permission? locked?
        auto sg = MakeScopeGuard([&] { f.close(); });
		f.seekg(0, f.end);
		auto&& siz = f.tellg();
		if ((uint64_t)siz > std::numeric_limits<size_t>::max()) return {};	// too big
		f.seekg(0, f.beg);
		auto outBuf = std::make_unique_for_overwrite<uint8_t[]>(siz);		// maybe throw oom
        Data d;
        d.Resize(siz);
		f.read((char*)d.buf, siz);
		if (!f) return {};													// only f.gcount() could be read
        return d;
	}

    Data ReadAllBytes_sv(std::string_view path) {
        if (path.empty()) return {};
        return ReadAllBytes((std::u8string_view&)path);
    }

	int WriteAllBytes(std::filesystem::path const& path, char const* buf, size_t len) {
		std::ofstream f(path, std::ios::binary | std::ios::trunc);
		if (!f) return -1;						// no create permission? exists readonly?
        auto sg = MakeScopeGuard([&] { f.close(); });
		f.write(buf, len);
		if (!f) return -2;						// write error
		return 0;
	}
	
    bool IsAbsolutePathName(std::string_view s) {
        return s[0] == '/' || (s.size() > 1 && s[1] == ':');
    }

}
