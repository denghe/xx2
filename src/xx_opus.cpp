#include "xx_opus.h"
#include <opusfile.h>

namespace xx {

	int LoadOpusMemory(xx::List<float>& tmpContainer, void* buf, size_t len) {
		tmpContainer.Clear();
		int error;
		auto f = op_open_memory((const unsigned char*)buf, len, &error);
		if (!f) return -__LINE__;
		auto f_sg = xx::MakeSimpleScopeGuard([&] { op_free(f); });
		auto head = op_head(f, -1);
		if (!head) return -__LINE__;
		assert(head->version == 1);
		auto channelCount = head->channel_count;
		static constexpr int bufCount{ 120 * 48 * 2 };
		float bufData[bufCount];
		int samplesRead{ -1 };
		while ((samplesRead = op_read_float(f, bufData, bufCount, nullptr)) > 0) {
			tmpContainer.AddRange(bufData, samplesRead * channelCount);
		}
		if (samplesRead < 0) return -__LINE__;
		return channelCount;
	}

	int LoadOpusFilePath(xx::List<float>& tmpContainer, std::filesystem::path opusFilePath) {
		auto data = ReadAllBytes(opusFilePath);
		return LoadOpusMemory(tmpContainer, data.buf, data.len);
	}

	int LoadOpusFileName(xx::List<float>& tmpContainer, std::string_view opusFilePath) {
		auto data = ReadAllBytes_sv(opusFilePath);
		return LoadOpusMemory(tmpContainer, data.buf, data.len);
	}

}
