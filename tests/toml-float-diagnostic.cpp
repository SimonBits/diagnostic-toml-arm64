#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <filesystem>

#include "../lsfg-vk-common/thirdparty/include/toml.hpp"

int main(int argc, char* argv[])
{
    std::cout << "=== build info ===\n";

#ifdef __GNUC__
    std::cout << "__GNUC__ = "
              << __GNUC__ << "."
              << __GNUC_MINOR__ << "."
              << __GNUC_PATCHLEVEL__ << "\n";
#endif

    std::cout << "\n=== stringstream test ===\n";

    {
        const char chars[] = "0.8";
        double result{};

        std::stringstream ss;
        ss.imbue(std::locale::classic());
        ss.write(chars, 3);

        if (ss >> result)
            std::cout << "stringstream: OK -> "
                      << std::setprecision(17) << result << "\n";
        else
            std::cout << "stringstream: FAILED\n";
    }

    std::cout << "\n=== toml::parse string test ===\n";

    try
    {
        auto tbl = toml::parse("flow_scale = 0.8");
        auto value = tbl["flow_scale"].value<double>();

        if (value)
            std::cout << "toml::parse: OK -> "
                      << std::setprecision(17) << *value << "\n";
        else
            std::cout << "toml::parse: parsed, but no double value\n";
    }
    catch (const toml::parse_error& err)
    {
        std::cout << "toml::parse: FAILED -> " << err << "\n";
    }

    std::cout << "\n=== toml::parse_file test ===\n";

    if (argc < 2)
    {
        std::cout << "No config file supplied.\n";
        std::cout << "Usage: " << argv[0] << " /path/to/conf.toml\n";
        return 0;
    }

    const std::filesystem::path config_path = argv[1];

    std::cout << "file: " << config_path << "\n";

    try
    {
        auto tbl = toml::parse_file(config_path.string());

        std::cout << "parse_file: OK\n";

        if (auto version = tbl["version"].value<int64_t>())
            std::cout << "version: " << *version << "\n";
        else
            std::cout << "version: <not found or not integer>\n";

        auto profiles = tbl["profile"].as_array();

        if (!profiles)
        {
            std::cout << "profiles: <not found or not array>\n";
            return 0;
        }

        std::cout << "profiles: " << profiles->size() << "\n";

        for (std::size_t i = 0; i < profiles->size(); ++i)
        {
            auto* profile = profiles->get(i)->as_table();

            if (!profile)
            {
                std::cout << "profile[" << i << "]: <not table>\n";
                continue;
            }

            std::cout << "profile[" << i << "]\n";

            if (auto name = (*profile)["name"].value<std::string>())
                std::cout << "  name: " << *name << "\n";
            else
                std::cout << "  name: <not found>\n";

            if (auto flow_scale = (*profile)["flow_scale"].value<double>())
                std::cout << "  flow_scale: "
                          << std::setprecision(17)
                          << *flow_scale << "\n";
            else
                std::cout << "  flow_scale: <not found or not float>\n";
        }
    }
    catch (const toml::parse_error& err)
    {
        std::cout << "parse_file: FAILED\n";
        std::cout << err << "\n";
        return 1;
    }
    catch (const std::exception& err)
    {
        std::cout << "parse_file: FAILED (std::exception)\n";
        std::cout << err.what() << "\n";
        return 1;
    }

    return 0;
}
