
TEMPLATE = lib

CONFIG += qt plugin c++11
QT     += qml quick

TARGET = $$qtLibraryTarget(qml-zmq)
uri = org.jemc.qml.ZMQ

TARGETDIR = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
SRCDIR    = $$PWD/src
DESTDIR   = $$PWD/build/native

android {
  VENDORDIR = $$PWD/vendor/prefix/$$ANDROID_TARGET_ARCH
  DESTDIR   = $$PWD/build/$$ANDROID_TARGET_ARCH
  QMAKE_LIBDIR += $$VENDORDIR/lib
  QMAKE_INCDIR += $$VENDORDIR/include
}

LIBS += -lzmq

HEADERS += $$SRCDIR/zmqplugin.h                  \
           $$SRCDIR/zmq_helper.h                 \
           $$SRCDIR/zmq_context.h                \
           $$SRCDIR/zmq_abstract_socket_thread.h \
           $$SRCDIR/zmq_util.h                   \
           $$SRCDIR/zmq_toplevel.h

SOURCES += $$SRCDIR/zmq_util.cpp

OBJECTS_DIR = $$TARGETDIR/.obj
MOC_DIR     = $$TARGETDIR/.moc
RCC_DIR     = $$TARGETDIR/.rcc
UI_DIR      = $$TARGETDIR/.ui

target.path  = $$TARGETDIR
qmldir.files = $$SRCDIR/qmldir
qmldir.path  = $$TARGETDIR
qml.files    = $$SRCDIR/qml/*
qml.path     = $$TARGETDIR/qml

INSTALLS += target qmldir qml

# Copy the libzmq shared library to the plugin folder (on android only)
android {
  androidlibs.files = $$VENDORDIR/lib/libzmq.so $$VENDORDIR/lib/libsodium.so
  androidlibs.path  = $$DESTDIR
  
  INSTALLS += androidlibs
}
