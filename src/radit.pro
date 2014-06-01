#-------------------------------------------------
#
# Radit - Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui

TARGET = radit
TEMPLATE = app
QT += sql
CONFIG += release


SOURCES += main.cpp\
        mainwindow.cpp \
    Lista.cpp \
    Player.cpp \
    Eventos.cpp \
    ListaEventos.cpp \
    engine/StreamMath.cpp \
    engine/StreamFile.cpp \
    engine/Fader.cpp \
    engine/Pisador.cpp \
    engine/Stream.cpp \
    micelanea/BotonEstado.cpp \
    micelanea/ListaEstado.cpp \
    micelanea/Aleatorio.cpp \
    micelanea/IconoItem.cpp \
    PlayerSetting.cpp \
    micelanea/Dock.cpp \
    iolista/IoLista.cpp \
    engine/Hth.cpp \
    engine/TagInfo.cpp \
    engine/CurrentSound.cpp \
    engine/Pisadores.cpp \
    PlayerPisadores.cpp \
    Indicadores.cpp \
    PlayerGrupos.cpp \
    ListaGrupos.cpp \
    EditorEventos.cpp \
    AddEventos.cpp \
    AddRadioInternet.cpp \
    AddHora.cpp \
    ListaEventosEdit.cpp \
    ToolBar.cpp \
    AcercaDe.cpp \
    Cunas.cpp \
    EditorCunas.cpp \
    micelanea/CunaButton.cpp \
    engine/Cue.cpp \
    General.cpp

HEADERS  += mainwindow.h \
    Lista.h \
    Player.h \
    Eventos.h \
    ListaEventos.h \
    engine/StreamMath.h \
    engine/StreamFile.h \
    engine/Pisador.h \
    engine/Pisador.h \
    engine/Pisador.h \
    engine/Fader.h \
    engine/Stream.h \
    micelanea/BotonEstado.h \
    micelanea/ListaEstado.h \
    micelanea/Aleatorio.h \
    micelanea/IconoItem.h \
    PlayerSetting.h \
    micelanea/Dock.h \
    iolista/IoLista.h \
    engine/Hth.h \
    engine/TagInfo.h \
    engine/CurrentSound.h \
    engine/Pisadores.h \
    PlayerPisadores.h \
    Indicadores.h \
    PlayerGrupos.h \
    ListaGrupos.h \
    EditorEventos.h \
    AddEventos.h \
    AddRadioInternet.h \
    AddHora.h \
    ListaEventosEdit.h \
    ToolBar.h \
    AcercaDe.h \
    Cunas.h \
    EditorCunas.h \
    micelanea/CunaButton.h \
    engine/Cue.h \
    General.h

FORMS    += mainwindow.ui \
    FrmPlayer.ui \
    FrmEventos.ui \
    FrmSetting.ui \
    FrmPisadores.ui \
    FrmIndicadores.ui \
    FrmGrupos.ui \
    FrmEventosEdit.ui \
    FrmAddEvento.ui \
    FrmAddRadioInternet.ui \
    FrmHora.ui \
    FrmToolBar.ui \
    FrmAcercaDe.ui \
    FrmCunas.ui \
    FrmEditCunas.ui \
    FrmGeneral.ui




#-----------------------------------------------------------

unix {

LIBS += /usr/lib/libbass.so \
       /usr/lib/libqvumeterplug.so \
      /usr/lib/libtag.so.1



INCLUDEPATH = ../lib/bass/include \
              ../lib/taglib/include \
              ../lib/vumeter/include

OBJECTS_DIR =../out
MOC_DIR = ../out
UI_DIR = ../out
DESTDIR = ../LinuxDesktop




}

#-------------------------------------------------------------
RESOURCES += \
    radit.qrc


RC_FILE = radit.rc












































