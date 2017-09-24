#-------------------------------------------------
#
# Project created by QtCreator 2017-09-19T22:50:23
#
#-------------------------------------------------

QT       += webenginewidgets

HEADERS =   mainwindow.h \
    ManageDataFromUrl.h \
    CreateDelay.h
SOURCES =   main.cpp \
            mainwindow.cpp \
    ManageDataFromUrl.cpp \
    CreateDelay.cpp
RESOURCES = jquery.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/webenginewidgets/contentmanipulation
INSTALLS += target
