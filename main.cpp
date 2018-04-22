#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <algorithm>

#include "Config.h"
#include "IDwarfCrawler.h"


void OperateLayer(
    std::unique_ptr<dwarf_crawler::IDwarfCrawler>& aCrawler, const Config aConfig, const size_t aOffset = 0)
{
    auto current = aCrawler->NextChild();

    if (!current)
    {
        aCrawler->ReturnToParent();
        return;
    }

    auto operateNode = [&aConfig, aOffset](const auto& current, auto operateParent)
    {
        if (current->Tag != dwarf_crawler::DwarfTag::Unknown || aConfig.ShowUnknown)
        {
            if (!aConfig.SkipNoname || !current->Name.empty())
            {
                auto& sl = aConfig.SkipList;
                if (std::find(sl.cbegin(), sl.cend(), current->Name) == sl.cend())
                {
                    std::cout << std::string(aOffset * OffsetSize, ' ');
                    std::cout << *current;

                    operateParent();
                }
            }
        }
    };

    operateNode(current, []{});

    while (auto current = aCrawler->NextSibling())
    {
        operateNode(current,
            [&current, &aCrawler, &aConfig, aOffset]{
                if (current->Tag == dwarf_crawler::DwarfTag::Namespace
                    || current->Tag == dwarf_crawler::DwarfTag::Class
                    || current->Tag == dwarf_crawler::DwarfTag::Struct)
                {
                    OperateLayer(aCrawler, aConfig, aOffset + 1);
                }

            }
        );
    }

    aCrawler->ReturnToParent();
}

void PrintHelp(std::ostream& aStream)
{
    aStream << "Usage:" << std::endl;
    aStream << std::string(OffsetSize, ' ') << ApplicationName << " <application-name>"
        << " [--skip=<name1>[|<name2>[|...]]]" << " [--skip-noname]" << " [--skip-unknown]" << std::endl;
    aStream << std::string(OffsetSize, ' ') << ApplicationName << " --help" << std::endl;
    aStream << std::string(OffsetSize, ' ') << ApplicationName << " --version" << std::endl;

    aStream << "--skip - " << "Skip nodes with such names. Their children will also be skipped." << std::endl;
    aStream << "--skip-noname - " << "Skip nodes with no name." << std::endl;
    aStream << "--show-unknown - "
        << "Show nodes with unknown types. Now Namespace, Class, Struct, Subprogram and Field are known." << std::endl;
}

void PrintVersion()
{
    std::cout << ApplicationName << " " << ApplicationVersion() << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2 || 5 < argc)
    {
        std::cerr << "Unsupported parameters number!" << std::endl;
        PrintHelp(std::cerr);
        return EXIT_FAILURE;
    }

    if (argc == 2)
    {
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
    }

    auto config = MakeConfig(argc, argv);

    PrintVersion();
    try
    {
        auto crawler = dwarf_crawler::CreateDwarfCrawler(config.FileName);

        auto first = crawler->NextSibling();
        assert(first);
        std::cout << *first;

        OperateLayer(crawler, config);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}

