CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(QtExampleSpreadsheet)
FIND_PACKAGE(Qt4 REQUIRED)

SET(QtExampleSpreadsheet_SOURCES 
spreadsheet/mainwindow.cc 
spreadsheet/spreadsheet.cc 
spreadsheet/spreadsheetMain.cc 
spreadsheet/cell.cc 
finddialog/FindDialog.cc 
gotocell/gotocelldialog.cc 
sort/sortdialog.cc)

SET(QtExampleSpreadsheet_HEADERS
spreadsheet/mainwindow.h
spreadsheet/spreadsheet.h
finddialog/FindDialog.h
gotocell/gotocelldialog.h
sort/sortdialog.h)

QT4_WRAP_CPP(QtExampleSpreadsheet_HEADERS_MOC ${QtExampleSpreadsheet_HEADERS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(spreadsheet 
${QtExampleSpreadsheet_SOURCES}
${QtExampleSpreadsheet_HEADERS_MOC})
TARGET_LINK_LIBRARIES(spreadsheet ${QT_LIBRARIES})