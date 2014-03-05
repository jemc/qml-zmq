
TEMPLATE = lib

CONFIG += plugin \
          c++11
QT += qml quick

TARGET = $$qtLibraryTarget(zmqplugin)
uri = org.jemc.qml.ZMQ

DESTDIR  = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
SRCDIR   = $$PWD/src
BUILDDIR = $$PWD/build

LIBS += -lzmq

HEADERS += $$SRCDIR/zmqplugin.h                      \
           $$SRCDIR/zmq_helper.h                     \
           $$SRCDIR/zmq_abstract_socket_thread.h     \
           $$SRCDIR/zmq_sockets.h

OBJECTS_DIR = $$BUILDDIR/.obj
MOC_DIR     = $$BUILDDIR/.moc
RCC_DIR     = $$BUILDDIR/.rcc
UI_DIR      = $$BUILDDIR/.ui

target.path  = $$DESTDIR
qmldir.files = $$PWD/qmldir
qmldir.path  = $$DESTDIR

OTHER_FILES += $$SRCDIR/qmldir \
               $$SRCDIR/qml/*

INSTALLS    += target qmldir

# Copy the qmldir file to the same folder as the plugin binary
QMAKE_POST_LINK += $$QMAKE_COPY $$replace($$list($$quote($$SRCDIR/qmldir) $$DESTDIR), /, $$QMAKE_DIR_SEP)

# Copy the qml implementation directory
copyqml.commands = $(COPY_DIR) $$SRCDIR/qml $$DESTDIR
first.depends = $(first) copyqml
export(first.depends)
export(copyqml.commands)
QMAKE_EXTRA_TARGETS += first copyqml
