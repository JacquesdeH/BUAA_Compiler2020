cmake_minimum_required(VERSION 3.17)
project(Project)

set(CMAKE_CXX_STANDARD 11)

add_executable(Project main.cpp core_lexic_Lexic.cpp core_lexic_Lexic.h core_lexic_Reader.cpp core_lexic_Reader.h config.h core_lexic_Printer.cpp core_lexic_Printer.h functional_convert.h functional_convert.cpp functional_strext.cpp functional_strext.h)