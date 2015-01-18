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
#include "tileset.h"
#include "objectgroup.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QStringList>
#include <QTextStream>



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
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        mError = tr("Could not open file for writing.");
        return false;
    }

    mMap = map;
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
    json["layersCount"] = 0;

    for (int i = 0; i < 4; i++)
    {
        mLayers[i].layerId = i;
        mLayers[i].cellsCount = 0;
    }

    foreach (Layer *layer, map->layers())
    {
        writeLayer(layer);
    }

    QJsonArray layers;
    int layersCount = 0;

    for (int i = 0; i < 4; i++)
    {
        if (mLayers[i].cellsCount == 0)
            continue;

        layersCount++;

        QJsonObject layer = createLayer(mLayers[i].layerId);
        QJsonArray cells;
        int cellsCount = 0;

        for (int cellIndex = 0; cellIndex < mLayers[i].cellsCount; cellIndex++)
        {
            if (mLayers[i].cells[cellIndex].elementsCount == 0)
                continue;

            cellsCount++;

            QJsonObject cell = createCell(mLayers[i].cells[cellIndex].cellId);
            cell["elementsCount"] = mLayers[i].cells[cellIndex].elementsCount;
            cell["elements"] = QJsonArray();
            QJsonArray elements;

            for (int elementIndex = 0; elementIndex < mLayers[i].cells[cellIndex].elementsCount; elementIndex++)
            {
                QJsonObject element = createElement(mLayers[i].cells[cellIndex].elements[elementIndex].elementId);
                elements.append(element);
            }

            cell["elements"] = elements;
            cells.append(cell);
        }

        layer["cellsCount"] = cellsCount;
        layer["cells"] = cells;
        layers.append(layer);
    }

    json["layersCount"] = layersCount;
    json["layers"] = layers;

    json["cellsCount"] = 560; // map->width() * map->height();
    QJsonArray cells;

    for (int i = 0; i < 560; i++)
        cells.append(createCellData());

    json["cells"] = cells;

    QJsonDocument output(json);
    file.write(output.toJson());
    file.close();

    return true;
}

QJsonObject DofusPlugin::createLayer(int layerId)
{
    QJsonObject layer;

    layer["layerId"] = layerId;
    layer["cellsCount"] = 0;
    layer["cells"] = QJsonArray();

    return layer;
}

QJsonObject DofusPlugin::createCell(int cellId)
{
    QJsonObject cell;

    cell["cellId"] = cellId;
    cell["elementsCount"] = 0;
    cell["elements"] = QJsonArray();

    return cell;
}

QJsonObject DofusPlugin::createElement(int elementId)
{
    QJsonObject element;

    element["elementName"] = "Graphical";
    element["elementId"] = elementId;
    element["hue_1"] = 0;
    element["hue_2"] = 0;
    element["hue_3"] = 0;
    element["shadow_1"] = 0;
    element["shadow_2"] = 0;
    element["shadow_3"] = 0;
    element["offsetX"] = 0;
    element["offsetY"] = 0;
    element["altitude"] = 0;
    element["identifier"] = 0;

    return element;
}

QJsonObject DofusPlugin::createCellData()
{
    QJsonObject cellData;

    cellData["floor"] = 0;
    cellData["losmov"] = 67; // ??
    cellData["speed"] = 0;
    cellData["mapChangeData"] = 0; // Triger ?
    cellData["moveZone"] = 0;
    cellData["tmpBits"] = 0;

    return cellData;
}

void DofusPlugin::writeLayer(Layer *layer)
{
    if (TileLayer *tileLayer = layer->asTileLayer())
    {
        int cellId = 0;

        for (int y = 0; y < mMap->height(); y++)
        {
            for (int x = 0; x < mMap->width(); x++)
            {
                Cell cell = tileLayer->cellAt(x, y);

                writeCell(&cell, layer->level(), cellId);
                mLayers[layer->level()].cellsCount = mLayers[layer->level()].cells.size();

                cellId++;
            }
        }
    }
}

void DofusPlugin::writeCell(Cell *cell, int layerId, int cellId)
{
    int index = -1;

    for (int i = 0; i < mLayers[layerId].cells.size(); i++)
    {
        if (mLayers[layerId].cells[i].cellId == cellId)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        struct t_cell newCell;
        newCell.cellId = cellId;
        newCell.elementsCount = 0;
        mLayers[layerId].cells.append(newCell);
        index = mLayers[layerId].cells.size() - 1;
    }

    writeElement(cell->tile, layerId, index);
    mLayers[layerId].cells[index].elementsCount = mLayers[layerId].cells[index].elements.size();
}

void DofusPlugin::writeElement(Tile *tile, int layerId, int cellIndex)
{
    if (tile)
    {
        int elementId = tile->property(QLatin1String("elementId")).toInt();

        if (elementId > 0)
        {
            struct t_element newElement;
            newElement.elementId = elementId;
            mLayers[layerId].cells[cellIndex].elements.append(newElement);
        }
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Dofus, DofusPlugin)
#endif
