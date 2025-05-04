cmake_minimum_required(VERSION 3.29)
project(EtToolkits LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick QuickControls2)
find_package(Qt6 COMPONENTS ShaderTools)
find_package(folly CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(gtest CONFIG REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

list(APPEND PROJECT_SOURCES
            ${PROJECT_SOURCE_DIR}/src/main.cpp
            ${PROJECT_SOURCE_DIR}/rc/logo.rc
)

list(APPEND PROJECT_INCLUDES
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/src
)

list(APPEND DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)

list(APPEND LIBS_DIRS ${PROJECT_SOURCE_DIR}/lib)

list(
  APPEND
  LIBS
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::Qml
  Qt6::Quick
  Qt6::QuickControls2
  DelegateUI$<$<CONFIG:Debug>:d>
  Folly::folly Folly::folly_deps Folly::follybenchmark Folly::folly_test_util
)

set(WIN32_FLAG "")
if (WIN32)
    set(WIN32_FLAG "WIN32")
endif()
qt_add_executable(${PROJECT_NAME} ${WIN32_FLAG} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDES})
target_link_directories(${PROJECT_NAME} PUBLIC ${LIBS_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${DEFINITIONS})

target_compile_options(${PROJECT_NAME} PRIVATE ${EXTRA_COMPILE_FLAGS})

qt_add_qml_module(${PROJECT_NAME}
    URI "${PROJECT_NAME}Qml"
    VERSION 1.0
    QML_FILES
        ui/Main.qml 
        ui/HomePage.qml 
        ui/AboutPage.qml 
        ui/SettingsPage.qml 
        ui/test_tools/RandomTest.qml 
    SOURCES
        ${PROJECT_SOURCE_DIR}/src/random_test/self_test_rand.h ${PROJECT_SOURCE_DIR}/src/random_test/self_test_rand.cpp
        ${PROJECT_SOURCE_DIR}/src/random_test/RandomTest.h ${PROJECT_SOURCE_DIR}/src/random_test/RandomTest.cpp
    RESOURCES
        ui/fonts/HackNerdFont-Regular.ttf
        rc/icon.png
)

qt_add_shaders(${PROJECT_NAME} "shaders"
    PREFIX "/qt/qml"
    FILES
        ui/shaders/effect1.vert
        ui/shaders/effect1.frag
        ui/shaders/effect2.vert
        ui/shaders/effect2.frag
        ui/shaders/purplemoutain.vert
        ui/shaders/purplemoutain.frag
        ui/shaders/starfield.vert
        ui/shaders/starfield.frag
)

# Install all DLLs in the same directory as gen_cert.exe
if (WIN32)
    file(GLOB DLLS "${CMAKE_CURRENT_SOURCE_DIR}/libs/*.dll")
    install(FILES ${DLLS} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

add_executable(randomKats ${PROJECT_SOURCE_DIR}/test/randomKats.cpp ${PROJECT_SOURCE_DIR}/src/random_test/self_test_rand.cpp)
target_include_directories(randomKats PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(randomKats PRIVATE GTest::gtest GTest::gtest_main)