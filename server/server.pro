QT += core gui widgets sql

CONFIG += c++17

SOURCES += \
        db_interface.cpp \
        main.cpp \
        main_widget.cpp \
        main_window.cpp \
        time_widget.cpp

HEADERS += \
    db_interface.h \
    defines.h \
    main_widget.h \
    main_window.h \
    time_widget.h

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
