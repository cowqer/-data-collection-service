# HDF5 CMake configuration file

set(HDF5_INCLUDE_DIRS "/home/cq/Desktop/git/data-collection-service/include/hdf5
")
set(HDF5_LIBRARY_DIRS "/home/cq/Desktop/git/data-collection-service/lib/hdf5/lib_amd64
")
set(HDF5_LIBRARIES "-lhdf5")

# Provide package information
include(CMakeFindDependencyMacro)
#find_dependency(HDF5 REQUIRED)

# Provide target for users


