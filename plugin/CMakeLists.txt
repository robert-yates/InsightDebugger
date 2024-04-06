cmake_minimum_required(VERSION 3.14)

project(InsightDebugger CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 HINTS "${BN_QT}/Qt6" COMPONENTS Core Gui Widgets REQUIRED)

add_library(${PROJECT_NAME} SHARED  src/plugin.cpp 
                                    inc/plugin.h
                                    debuggerui.qrc
                                    ${MOCS}
)

include_directories("./inc")
target_link_libraries(${PROJECT_NAME} PRIVATE binaryninjaapi binaryninjaui Qt6::Core Qt6::Gui Qt6::Widgets)

bn_install_plugin(${PROJECT_NAME})