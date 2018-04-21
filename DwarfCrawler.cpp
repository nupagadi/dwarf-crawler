#include <cassert>
#include <iostream>
#include <vector>
#include <memory>

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

        Dwarf_Unsigned cu_header_length, abbrev_offset, next_cu_header;
        Dwarf_Half version_stamp, address_size;

        if (dwarf_next_cu_header(
                mDwarfDebug,
                &cu_header_length,
                &version_stamp,
                &abbrev_offset,
                &address_size,
                &next_cu_header,
                &err) == DW_DLV_ERROR)
        {
            throw std::runtime_error("Error reading DWARF cu header\n");
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

    TOptional<DwarfNode> NextSibling() override
    {
        Dwarf_Error err;
        int rc;

        auto prev = mCurrentDie;
        dwarf_siblingof(mDwarfDebug, prev, &mCurrentDie, &err);
        if (rc == DW_DLV_ERROR)
        {
            throw std::runtime_error("Error getting sibling of DIE");
        }
        else if (rc == DW_DLV_NO_ENTRY)
        {
            return {};
        }
        else if (prev == mCurrentDie)
        {
            return {};
        }

        return MakeDwarfNode(mCurrentDie);
    }

    TOptional<DwarfNode> NextChild() override
    {
        Dwarf_Error err;
        int rc;

        mParents.push_back(mCurrentDie);

        rc = dwarf_child(mCurrentDie, &mCurrentDie, &err);
        if (rc == DW_DLV_ERROR)
        {
            throw std::runtime_error("Error getting child of DIE");
        }
        else if (rc == DW_DLV_NO_ENTRY)
        {
            return {};
        }

        return MakeDwarfNode(mCurrentDie);
    }

private:

    static TOptional<DwarfNode> MakeDwarfNode(Dwarf_Die aDie)
    {
        char* dieName = 0;
        Dwarf_Error err;
        Dwarf_Half tag;
        int rc = dwarf_diename(aDie, &dieName, &err);

        if (rc == DW_DLV_ERROR)
        {
            throw std::runtime_error("Error in dwarf_diename");
        }
//        else if (rc == DW_DLV_NO_ENTRY)
//        {
//        }

        if (dwarf_tag(aDie, &tag, &err) != DW_DLV_OK)
        {
            throw std::runtime_error("Error in dwarf_tag");
        }

        return {{Convert(tag), dieName ? dieName : ""}};
    }

    static DwarfTag Convert(Dwarf_Half aRaw)
    {
        switch (aRaw)
        {
        case DW_TAG_subprogram:
            return DwarfTag::Subprogram;
        case DW_TAG_class_type:
            return DwarfTag::Class;
        case DW_TAG_structure_type:
            return DwarfTag::Struct;
        case DW_TAG_namespace:
            return DwarfTag::Namespace;
        default:
            return DwarfTag::Unknown;
        }
    }

    int mFileDecriptor = -1;

    Dwarf_Debug mDwarfDebug = nullptr;

    Dwarf_Die mCurrentDie = nullptr;
    std::vector<Dwarf_Die> mParents;
};

std::unique_ptr<IDwarfCrawler> CreateDwarfCrawler(const std::string& aFileName)
{
    return std::make_unique<DwarfCrawler>(aFileName);
}

}
