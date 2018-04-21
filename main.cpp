#include <cassert>
#include <iostream>
#include <memory>

#include "IDwarfCrawler.h"

const constexpr size_t OffsetSize = 4;

void OperateLayer(std::unique_ptr<dwarf_crawler::IDwarfCrawler>& aCrawler, size_t aOffset = 0)
{
    auto current = aCrawler->NextChild();

    if (!current)
    {
        aCrawler->ReturnToParent();
        return;
    }

    std::cout << std::string(aOffset * OffsetSize, ' ');
    std::cout << *current;

    while (auto current = aCrawler->NextSibling())
    {
        if (current->Tag != dwarf_crawler::DwarfTag::Unknown)
        {
            std::cout << std::string(aOffset * OffsetSize, ' ');
            std::cout << *current;

            if (current->Tag == dwarf_crawler::DwarfTag::Namespace
                || current->Tag == dwarf_crawler::DwarfTag::Class
                || current->Tag == dwarf_crawler::DwarfTag::Struct)
            {
                OperateLayer(aCrawler, aOffset + 1);
            }
        }
    }

    aCrawler->ReturnToParent();
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Should provide executable name" << std::endl;
        return 1;
    }

    auto crawler = dwarf_crawler::CreateDwarfCrawler(argv[1]);

    auto first = crawler->NextSibling();
    assert(first);
    std::cout << *first;

    OperateLayer(crawler);

    return 0;
}

