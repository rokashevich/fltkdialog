QT += core gui widgets xml svg svgwidgets
CONFIG += c++17
LIBS += -lpthread
# https://doc.qt.io/qt-6/qmake-function-reference.html
SOURCES = $$files(*.cpp, true)
HEADERS = $$files(*.hpp, true)

OTHER_FILES += $$files(*.qss, true)
OTHER_FILES += $$files(*.svg, true)
OTHER_FILES += $$files(*.sh, true)
