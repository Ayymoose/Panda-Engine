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

#pragma once

#include <QImage>
#include <QRect>
#include <QtGlobal>
#include <QByteArray>
#include <QHash>
#include <QDebug>
#include <vector>

class Tilemap
{
public:

    // Config to use when creating a tilemap
    struct TilemapConfig
    {
        size_t tileX;           // Create tiles using width
        size_t tileY;           // Create tiles using height
        size_t outputMapWidth;  // Width of output tilemap (image)
    };

    using TileIndexMap = QHash<QImage, size_t>;
    using MapTiles = std::vector<QImage>;

    struct TileArea
    {
        size_t tilesAcross;
        size_t tilesDown;
        std::vector<size_t> tileIndices;

        friend QDebug operator<<(QDebug dbg, const TileArea& tileArea)
        {
            size_t index = 0;
            dbg.nospace();
            for (size_t ty = 0; ty < tileArea.tilesDown; ++ty)
            {
                dbg << '[';
                for (size_t tx = 0; tx < tileArea.tilesAcross-1; ++tx)
                {
                    Q_ASSERT(index < tileArea.tileIndices.size());
                    dbg << tileArea.tileIndices[index++];
                    dbg << ',';
                }
                Q_ASSERT(index < tileArea.tileIndices.size());
                dbg << tileArea.tileIndices[index++];
                dbg << ']';
                dbg << '\n';
            }
            return dbg;
        }
    };

    struct GeneratedTileMap
    {
        std::vector<TileArea> generatedAreas;
        QImage generatedTileMap;
    };

    // Create's a tilemap from sourceImage using config and areas
    static GeneratedTileMap generate(const QImage& sourceImage, const TilemapConfig& config, std::vector<QRect>& rooms);

private:
    static TileArea constructTilemap(TileIndexMap& tileIndexMap, MapTiles& mapTiles, const QImage& sourceImage, const TilemapConfig& config, const QRect& area, size_t& tileIndex);

};


// Hash the byte array of the QImage
inline uint qHash(const QImage& key)
{
    return qHash(QByteArray((const char*)key.bits(), key.sizeInBytes()));
}
