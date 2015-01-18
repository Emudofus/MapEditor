/*
 * Dofus Tiled Plugin
 * Copyright 2015, Sydoria Games <luax@sydoria.fr>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOFUSPLUGIN_H
#define DOFUSPLUGIN_H

#include "dofus_global.h"

#include "mapwriterinterface.h"
#include "layer.h"
#include "tilelayer.h"
#include "tile.h"

#include <QVector>
#include <QObject>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

struct t_element {
    QString elementName;
    int elementId = -1;
    int hue_1;
    int hue_2;
    int hue_3;
    int shadow_1;
    int shadow_2;
    int shadow_3;
    int offsetX;
    int offsetY;
    int altitude;
    int identifier;
};

struct t_cell {
    int cellId = -1;
    int elementsCount;
    QVector<struct t_element> elements;
};

struct t_layer {
    int layerId = -1;
    int cellsCount;
    QVector<struct t_cell> cells;
};

namespace Dofus {

class DOFUSSHARED_EXPORT DofusPlugin
        : public QObject
        , public Tiled::MapWriterInterface
{
    Q_OBJECT
    Q_INTERFACES(Tiled::MapWriterInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.mapeditor.MapWriterInterface" FILE "plugin.json")
#endif

public:
    DofusPlugin();

    // MapWriterInterface
    bool write(const Tiled::Map *map, const QString &fileName);
    QString nameFilter() const;
    QString errorString() const;

private:
    QString mError;
    const Tiled::Map* mMap;
    QMap<int, struct t_layer> mLayers;

    QJsonObject createLayer(int layerId);
    QJsonObject createCell(int cellId);
    QJsonObject createElement(int elementId);
    QJsonObject createCellData();
    void writeLayer(Tiled::Layer* layer);
    void writeCell(Tiled::Cell* cell, int layerId, int cellId);
    void writeElement(Tiled::Tile* tile, int layerId, int cellIndex);

};

} // namespace Dofus

#endif // DOFUSPLUGIN_H
