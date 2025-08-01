TEMPLATE = app
QT = gui core xml svg widgets printsupport
CONFIG += qt warn_on console release
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
SOURCES = src/bindata.cpp \
 src/changepointsontree.cpp \
 src/moveaddcp.cpp \
 src/move.cpp \
 src/moveposcp.cpp \
 src/moveremovecp.cpp \
 src/movevaluecp.cpp \
 src/node.cpp \
 src/param.cpp \
 src/tree.cpp \
 src/mainwindowimpl.cpp \
 src/genoplast.cpp \
 src/progressimpl.cpp \
 src/runparamimpl.cpp \
 src/plotimpl.cpp \
 src/paramcons.cpp \
 src/selectmovesimpl.cpp \
 src/gelmanrubinimpl.cpp \
 src/prior.cpp \
 src/editpriorimpl.cpp \
 src/editinitimpl.cpp \
 src/power.cpp \
 src/ancestralimpl.cpp
HEADERS = src/bindata.h \
 src/changepointsontree.h \
 src/moveaddcp.h \
 src/move.h \
 src/moveposcp.h \
 src/moveremovecp.h \
 src/movevaluecp.h \
 src/node.h \
 src/param.h \
 src/tree.h \
 src/mainwindowimpl.h \
 src/progressimpl.h \
 src/runparamimpl.h \
 src/plotimpl.h \
 src/paramcons.h \
 src/selectmovesimpl.h \
 src/gelmanrubinimpl.h \
 src/prior.h \
 src/editpriorimpl.h \
 src/exponential.h \
 src/editinitimpl.h \
 src/power.h \
 src/ancestralimpl.h
FORMS += ui/mainwindow.ui \
 ui/progress.ui \
 ui/runParam.ui \
 ui/plot.ui \
 ui/SelectMoves.ui \
 ui/gelmanrubin.ui \
 ui/editprior.ui \
 ui/editinit.ui \
 ui/ancestral.ui
