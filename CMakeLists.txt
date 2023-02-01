cmake_minimum_required(VERSION 3.12)
project(qtutils VERSION 0.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

find_package(ers REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets Gui)

qt5_wrap_cpp(moc_srcs
    include/qtutils/highlighter.hpp
    include/qtutils/FilterTextEdit.hpp 
    include/qtutils/qdebugstream.hpp)

daq_add_library(*.cpp ${moc_srcs} LINK_LIBRARIES Qt5::Gui Qt5::Widgets ers::ers)

daq_install()
