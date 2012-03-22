CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(QtExampleDiagram)
FIND_PACKAGE(Qt4 REQUIRED)

SET(QtExampleDiagram_SOURCES 
Link.cc
Node.cc
DiagramWindow.cc
propertiesdialog.cc
main.cc)

SET(QtExampleDiagram_HEADERS
Link.h
Node.h
DiagramWindow.h
propertiesdialog.h)

SET(QtExampleDiagram_FORMS propertiesdialog.ui)

SET(QtExampleDiagram_RESOURCES resources.qrc)

QT4_WRAP_CPP(QtExampleDiagram_HEADERS_MOC ${QtExampleDiagram_HEADERS})
QT4_WRAP_UI(QtExampleDiagram_FORMS_HEADERS ${QtExampleDiagram_FORMS})
QT4_ADD_RESOURCES(QtExampleDiagram_RESOURCES_RCC ${QtExampleDiagram_RESOURCES})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(diagram
${QtExampleDiagram_SOURCES}
${QtExampleDiagram_FORMS_HEADERS}
${QtExampleDiagram_HEADERS_MOC}
${QtExampleDiagram_RESOURCES_RCC})
TARGET_LINK_LIBRARIES(diagram ${QT_LIBRARIES})