cmake_minimum_required(VERSION 3.16)

project(FlagQuizII VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Core Network Svg Gui)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appFlagQuizII
    main.cpp
    finitgame.h finitgame.cpp
)

qt_add_qml_module(appFlagQuizII
    URI FlagQuizII
    VERSION 2.0
    QML_FILES
    Main.qml
    FButton.qml
    FErrorMessage.qml
    FGameInit.qml
    images.qrc
)

set_target_properties(appFlagQuizII PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appFlagQuizII
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Core
    PRIVATE Qt6::Network
    PRIVATE Qt6::Svg
    PRIVATE Qt6::Gui
)

install(TARGETS appFlagQuizII
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
