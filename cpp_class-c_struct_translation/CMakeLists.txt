﻿# CMakeList.txt: CMake-Projekt für "cpp_class-c_struct translation". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (cpp_class-c_struct_translation "cpp_class-c_struct_translation.cpp"  "Extract_Types.h" "Extract_Arguments.h")
add_executable (new_translator.cpp "new_translator.cpp")
add_executable (test_Extract_Arguments "test_Extract_Arguments.cpp" "Extract_Arguments.h")
add_executable (test_Check_If_Has_Member "test_Check_If_Has_Member.cpp" "Check_If_Has_Member.h")
add_executable (testcpp "test.cpp")
add_executable (ParameterPack "ParameterPack.cpp")
add_executable (new_translator_with_variant_tuple "new_translator_with_variant_tuple.cpp")
# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
