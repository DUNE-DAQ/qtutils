tdaq_package()

find_package(Qt5 REQUIRED COMPONENTS Widgets Gui HINTS ${QT5_ROOT})

qt5_wrap_cpp(moc_srcs
    QTUtils/highlighter.h
    QTUtils/FilterTextEdit.h 
    QTUtils/qdebugstream.h)

tdaq_add_library(qtutils 
  src/highlighter.cpp 
  src/qdebugstream.cpp
  ${moc_srcs}
  LINK_LIBRARIES Qt5::Gui Qt5::Widgets tdaq-common::ers)
