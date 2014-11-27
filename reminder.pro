QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET =    Reminder
TEMPLATE =  app
SOURCES +=  main.cpp\
            window.cpp \
            popup.cpp
HEADERS  += window.h \
            popup.h
FORMS    += window.ui \
            popup.ui

RESOURCES += \
    icons.qrc
RC_FILE += reminder.rc