#include <cassert>
#include <iostream>
#include <memory>

#include "IDwarfCrawler.h"

void OperateLayer(std::unique_ptr<dwarf_crawler::IDwarfCrawler>& aCrawler)
{
    auto current = aCrawler->NextChild();

    while (auto current = aCrawler->NextSibling())
    {
        if (current->Tag != dwarf_crawler::DwarfTag::Unknown)
        {
            std::cout << *current;

            if (current->Tag == dwarf_crawler::DwarfTag::Namespace)
            {
                OperateLayer(aCrawler);
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

