# CMakeList.txt: CMake-Projektdatei der obersten Ebene. Führen Sie hier die globale Konfiguration aus,
# und schließen Sie Unterprojekte ein.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project ("cpp_class-c_struct_translation")

# Schließen Sie Unterprojekte ein.
add_subdirectory ("cpp_class-c_struct_translation")
