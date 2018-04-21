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
    Subprogram,
    Field,
};

inline std::ostream& operator <<(std::ostream& aStream, DwarfTag aTag)
{
    switch (aTag)
    {
    case DwarfTag::Namespace:
        return aStream << "Namespace";
    case DwarfTag::Class:
        return aStream << "Class";
    case DwarfTag::Struct:
        return aStream << "Struct";
    case DwarfTag::Subprogram:
        return aStream << "Subprogram";
    case DwarfTag::Field:
        return aStream << "Field";
    case DwarfTag::Unknown:
        return aStream << "UNKNOWN";
    }
}

struct DwarfNode
{
    const DwarfTag Tag;
    const std::string Name;
};

inline std::ostream& operator <<(std::ostream& aStream, const DwarfNode& aNode)
{
    return aStream << "{" << aNode.Tag << ", " << aNode.Name << "}" << std::endl;
}

struct IDwarfCrawler
{
    template <typename T>
    using TOptional = std::experimental::optional<T>;

    virtual ~IDwarfCrawler() = default;

    virtual TOptional<DwarfNode> NextSibling() = 0;

    virtual TOptional<DwarfNode> NextChild() = 0;

    virtual void ReturnToParent() = 0;
};

std::unique_ptr<IDwarfCrawler> CreateDwarfCrawler(const std::string& aFileName);

}
