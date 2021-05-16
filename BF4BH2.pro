QT       += core \
            gui \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    bf4bh2.cpp

HEADERS += \
    bf4bh2.h \
    defines.h \
    res.rc

FORMS += \
    bf4bh2.ui

TRANSLATIONS += \
    BF4BH2_ru_RU.ts

DISTFILES +=

RESOURCES += \
  res.qrc

RC_FILE =   res.rc

# разделяем по директориям все выходные файлы проекта
unix:OBJECTS_DIR = build/o/unix
win32:OBJECTS_DIR = build/o/win32
macx:OBJECTS_DIR = build/o/mac
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui

CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/Debug
} else {
    DESTDIR = $$OUT_PWD/Release
}

CONFIG(debug, debug|release) {
    QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$OUT_PWD/Debug
} else {
    QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$OUT_PWD/Release
}
