QT       += core gui multimedia quickwidgets qml network quick positioning location

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# Workaround for QTBUG-38735
QT_FOR_CONFIG += location-private
qtConfig(geoservices_mapboxgl): QT += sql opengl
qtConfig(geoservices_osm): QT += concurrent

SOURCES += \
    AudioFile.cpp \
    AudioStream.cpp \
    M10Decoder.cpp \
    M10GeneralParser.cpp \
    M10GtopParser.cpp \
    M10TrimbleParser.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    AudioFile.h \
    AudioStream.h \
    M10Decoder.h \
    M10GeneralParser.h \
    M10GtopParser.h \
    M10TrimbleParser.h \
    MainWindow.h

FORMS += \
    MainWindow.ui

OTHER_FILES += Map.qml \
    helper.js \
    map/MapComponent.qml \
    map/MapSliders.qml \
    map/Marker.qml \
    map/CircleItem.qml \
    map/RectangleItem.qml \
    map/PolylineItem.qml \
    map/PolygonItem.qml \
    map/ImageItem.qml \
    map/MiniMap.qml \
    menus/ItemPopupMenu.qml \
    menus/MainMenu.qml \
    menus/MapPopupMenu.qml \
    menus/MarkerPopupMenu

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
   Map.qml

RESOURCES += \
   SondeDecoder.qrc

RC_ICONS = resources/beacon.ico
