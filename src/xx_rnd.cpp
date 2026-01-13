#include "xx_rnd.h"
#include "xx_time.h"

namespace xx {

	Rnd::Rnd() {
		SetSeed(NowEpoch10m());
	}

	void Rnd::SetSeed(uint64_t seed) {
		auto calc = [&]()->uint64_t {
			auto z = (seed += 0x9e3779b97f4a7c15);
			z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
			z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
			return z ^ (z >> 31);
			};
		auto v = calc();
		memcpy(&state[0], &v, 8);
		v = calc();
		memcpy(&state[2], &v, 8);
	}

	uint32_t Rnd::Get() {
		auto rotl = [](uint32_t x, int s)->uint32_t {
			return (x << s) | (x >> (32 - s));
			};
		auto result = rotl(state[1] * 5, 7) * 9;
		auto t = state[1] << 9;
		state[2] ^= state[0];
		state[3] ^= state[1];
		state[1] ^= state[2];
		state[0] ^= state[3];
		state[2] ^= t;
		state[3] = rotl(state[3], 11);
		return result;
	}

	void Rnd::NextBytes(void* buf, size_t len) {
		uint32_t v{};
		size_t i{};
		auto e = len & (std::numeric_limits<size_t>().max() - 3);
		for (; i < e; i += 4) {
			v = Get();
			memcpy((char*)buf + i, &v, 4);
		}
		if (i < len) {
			v = Get();
			memcpy((char*)buf + i, &v, len - i);
		}
	}

	std::string Rnd::NextWord(size_t siz, std::string_view chars) {
		assert(chars.size() < 256);
		if (!siz) {
			siz = Next(2, 10);
		}
		std::string s;
		s.resize(siz);
		NextBytes(s.data(), siz);
		for (auto& c : s) {
			c = chars[c % chars.size()];
		}
		return s;
	}

}
