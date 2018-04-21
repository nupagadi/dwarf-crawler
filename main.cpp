#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>

#include "Config.h"
#include "IDwarfCrawler.h"


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

void PrintHelp(std::ostream& aStream)
{
    aStream << "Usage:" << std::endl;
    aStream << std::string(OffsetSize, ' ') << ApplicationName << " <application-name>" << std::endl;
}

void PrintVersion()
{
    std::cout << ApplicationName << " " << ApplicationVersion() << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Unsupported parameters!" << std::endl;
        PrintHelp(std::cerr);
        return EXIT_FAILURE;
    }

    if (std::strcmp(argv[1], "--help") == 0)
    {
        PrintHelp(std::cout);
        return EXIT_SUCCESS;
    }

    if (std::strcmp(argv[1], "--version") == 0)
    {
        PrintVersion();
        return EXIT_SUCCESS;
    }

    PrintVersion();
    try
    {
        auto crawler = dwarf_crawler::CreateDwarfCrawler(argv[1]);

        auto first = crawler->NextSibling();
        assert(first);
        std::cout << *first;

        OperateLayer(crawler);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}

