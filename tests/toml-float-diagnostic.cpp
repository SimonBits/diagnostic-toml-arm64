#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>

#include "../lsfg-vk-common/thirdparty/include/toml.hpp"

int main()
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

    std::cout << "\n=== toml++ test ===\n";

    try
    {
        auto tbl = toml::parse("flow_scale = 0.8");
        auto value = tbl["flow_scale"].value<double>();

        if (value)
            std::cout << "toml++: OK -> "
                      << std::setprecision(17) << *value << "\n";
        else
            std::cout << "toml++: parsed, but no double value\n";
    }
    catch (const toml::parse_error& err)
    {
        std::cout << "toml++: FAILED -> " << err << "\n";
    }

    return 0;
}
