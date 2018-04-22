#pragma once

#include <string>
#include <vector>


const constexpr size_t MajorVersion = 1;
const constexpr size_t MinorVersion = 0;
const constexpr size_t Revision = 12;

const constexpr size_t OffsetSize = 4;

const std::string ApplicationName = "dwarf-crawler";

std::string ApplicationVersion();

struct Config
{
    std::string FileName;
    std::vector<std::string> SkipList;
    bool SkipNoname {};
    bool ShowUnknown {};
};

Config MakeConfig(int argc, char* argv[]);
