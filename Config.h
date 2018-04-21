#pragma once
#include <string>

const constexpr size_t MajorVersion = 1;
const constexpr size_t MinorVersion = 0;
const constexpr size_t Revision = 12;

const constexpr size_t OffsetSize = 4;

const std::string ApplicationName = "dwarf-crawler";

inline std::string ApplicationVersion()
{
    return std::to_string(MajorVersion)
        + "." + std::to_string(MinorVersion)
        + "." + std::to_string(Revision);
}
