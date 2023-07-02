# CMakeList.txt: CMake-Projekt für "cpp_class-c_struct translation". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (cpp_class-c_struct_translation "cpp_class-c_struct_translation.cpp"  "Extract_Types.h" "Extract_Arguments.h")
add_executable (test_Extract_Arguments "test_Extract_Arguments.cpp"  "Extract_Types.h" "Extract_Arguments.h")
# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
