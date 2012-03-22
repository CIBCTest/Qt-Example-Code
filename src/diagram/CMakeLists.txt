CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(QtExampleDiagram)
FIND_PACKAGE(Qt4 REQUIRED)

SET(QtExampleDiagram_SOURCES 
Link.cc
Node.cc
DiagramWindow.cc
main.cc)

SET(QtExampleDiagram_HEADERS
Link.h
Node.h
DiagramWindow.h)

QT4_WRAP_CPP(QtExampleDiagram_HEADERS_MOC ${QtExampleDiagram_HEADERS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(diagram
${QtExampleDiagram_SOURCES}
${QtExampleDiagram_HEADERS_MOC})
TARGET_LINK_LIBRARIES(diagram ${QT_LIBRARIES})