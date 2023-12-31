cmake_minimum_required(VERSION 3.16)

project(FlagQuizII VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

# set icon on windows
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Core Network Svg Gui Multimedia)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appFlagQuizII
    main.cpp
    finitgame.h finitgame.cpp
    fgame.h fgame.cpp
    fgamesettings.h fgamesettings.cpp
    ${APP_ICON_RESOURCE_WINDOWS}
)

qt_add_qml_module(appFlagQuizII
    URI FlagQuizII
    VERSION 2.0
    QML_FILES
    Main.qml
    FAppMenu.qml
    FButton.qml
    FPoints.qml
    FErrorMessage.qml
    FGameInit.qml
    FSettings.qml
    FInformation.qml
    images.qrc
    SOURCES fgamesettings.h fgamesettings.cpp

)

if (ANDROID)
    include(D:/Qt/Android/android_openssl/CMakeLists.txt)
    set_target_properties(appFlagQuizII PROPERTIES
            QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
     )
endif()

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
    PRIVATE Qt6::Multimedia
)

install(TARGETS appFlagQuizII
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
