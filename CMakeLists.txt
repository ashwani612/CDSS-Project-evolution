cmake_minimum_required(VERSION 3.20)

project(CDSS_Evolution3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Drogon CONFIG REQUIRED)

add_executable(CDSS src/main.cpp)

target_compile_definitions(CDSS PRIVATE
    DROGON_STATIC_DEFINE
    TRANTOR_STATIC_DEFINE
)

target_link_libraries(CDSS PRIVATE
    Drogon::Drogon
    C:/msys64/ucrt64/lib/libsqlite3.dll.a
)