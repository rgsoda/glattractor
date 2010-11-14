VPATH += ../shared
INCLUDEPATH += ../shared

HEADERS       = glwidget.h \
                window.h \
                pointbuffer.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
                pointbuffer.cpp
QT           += opengl

# install
target.path = .
sources.files = $$SOURCES $$HEADERS $$RESOURCES glattractor.pro
sources.path = .
INSTALLS += target sources
