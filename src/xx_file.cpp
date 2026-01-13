#include <xx.h>
#include <fstream>
#include <zstd.h>

namespace xx {
	
    /**************************************************************************************************/
    // xx_file.h
    /**************************************************************************************************/

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


    bool IsZstdCompressedData(void const* buf_, size_t len) {
        auto buf = (uint8_t const*)buf_;
        return len >= 4 && buf[0] == 0x28 && buf[1] == 0xB5 && buf[2] == 0x2F && buf[3] == 0xFD;
    }

    bool IsZstdCompressedData(Span d) {
        return IsZstdCompressedData(d.buf, d.len);
    }

    // .exe + 50k
    void ZstdDecompress(Span src, Data& dst) {
        auto&& siz = ZSTD_getFrameContentSize(src.buf, src.len);
        if (ZSTD_CONTENTSIZE_UNKNOWN == siz) throw std::logic_error("ZstdDecompress error: unknown content size.");
        if (ZSTD_CONTENTSIZE_ERROR == siz) throw std::logic_error("ZstdDecompress read content size error.");
        dst.Resize(siz);
        if (0 == siz) return;
        siz = ZSTD_decompress(dst.buf, siz, src.buf, src.len);
        if (ZSTD_isError(siz)) throw std::logic_error("ZstdDecompress decompress error.");
        dst.Resize(siz);
    }

    void TryZstdDecompress(Data& d) {
        if (d.len >= 4) {
            if (d[0] == 0x28 && d[1] == 0xB5 && d[2] == 0x2F && d[3] == 0xFD) {
                Data d2;
                ZstdDecompress(d, d2);
                std::swap(d, d2);
            }
        }
    }

    // .exe + 320k
    void ZstdCompress(std::string_view const& src, Data& dst, int level, bool doShrink) {
        auto req = ZSTD_compressBound(src.size());
        dst.Resize(req);
        dst.len = ZSTD_compress(dst.buf, dst.cap, src.data(), src.size(), level);
        if (doShrink) {
            dst.Shrink();
        }
    }

}
