/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.5
import QtQuick.Controls 1.4

Menu {
    property variant coordinate
    property int markersCount
    property int mapItemsCount
    signal itemClicked(string item)

    signal selectMapType(variant mapType)

    function update() {
    }

    function createMenu(map)
    {
        clear()

        addItem(qsTr("Add Marker")).triggered.connect(function(){itemClicked("addMarker")})
        addItem(qsTr("Get coordinate")).triggered.connect(function(){itemClicked("getCoordinate")})
        addItem(qsTr("Fit Viewport To Map Items")).triggered.connect(function(){itemClicked("fitViewport")})

        addItem(qsTr("Delete all markers")).triggered.connect(function(){itemClicked("deleteMarkers")})

        addSeparator()

        for (var i = 0; i<map.supportedMapTypes.length; i++) {
            createMapTypeMenuItem(map.supportedMapTypes[i]).checked =
                    (map.activeMapType === map.supportedMapTypes[i]);
        }
    }

    function createMapTypeMenuItem(mapType)
    {
        var item = addItem(mapType.name);
        item.checkable = true;
        item.triggered.connect(function(){selectMapType(mapType)})
        return item;
    }

    onSelectMapType: {
        stackView.pop(page)
        for (var i = 0; i < items.length; i++) {
            items[i].checked = items[i].text === mapType.name
        }
        map.activeMapType = mapType
    }
}
