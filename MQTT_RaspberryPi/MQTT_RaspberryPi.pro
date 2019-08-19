QT += gui widgets core

CONFIG += c++14 console
CONFIG -= app_bundle

QMAKE_LFLAGS += -lgps
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
           Debug_uc20g_init

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += "/home/lap13078/raspi/" \
               "/home/lap13078/raspi/sysroot/" \
               "/home/lap13078/raspi/sysroot/lib" \
               "/home/lap13078/raspi/sysroot/opt" \
               "/home/lap13078/raspi/sysroot/usr" \
               "/home/lap13078/raspi/sysroot/usr/include" \
               "/home/lap13078/raspi/sysroot/usr/include/arm-linux-gnueabihf"


LIBS +=    "-L/home/lap13078/raspi/" \
           "-L/home/lap13078/raspi/sysroot/" \
           "-L/home/lap13078/raspi/sysroot/opt" \
           "-L/home/lap13078/raspi/sysroot/lib" \
           "-L/home/lap13078/raspi/sysroot/usr" \
           "-L/home/lap13078/raspi/sysroot/usr/include" \
           "-L/home/lap13078/raspi/sysroot/usr/include/arm-linux-gnueabihf" \
           "-L/usr/local/include" -lwiringPi

SOURCES += main.cpp \
    uc2og_module_function.cpp


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/local/qt5pi/$${TARGET}/bin#opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    uc2og_module_function.h

