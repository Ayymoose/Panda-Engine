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

#include "TilemapTests.h"
#include "Test.h"

void TilemapTest::runTests()
{
    {
        Tilemap::TilemapConfig config{16,16, 48};
        std::vector<QRect> rooms;
        rooms.emplace_back(0,0, 48, 16);

        QImage sourceImage(R"(..\Tilemap\Test Files\squares.png)");
        QImage resultImage(R"(..\Tilemap\Test Results\squares_16_16_48.png)");
        Q_ASSERT(!sourceImage.isNull());
        Q_ASSERT(!resultImage.isNull());

        auto const& [tileAreas, tileMapImage] = Tilemap::generate(sourceImage, config, rooms);
        Q_ASSERT(!tileMapImage.isNull());

        CHECK_EQUAL(tileMapImage, resultImage);
        CHECK_EQUAL(tileAreas.size(), 1);
        std::vector<size_t> indices = {0,1,2};
        CHECK_EQUAL(tileAreas.front().tileIndices, indices);
    }
    {
        Tilemap::TilemapConfig config{16,16, 16};
        std::vector<QRect> rooms;
        rooms.emplace_back(0,0, 48, 32);

        QImage sourceImage(R"(..\Tilemap\Test Files\squares.png)");
        QImage resultImage(R"(..\Tilemap\Test Results\squares_16_16_16.png)");
        Q_ASSERT(!sourceImage.isNull());
        Q_ASSERT(!resultImage.isNull());

        auto const& [tileAreas, tileMapImage] =  Tilemap::generate(sourceImage, config, rooms);
        Q_ASSERT(!tileMapImage.isNull());

        CHECK_EQUAL(tileMapImage, resultImage);
        CHECK_EQUAL(tileAreas.size(), 1);
        std::vector<size_t> indices = {0,1,2,1,0,2};
        CHECK_EQUAL(tileAreas.front().tileIndices, indices);

    }


}
