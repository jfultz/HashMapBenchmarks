cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project (HashMapExamples)


############################################################
#   base
############################################################

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_SUPPRESS_REGENERATION TRUE)


find_package(Boost)
if(Boost_FOUND)
	include_directories("nonius/include")
	include_directories(${Boost_INCLUDE_DIRS})

	add_executable(HashMapBenchmarks HashMapBenchmarks.cpp StringCMap.cpp StringUnorderedSet.cpp StringNSet.cpp)
endif()
