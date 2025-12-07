#include "pch.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    auto&& cp = std::filesystem::current_path();
    std::cout << "tool: *.webp -> *_{n}.png\n\nworking dir: " << cp.string() << R"#(

tips: require C:\Softs\libwebp-1.6.0-windows-x64\bin\webpmux.exe

press ENTER to continue...)#";
    std::cin.get();

    // webpmux.exe's path root
    std::string pathPrefix{ R"#(C:\Softs\libwebp-1.6.0-windows-x64\bin\)#" };

    for (auto&& entry : std::filesystem::/*recursive_*/directory_iterator(cp)) {
        if (!entry.is_regular_file()) continue;
        auto&& p = entry.path();
        if (p.extension() != u8".webp") continue;

        auto webpName = xx::U8AsString(p.filename().u8string());
        auto fullpath = xx::U8AsString(std::filesystem::absolute(p).u8string());
        auto pngNamePrefix = webpName.substr(0, webpName.size() - 5) + "_";

        xx::CoutN("handle ", webpName, ":");

        int32_t n{};    // num frames
        {
            auto cmd = xx::ToString(pathPrefix, "webpmux.exe -info \"", webpName, '"');
            auto [err, str] = xx::ExecCmd(cmd);
            if (err) {
                xx::CoutN("execute cmd: ", cmd, "\n\nlast error number = ", err);
                return err;
            }

            std::istringstream stream(str);
            std::string line;
            while (std::getline(stream, line)) {
                static constexpr auto key{ "Number of frames:" };
                if (line.starts_with(key)) {
                    xx::SvToNumber(line.substr(strlen(key) + 1), n);
                    std::cout << n << std::endl;
                }
                // todo: store  canvas size, x_offset, y_offset
            }
        }

        for (int32_t i = 0; i < n; ++i) {
            auto cmd = xx::ToString(pathPrefix, "webpmux.exe -get frame ", i, " ", webpName," -o ", pngNamePrefix, i, ".png");
            auto [err, str] = xx::ExecCmd(cmd);
            if (err) {
                xx::CoutN("execute cmd: ", cmd, "\n\nlast error number = ", err);
                return err;
            }
            else {
                xx::CoutN(cmd);
            }

            // todo: move png content to { x_offset, y_offset }, resize to canvas size
        }
    }

    xx::CoutN("finished! press ENTER to continue...");
    std::cin.get();

    return 0;
}
