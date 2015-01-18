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

#include "dofusplugin.h"

#include "gidmapper.h"
#include "map.h"
#include "mapobject.h"
#include "mapreader.h"
#include "tile.h"
#include "tilelayer.h"
#include "tileset.h"
#include "objectgroup.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QStringList>
#include <QTextStream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

using namespace Dofus;
using namespace Tiled;

DofusPlugin::DofusPlugin()
{
}

QString DofusPlugin::nameFilter() const
{
    return tr("Dofus map files (*.d2m)");
}

QString DofusPlugin::errorString() const
{
    return mError;
}

bool DofusPlugin::write(const Tiled::Map *map, const QString &fileName)
{
    //GidMapper gidMapper(map->tilesets());
    QJsonObject json;

    json["header"] = 77;
    json["mapVersion"] = 8;
    json["mapId"] = 0; // TODO
    json["encrypted"] = true;
    json["encryptionVersion"] = 1;
    json["relativeId"] = 0; // TODO
    json["mapType"] = 0;
    json["subareaId"] = 0; // TODO
    json["topNeighbourId"] = 0; // TODO
    json["bottomNeighbourId"] = 0; // TODO
    json["leftNeighbourId"] = 0; // TODO
    json["rightNeighbourId"] = 0; // TODO
    json["shadowBonusOnEntities"] = 0;
    json["zoomScale"] = 100;
    json["zoomOffsetX"] = 0;
    json["zoomOffsetY"] = 0;
    json["useLowPassFilter"] = false;
    json["useReverb"] = false;
    json["presetId"] = -1;
    json["foregroundsCount"] = 0; // TODO
    json["backgroundsCount"] = 0; // TODO
    json["backgroundRed"] = map->backgroundColor().red();
    json["backgroundGreen"] = map->backgroundColor().green();
    json["backgroundBlue"] = map->backgroundColor().blue();
    json["backgroundColor"] = 0; // Needed ?
    json["backgroundFixtures"] = QJsonArray(); // TODO
    json["foregroundsFixtures"] = QJsonArray(); // TODO
    json["unknown_1"] = 0;
    json["groundCRC"] = 0; // How to calcul it ?
    json["layersCount"] = map->layerCount(); // Not correct



    QJsonArray layers;

    foreach (Layer *layer, map->layers()) {

        layer->hasProperty("level");

        if (TileLayer *tileLayer = layer->asTileLayer()) {
            Cell cell = tileLayer->cellAt(0, 0);
            if (cell.tile) {
                qDebug() << cell.tile->property(QLatin1String("elementId"));
            }
        }
    }

    json["layers"] = layers;

    QJsonDocument output(json);
    qDebug() << output.toJson();

    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Dofus, DofusPlugin)
#endif
