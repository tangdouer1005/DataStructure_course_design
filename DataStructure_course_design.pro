QT += core gui widgets

CONFIG += c++17

win32-msvc*:QMAKE_CXXFLAGS += /utf-8
# QMAKE_LFLAGS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

SOURCES += \
    $$files($$PWD/src/*.cpp)

HEADERS += \
    $$files($$PWD/src/*.h)

RESOURCES += \
    $$PWD/res/res.qrc

FORMS += $$PWD/src/DataStructure_course_design.ui

FORMS += $$PWD/src/navigation_window.ui

FORMS += $$PWD/src/alarm_window.ui

FORMS += $$PWD/src/login_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target