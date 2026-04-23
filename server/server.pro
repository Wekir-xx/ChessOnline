QT += core gui widgets

CONFIG += c++17

SOURCES += \
        main.cpp \
        main_widget.cpp \
        main_window.cpp \
        time_widget.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    defines.h \
    main_widget.h \
    main_window.h \
    time_widget.h
