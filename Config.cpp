#include <cstring>
#include <cassert>
#include <algorithm>

#include "Config.h"

std::string ApplicationVersion()
{
    return std::to_string(MajorVersion)
        + "." + std::to_string(MinorVersion)
        + "." + std::to_string(Revision);
}

Config MakeConfig(int argc, char* argv[])
{
    Config config;
    std::string skipList;

    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "--skip-noname") == 0)
        {
            config.SkipNoname = true;
            continue;
        }

        if (std::strcmp(argv[i], "--show-unknown") == 0)
        {
            config.ShowUnknown = true;
            continue;
        }

        if (std::strncmp(argv[i], "--skip=", std::strlen("--skip=")) == 0)
        {
            skipList = argv[i];
            continue;
        }

        config.FileName = argv[i];
    }

    if (!skipList.empty())
    {
        auto& sl = config.SkipList;
        skipList = skipList.substr(std::strlen("--show="));

        for (auto wordBegin = skipList.cbegin(), end = skipList.cend();;)
        {
            auto wordEnd = std::find(wordBegin, end, ',');
            sl.emplace_back(wordBegin, wordEnd);

            if (wordEnd == end)
            {
                break;
            }

            wordBegin = wordEnd + 1;
        }

        sl.erase(std::remove(sl.begin(), sl.end(), ""), sl.end());
    }

    return config;
}

