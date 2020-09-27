cmake_minimum_required(VERSION 3.17)
project(Project)

set(CMAKE_CXX_STANDARD 11)

add_executable(Project main.cpp core_lexic_Lexic.cpp core_lexic_Lexic.h core_lexic_Reader.cpp core_lexic_Reader.h config.h core_lexic_Printer.cpp core_lexic_Printer.h utils_convert.h utils_convert.cpp utils_strext.cpp utils_strext.h)