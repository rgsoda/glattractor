VPATH += ../shared
INCLUDEPATH += ../shared

HEADERS       = glwidget.h \
                window.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp
QT           += opengl

# install
target.path = .
sources.files = $$SOURCES $$HEADERS $$RESOURCES glattractor.pro
sources.path = .
INSTALLS += target sources

