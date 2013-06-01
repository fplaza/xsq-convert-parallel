xsq-convert-parallel
===========
xsq-convert-parallel converts Solid XSQ files to csfasta/qual files in parallel by launching several xsq-convert processes

Requirements
-------------
- A C++11 compiler (tested with latest versions of Clang and g++)
- [Boost C++ Libraries](www.boost.org)
- [HDF5 cpp library](http://www.hdfgroup.org/HDF5/doc/cpplus_RM/)
- Boost C++ Libraries (www.boost.org)
- xsq-convert executable in PATH

Performance
-------------
More running processes does not necessarily mean better performance.

Run your own tests to know with how many processes xsq-convert-parallel becomes I/O bound.
