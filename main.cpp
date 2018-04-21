#include <cassert>
#include <iostream>
#include <memory>

#include "IDwarfCrawler.h"


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
    auto current = crawler->NextChild();
    if (!current)
    {
        std::cerr << "Unexpected end" << std::endl;
        return 1;
    }
    std::cout << *current;

    while (auto current = crawler->NextSibling())
    {
        if (current->Tag != dwarf_crawler::DwarfTag::Unknown)
        {
            std::cout << *current;

            if (current->Name == "top_stocks")
            {
                auto current2 = crawler->NextChild();
                assert(current2);
                std::cout << *current2;
                while (auto current2 = crawler->NextSibling())
                {
                    if (current2->Tag != dwarf_crawler::DwarfTag::Unknown)
                    {
                        std::cout << *current2;
                    }

                }
            }
        }
    }

    return 0;
}

