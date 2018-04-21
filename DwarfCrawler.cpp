#include <iostream>

extern "C"
{
#include <unistd.h>
#include <fcntl.h>
#include <dwarf.h>
#include <libdwarf.h>
}

#include "IDwarfCrawler.h"

namespace dwarf_crawler
{

struct DwarfCrawler : IDwarfCrawler
{
    DwarfCrawler(const std::string& aFileName)
    {
        if ((mFileDecriptor = open(aFileName.c_str(), O_RDONLY)) < 0)
        {
            throw std::runtime_error("Error opening file");
        }

        Dwarf_Error err;
        if (dwarf_init(mFileDecriptor, DW_DLC_READ, 0, 0, &mDwarfDebug, &err) != DW_DLV_OK)
        {
            throw std::runtime_error("Failed DWARF initialization");
        }
    }

    ~DwarfCrawler()
    {
        Dwarf_Error err;

        if (dwarf_finish(mDwarfDebug, &err) != DW_DLV_OK)
        {
            std::cerr << "Failed DWARF finalization" << std::endl;
        }

        close(mFileDecriptor);
    }

    TOptional<DwarfNode> NextSibling() override;

    TOptional<DwarfNode> NextChild() override;

private:

    int mFileDecriptor = -1;

    Dwarf_Debug mDwarfDebug = 0;
};

}
