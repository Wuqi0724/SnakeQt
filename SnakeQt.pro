QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boardwidget.cpp \
    main.cpp \
    snake.cpp \
    square.cpp \
    widget.cpp

HEADERS += \
    boardwidget.h \
    snake.h \
    square.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE += app.rc

RESOURCES += icons.qrc


# ---- Release 构建完成后，自动把 Qt 运行库补齐到 release 里 ----
# 作用与手动跑 windeployqt 相同，省掉记路径与敲命令。
# 只对 Release 生效，Debug 构建不受影响。
CONFIG(release, debug|release) {
    QMAKE_POST_LINK += $$quote(\"$$[QT_INSTALL_BINS]/windeployqt.exe\" --release --no-translations \"$$system_path($$OUT_PWD/release/$${TARGET}.exe)\")
}
