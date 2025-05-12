cmake_minimum_required(VERSION 3.29)
project(EtToolkits LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

message("qt major version: ${QT_DEFAULT_MAJOR_VERSION}")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick QuickControls2)
find_package(Qt6 COMPONENTS ShaderTools)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)

qt_policy(SET QTP0001 NEW)
if(QT_KNOWN_POLICY_QTP0004)
    qt_policy(SET QTP0004 NEW)
endif()

list(APPEND PROJECT_SOURCES
            ${PROJECT_SOURCE_DIR}/src/main.cpp
            ${PROJECT_SOURCE_DIR}/src/cert_util/openssl_util.cpp
            ${PROJECT_SOURCE_DIR}/src/hex_util.cpp
            ${PROJECT_SOURCE_DIR}/rc/logo.rc
)

list(APPEND PROJECT_INCLUDES
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/3rdparty/QHotkey/QHotkey
)

list(APPEND DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)

list(APPEND LIBS_DIRS ${PROJECT_SOURCE_DIR}/libs)
list(APPEND EXTRA_COMPILE_FLAGS $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
list(APPEND EXTRA_COMPILE_FLAGS $<$<C_COMPILER_ID:MSVC>:/utf-8>)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/QHotkey)

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
  EtRngs$<$<CONFIG:Debug>:d>
  OpenSSL::Crypto
  fmt::fmt-header-only
  QHotkey::QHotkey
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
        ui/SystemTray.qml
        # ui/ToolSearchDialog.qml
        ui/test_tools/RandomTest.qml
        ui/cert_tools/GenCert.qml
        ui/cert_tools/utils.js
    SOURCES
        ${PROJECT_SOURCE_DIR}/src/random_test/self_test_rand.h ${PROJECT_SOURCE_DIR}/src/random_test/self_test_rand.cpp
        ${PROJECT_SOURCE_DIR}/src/random_test/RandomTest.h ${PROJECT_SOURCE_DIR}/src/random_test/RandomTest.cpp
        ${PROJECT_SOURCE_DIR}/src/cert_util/gen_cert.h ${PROJECT_SOURCE_DIR}/src/cert_util/gen_cert.cpp
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

# test code
add_executable(randomKats ${PROJECT_SOURCE_DIR}/test/randomKats.cpp ${PROJECT_SOURCE_DIR}/src/random_test/self_test_rand.cpp)
target_include_directories(randomKats PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(randomKats PRIVATE GTest::gtest GTest::gtest_main)

# NSIS packaging
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
	LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}" # dynamic library
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" # static library
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
message(STATUS "CMAKE_INSTALL_BINDIR: ${CMAKE_INSTALL_BINDIR}")

# Install all DLLs in the same directory as logviewer.exe
if (WIN32)
    file(GLOB DLLS "${CMAKE_CURRENT_SOURCE_DIR}/libs/*.dll")
    install(FILES ${DLLS} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(${Qt6_VERSION} VERSION_LESS "6.5")
    qt_generate_deploy_qml_app_script(
        TARGET ${PROJECT_NAME}
        FILENAME_VARIABLE deploy_script
    )
else()
    qt_generate_deploy_qml_app_script(
        TARGET ${PROJECT_NAME}
        OUTPUT_SCRIPT deploy_script
    )
endif()
install(SCRIPT ${deploy_script})

# Copy Visual Studio runtime libraries
if (MSVC)
    include(InstallRequiredSystemLibraries)
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION  "${CMAKE_INSTALL_BINDIR}")
endif()

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_PACKAGE_VENDOR "Trafalgar")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Qt6 QML application for set of tools")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
set(CPACK_PACKAGE_CONTACT "guoyawen98@gmail.com")

# NSIS specific configuration
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)  # 在安装前卸载旧版本
set(CPACK_NSIS_DISPLAY_NAME ${PROJECT_NAME})
set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_NSIS_MUI_ICON ${CMAKE_CURRENT_SOURCE_DIR}/rc/tools.ico)
set(CPACK_NSIS_MUI_UNIICON ${CMAKE_CURRENT_SOURCE_DIR}/rc/tools.ico)
set(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}.exe")
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    CreateShortCut \\\"$DESKTOP\\\\${PROJECT_NAME}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
")
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    Delete \\\"$DESKTOP\\\\${PROJECT_NAME}.lnk\\\"
")

set(CPACK_NSIS_MUI_WELCOME_PAGE_TITLE "${PROJECT_NAME} Setup Wizard")
set(CPACK_NSIS_MUI_WELCOME_PAGE_TEXT "Welcome to the ${PROJECT_NAME} Setup Wizard")
set(CPACK_NSIS_MUI_WELCOME_PAGE_TITLE_FINISH "${PROJECT_NAME} Setup Wizard Complete")
set(CPACK_NSIS_MUI_WELCOME_PAGE_TEXT_FINISH "The ${PROJECT_NAME} Setup Wizard has completed installing ${PROJECT_NAME}.")
set(CPACK_NSIS_MUI_FINISH_TEXT "The ${PROJECT_NAME} Setup Wizard has completed installing ${PROJECT_NAME}.")
set(CPACK_NSIS_MUI_FINISH_TEXT_FINISH "The ${PROJECT_NAME} Setup Wizard has completed installing ${PROJECT_NAME}.")

set(CPACK_NSIS_MUI_FINISHPAGE_RUN ON) # Run the application after installation
set(CPACK_NSIS_CREATE_ICONS_EXTRA "
    CreateShortCut \\\"$SMPROGRAMS\\\\${PROJECT_NAME}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
    CreateShortCut \\\"$SMPROGRAMS\\\\${PROJECT_NAME}\\\\Uninstall.lnk\\\" \\\"$INSTDIR\\\\Uninstall.exe\\\"
")

set(CPACK_NSIS_DELETE_ICONS_EXTRA "
    Delete \\\"$SMPROGRAMS\\\\${PROJECT_NAME}.lnk\\\"
    Delete \\\"$SMPROGRAMS\\\\${PROJECT_NAME}\\\\Uninstall.lnk\\\"
")


# WiX Toolset specific configuration
set(CPACK_WIX_PRODUCT_ICON ${CMAKE_CURRENT_SOURCE_DIR}/rc/tools.ico)
set(CPACK_WIX_LICENSE_RTF ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.rtf)
set(CPACK_WIX_UI_BANNER ${CMAKE_CURRENT_SOURCE_DIR}/rc/icon.png)
set(CPACK_WIX_UI_DIALOG ${CMAKE_CURRENT_SOURCE_DIR}/rc/icon.png)
# set(CPACK_WIX_PATCH_FILE "${PROJECT_SOURCE_DIR}/wix-patch.xml") # set environment variables

# Choose packaging tool
option(USE_NSIS "Use NSIS for packaging" ON)

if(USE_NSIS)
    set(CPACK_GENERATOR "NSIS")
else()
    set(CPACK_GENERATOR "WIX")
endif()

include(CPack)
