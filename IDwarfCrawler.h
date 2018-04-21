#pragma once

#include <string>
#include <experimental/optional>

namespace dwarf_crawler
{

enum class DwarfTag
{
    Unknown,
    Namespace,
    Class,
    Struct,
    Subroutine,
};

struct DwarfNode
{
    const DwarfTag Tag;
    const std::string Name;
};

struct IDwarfCrawler
{
    template <typename T>
    using TOptional = std::experimental::optional<T>;

    virtual ~IDwarfCrawler() = default;

    virtual TOptional<DwarfNode> NextSibling() = 0;

    virtual TOptional<DwarfNode> NextChild() = 0;
};

}
