// Copyright (C) 2022  Ayymooose

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Tilemap.h"
#include <QPainter>
#include <algorithm>

Tilemap::TileArea Tilemap::constructTilemap(TileIndexMap& tileIndexMap, MapTiles& mapTiles, const QImage& sourceImage, const TilemapConfig& config, const QRect& area, size_t& tileIndex)
{
    Q_ASSERT(!sourceImage.isNull());
    Q_ASSERT(area.width() % config.tileX == 0);
    Q_ASSERT(area.height() % config.tileY == 0);

    auto const tileX = config.tileX;
    auto const tileY = config.tileY;

    TileArea tileArea;
    tileArea.tilesAcross = area.width() / tileX;
    tileArea.tilesDown = area.height() / tileY;

    for (auto ty = area.top(); ty < area.height() + area.top(); ty += tileY)
    {
        for (auto tx = area.left(); tx < area.width() + area.left(); tx += tileX)
        {
            // Construct hash-map of tiles
            auto const sourceRect = QRect(tx, ty, tileX, tileY);
            Q_ASSERT(sourceImage.rect().contains(sourceRect));

            auto const sourceTile = sourceImage.copy(sourceRect);

            if (!tileIndexMap.contains(sourceTile))
            {
                tileIndexMap[sourceTile] = tileIndex;
                tileArea.tileIndices.push_back(tileIndex++);
                mapTiles.emplace_back(std::move(sourceTile));
            }
            else
            {
                tileArea.tileIndices.push_back(tileIndexMap[sourceTile]);
            }

        }
    }

    Q_ASSERT(tileArea.tileIndices.size() == tileArea.tilesAcross * tileArea.tilesDown);

    return tileArea;
}

Tilemap::GeneratedTileMap Tilemap::generate(const QImage& sourceImage, const TilemapConfig& config, std::vector<QRect>& rooms)
{
    Q_ASSERT(!sourceImage.isNull());

    qDebug() << "Creating tilemap from " << QString::number(rooms.size()) << " areas";

    TileIndexMap tileIndexMaps;
    MapTiles mapTiles;
    std::vector<TileArea> tileAreas;

    // Sort rooms first
    // The ordering we want is such that rooms closer to (0,0) via the row they are on are inserted first
    // This will allow us to generate a tilemap with each area in the same place as on the image
    std::sort(rooms.begin(), rooms.end(), [](const QRect& r1, const QRect& r2)
    {
        return (r1.top() < r2.top()) && (r1.left() < r2.left());
    });

    // Construct the tile set from all the areas
    size_t tileIndex = 0;
    for (auto const& room : rooms)
    {
        auto tileArea = constructTilemap(tileIndexMaps, mapTiles, sourceImage, config, room, tileIndex);
        qDebug() << tileArea;
        qDebug() <<  '\n';
        tileAreas.emplace_back(std::move(tileArea));
    }

    // if numTiles < numTilesPerRow -> tileHeight
    // if numTiles == numTilesPerRow -> tileHeight
    // if numTiles > numTilesPerRow -> tileHeight * ceil(numTiles / numTilesPerRow)

    // Now paint the output image
    auto const numberOfTiles = tileIndexMaps.size();
    auto const tileX = config.tileX;
    auto const tileY = config.tileY;
    auto const mapWidth = config.outputMapWidth;
    const qsizetype tilesPerRow = mapWidth / tileX;
    auto const mapHeight = (numberOfTiles <= tilesPerRow ? tileY : tileY * std::ceil(numberOfTiles / (float)tilesPerRow));

    qDebug() << QString::number(numberOfTiles) << " unique tiles in set";

    QImage destImage(mapWidth, mapHeight, QImage::Format_RGB32);
    QPainter painter(&destImage);

    size_t destX = 0;
    size_t destY = 0;

    for (auto const& tile : mapTiles)
    {
        auto const destPoint = QPoint(destX, destY);
        Q_ASSERT(destImage.rect().contains(destPoint));

        painter.drawImage(destPoint, tile, tile.rect());

        if (destX + tileX < mapWidth)
        {
           destX += tileX;
        }
        else
        {
           destX = 0;
           destY += tileY;
        }
    }

    return GeneratedTileMap{tileAreas, destImage};
}
