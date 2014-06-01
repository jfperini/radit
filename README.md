R4dit - Software para radiodifusión.
=============================================

![Radit-1](https://raw.githubusercontent.com/jfperini/radit/master/screenshots/radit-header.jpg "Radit-1")

#### Radit es un proyecto de software libre para la automatización de emisoras de radio.

Entre otras funciones Radit soporta:

    Crossfade entre audios.
    Soporte para formatos (MP2, MP3, WAV, WMA, OGG, FLAC).
    Reproducción aleatoria de carpetas.
    Sistema de eventos para automatizar nuestros contenidos.
    Dos reproductores auxiliares y uno principal.
    Log de emisión
    Locuciones de hora, temperatura y humedad.
    Pisadores de entrada y salida.
    Conexiones con radios on-line.
    Secuencias y tandas para armar bloques publicitarios o musicales
    Y mucho más........

## Instalar

    En ArchLinux:

    [user@archlinux:~] yaourt -S radit
    
Se necesitan algunos permisos. Ejecutar el [script](https://github.com/jfperini/radit/blob/master/radit.sh):

    [user@archlinux:~] sh ~/Descargas/radit.sh
    
    [user@archlinux:~] radit

## Compilar

    cd src

    qmake-qt4 radit.pro

    make

El binario generado se encuntra en la carpeta LinuxDesktop.

![Radit-2](https://raw.githubusercontent.com/jfperini/radit/master/screenshots/radit-footer.jpg "Radit-2")

Página oficial: http://www.radit.org/net


