## dwarf-crawler

### Execution format
Usage:

    dwarf-crawler <application-name> [--skip=<name1>[|<name2>[|...]]] [--skip-noname] [--skip-unknown]
    dwarf-crawler --help
    dwarf-crawler --version

    --skip - Skip nodes with such names. The content will also be skipped if the node is Class, Struct or Namespace.
    --skip-noname - Skip nodes with no name.
    --show-unknown - Show nodes with unknown types. Now Namespace, Class, Struct, Subprogram and Field are known.

For example, to skip namespaces `std` and `__gnu_cxx` with all its content and also skip entries with no name:

    dwarf-crawler path/to/application/to/explore --skip-noname --skip=std,__gnu_cxx

### Third-party libraries used
[libdwarf](https://www.prevanders.net/dwarf.html)

[libelf](https://directory.fsf.org/wiki/Libelf)
### Code reused from another applications
None.
### Licenses for the libraries and any code used
None.
### Known limitations
- Applications to explore should be built using DWARF format. 
- Max application size is 20Mb.
### Known bugs
None.
