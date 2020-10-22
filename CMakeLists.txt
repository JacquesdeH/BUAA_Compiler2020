cmake_minimum_required(VERSION 3.17)
project(Project)

set(CMAKE_CXX_STANDARD 11)

add_executable(Project main.cpp core_lexic_Lexic.cpp core_lexic_Lexic.h core_lexic_Reader.cpp core_lexic_Reader.h config.h core_lexic_Printer.cpp core_lexic_Printer.h functional_convert.h functional_convert.cpp functional_strext.cpp functional_strext.h utils_PeekableQueue.cpp utils_PeekableQueue.h utils_Token.cpp utils_Token.h symbol_SymbolManager.cpp symbol_SymbolManager.h symbol_Table.cpp symbol_Table.h symbol_Info.cpp symbol_Info.h core_syntactic_Syntactic.cpp core_syntactic_Syntactic.h core_syntactic_Printer.cpp core_syntactic_Printer.h ErrorManager.cpp ErrorManager.h)