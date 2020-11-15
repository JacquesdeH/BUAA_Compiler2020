cmake_minimum_required(VERSION 3.17)
project(Project)

set(CMAKE_CXX_STANDARD 11)

add_executable(Project main.cpp core_lexic_Lexic.cpp core_lexic_Lexic.h core_lexic_Reader.cpp core_lexic_Reader.h config.h core_lexic_Printer.cpp core_lexic_Printer.h functional_convert.h functional_convert.cpp functional_strext.cpp functional_strext.h utils_PeekableQueue.cpp utils_PeekableQueue.h utils_Token.cpp utils_Token.h symbol_SymbolManager.cpp symbol_SymbolManager.h symbol_Table.cpp symbol_Table.h symbol_Info.cpp symbol_Info.h core_syntactic_Syntactic.cpp core_syntactic_Syntactic.h core_syntactic_Printer.cpp core_syntactic_Printer.h error_ErrorManager.cpp error_ErrorManager.h error_ErrorEntry.cpp error_ErrorEntry.h config.cpp utils_Quad.cpp utils_Quad.h utils_MIR.cpp utils_MIR.h utils_Proc.cpp utils_Proc.h utils_Block.cpp utils_Block.h core_semantic_Semantic.cpp core_semantic_Semantic.h Mips.cpp Mips.h LocalRegPool.cpp LocalRegPool.h ObjCodes.cpp ObjCodes.h)