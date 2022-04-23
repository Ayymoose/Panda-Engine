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

#include "RoomLinkTests.h"
#include "Test.h"

void RoomLinkTest::runTests()
{
    // Room position tests
    {
        {
            // Check when actually LEFT of thatRoom
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(160,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::LEFT, thatRoom, 0);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(165,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::LEFT, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(160,5,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::LEFT, thatRoom, 6);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(160,-5,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::LEFT, thatRoom, 6);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(160,5,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::LEFT, thatRoom, 3);
                CHECK_EQUAL(result, false);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(160,128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::LEFT, thatRoom, 128);
                CHECK_EQUAL(result, false);
            }
            // Check erroneous positions
            {
                // RIGHT
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(-160,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::LEFT, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {
                // UP
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(0,128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::LEFT, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {   // DOWN
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(0,-128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::LEFT, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
        }
        {
            // Check when actually RIGHT of thatRoom
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(-160,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::RIGHT, thatRoom, 0);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(-165,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::RIGHT, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(-160,5,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::RIGHT, thatRoom, 6);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(-160,-5,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::RIGHT, thatRoom, 6);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(-160,5,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::RIGHT, thatRoom, 3);
                CHECK_EQUAL(result, false);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(-160,128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::RIGHT, thatRoom, 128);
                CHECK_EQUAL(result, false);
            }
            // Check erroneous positions
            {
                // LEFT
                QRect thisRoom(-160,0,160,128);
                QRect thatRoom(0,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::RIGHT, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {
                // UP
                QRect thisRoom(0,-128,160,128);
                QRect thatRoom(0,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::RIGHT, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {   // DOWN
                QRect thisRoom(0,128,160,128);
                QRect thatRoom(0,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::RIGHT, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
        }
        {
            // Check when actually DOWN of thatRoom
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(0,-128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::DOWN, thatRoom, 0);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(0,-130,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::DOWN, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(5,-128, 160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::DOWN, thatRoom, 6);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(-5,-128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::DOWN, thatRoom, 6);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(5,-128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::DOWN, thatRoom, 3);
                CHECK_EQUAL(result, false);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(160,-128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::DOWN, thatRoom, 160);
                CHECK_EQUAL(result, false);
            }
            // Check erroneous positions
            {
                // LEFT
                QRect thisRoom(-160,0,160,128);
                QRect thatRoom(0,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::DOWN, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {
                // UP
                QRect thisRoom(0,-128,160,128);
                QRect thatRoom(0,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::DOWN, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {   // RIGHT
                QRect thisRoom(160,0,160,128);
                QRect thatRoom(0,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::DOWN, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
        }
        {
            // Check when actually UP of thatRoom
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(0,128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::UP, thatRoom, 0);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(0,130,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::UP, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(5,128, 160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::UP, thatRoom, 6);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(-5,128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::UP, thatRoom, 6);
                CHECK_EQUAL(result, true);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(5,128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::UP, thatRoom, 3);
                CHECK_EQUAL(result, false);
            }
            {
                QRect thisRoom(0,0,160,128);
                QRect thatRoom(160,128,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::UP, thatRoom, 160);
                CHECK_EQUAL(result, false);
            }
            // Check erroneous positions
            {
                // LEFT
                QRect thisRoom(-160,0,160,128);
                QRect thatRoom(0,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::UP, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {
                // DOWN
                QRect thisRoom(0,128,160,128);
                QRect thatRoom(0,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::UP, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
            {   // RIGHT
                QRect thisRoom(160,0,160,128);
                QRect thatRoom(0,0,160,128);
                bool result = RoomLink::roomPosition(thisRoom, RoomLink::RoomPosition::UP, thatRoom, 0);
                CHECK_EQUAL(result, false);
            }
        }
    }

    // Room link tests
    {
        {
            // Single room on its own
            std::vector<QRect> rooms =
            {
                {0,0,160,128},
            };

            auto const roomLinkMap = RoomLink::linkRooms(rooms, 0);
            REQUIRE_EQUAL(roomLinkMap.size(), 1);

            auto const room = roomLinkMap.constFind({0,0,160,128});
            Q_ASSERT(room != roomLinkMap.constEnd());

            CHECK_EQUAL((*room).down.has_value(), false);
            CHECK_EQUAL((*room).up.has_value(), false);
            CHECK_EQUAL((*room).right.has_value(), false);
            CHECK_EQUAL((*room).left.has_value(), false);
        }
        {
            // Unconnected rooms
            std::vector<QRect> rooms =
            {
                {0,0,160,128},
                {180,0,160,128},
            };

            auto const roomLinkMap = RoomLink::linkRooms(rooms, 0);
            REQUIRE_EQUAL(roomLinkMap.size(), 2);

            auto room = roomLinkMap.find({0,0,160,128});
            Q_ASSERT(room != roomLinkMap.end());

            CHECK_EQUAL((*room).down.has_value(), false);
            CHECK_EQUAL((*room).up.has_value(), false);
            CHECK_EQUAL((*room).right.has_value(), false);
            CHECK_EQUAL((*room).left.has_value(), false);

            room = roomLinkMap.find({180,0,160,128});
            Q_ASSERT(room != roomLinkMap.end());

            CHECK_EQUAL((*room).down.has_value(), false);
            CHECK_EQUAL((*room).up.has_value(), false);
            CHECK_EQUAL((*room).right.has_value(), false);
            CHECK_EQUAL((*room).left.has_value(), false);
        }
        {
            // Connected rooms
            std::vector<QRect> rooms =
            {
                {0,0,160,128},      // Center
                {160,0,160,128},    // Right
                {-160,0,160,128},   // Left
                {0,128,160,128},    // Down
                {0,-128,160,128}    // Up
            };

            auto const roomLinkMap = RoomLink::linkRooms(rooms, 0);
            REQUIRE_EQUAL(roomLinkMap.size(), 5);

            // Center room has all rooms
            auto room = roomLinkMap.find({0,0,160,128});
            Q_ASSERT(room != roomLinkMap.end());

            CHECK_EQUAL((*room).down.value(), QRect(0,128,160,128));
            CHECK_EQUAL((*room).up.value(), QRect(0,-128,160,128));
            CHECK_EQUAL((*room).right.value(), QRect(160,0,160,128));
            CHECK_EQUAL((*room).left.value(), QRect(-160,0,160,128));

            // Right room should only have center on left
            room = roomLinkMap.find({160,0,160,128});
            Q_ASSERT(room != roomLinkMap.end());

            CHECK_EQUAL((*room).down.has_value(), false);
            CHECK_EQUAL((*room).up.has_value(), false);
            CHECK_EQUAL((*room).right.has_value(), false);
            CHECK_EQUAL((*room).left.value(), QRect(0,0,160,128));

            // Left room should only have center on right
            room = roomLinkMap.find({-160,0,160,128});
            Q_ASSERT(room != roomLinkMap.end());

            CHECK_EQUAL((*room).down.has_value(), false);
            CHECK_EQUAL((*room).up.has_value(), false);
            CHECK_EQUAL((*room).right.value(), QRect(0,0,160,128));
            CHECK_EQUAL((*room).left.has_value(), false);

            // Up room should only have center on down
            room = roomLinkMap.find({0,-128,160,128});
            Q_ASSERT(room != roomLinkMap.end());

            CHECK_EQUAL((*room).down.value(), QRect(0,0,160,128));
            CHECK_EQUAL((*room).up.has_value(), false);
            CHECK_EQUAL((*room).right.has_value(), false);
            CHECK_EQUAL((*room).left.has_value(), false);

            // Down room should only have center on up
            room = roomLinkMap.find({0,128,160,128});
            Q_ASSERT(room != roomLinkMap.end());

            CHECK_EQUAL((*room).down.has_value(), false);
            CHECK_EQUAL((*room).up.value(), QRect(0,0,160,128));
            CHECK_EQUAL((*room).right.has_value(), false);
            CHECK_EQUAL((*room).left.has_value(), false);
        }
    }

    // Room link association tests
    {
        {
            // Single room on its own
            std::vector<QRect> rooms =
            {
                {0,0,160,128},
            };

            auto const linkedRoomMap = RoomLink::linkRooms(rooms, 0);
            REQUIRE_EQUAL(linkedRoomMap.size(), 1);

            auto const roomLinkMap = RoomLink::associateRoomsWithLinks(rooms, linkedRoomMap);
            REQUIRE_EQUAL(roomLinkMap.size(), 1);

            auto const key = roomLinkMap.find(0);
            auto const roomLink = key->second;
            CHECK_EQUAL(roomLink.down, -1);
            CHECK_EQUAL(roomLink.up, -1);
            CHECK_EQUAL(roomLink.left, -1);
            CHECK_EQUAL(roomLink.right, -1);
        }
        {
            // Unconnected rooms
            std::vector<QRect> rooms =
            {
                {0,0,160,128},
                {180,0,160,128},
            };

            auto const linkedRoomMap = RoomLink::linkRooms(rooms, 0);
            REQUIRE_EQUAL(linkedRoomMap.size(), 2);

            auto const roomLinkMap = RoomLink::associateRoomsWithLinks(rooms, linkedRoomMap);
            REQUIRE_EQUAL(roomLinkMap.size(), 2);

            auto key = roomLinkMap.find(0);
            auto roomLink = key->second;
            CHECK_EQUAL(roomLink.down, -1);
            CHECK_EQUAL(roomLink.up, -1);
            CHECK_EQUAL(roomLink.left, -1);
            CHECK_EQUAL(roomLink.right, -1);

            key = roomLinkMap.find(1);
            roomLink = key->second;
            CHECK_EQUAL(roomLink.down, -1);
            CHECK_EQUAL(roomLink.up, -1);
            CHECK_EQUAL(roomLink.left, -1);
            CHECK_EQUAL(roomLink.right, -1);
        }
        {
            // Connected rooms
            std::vector<QRect> rooms =
            {
                {0,0,160,128},      // Center
                {160,0,160,128},    // Right
                {-160,0,160,128},   // Left
                {0,128,160,128},    // Down
                {0,-128,160,128}    // Up
            };

            auto const linkedRoomMap = RoomLink::linkRooms(rooms, 0);
            REQUIRE_EQUAL(linkedRoomMap.size(), 5);

            auto const roomLinkMap = RoomLink::associateRoomsWithLinks(rooms, linkedRoomMap);
            REQUIRE_EQUAL(roomLinkMap.size(), 5);

            auto key = roomLinkMap.find(0);
            auto roomLink = key->second;

            // Center room has all rooms
            CHECK_EQUAL(roomLink.down, 3);
            CHECK_EQUAL(roomLink.up, 4);
            CHECK_EQUAL(roomLink.left, 2);
            CHECK_EQUAL(roomLink.right, 1);

            // Right room should only have center on left
            key = roomLinkMap.find(1);
            roomLink = key->second;

            CHECK_EQUAL(roomLink.down, -1);
            CHECK_EQUAL(roomLink.up, -1);
            CHECK_EQUAL(roomLink.left, 0);
            CHECK_EQUAL(roomLink.right, -1);

            // Left room should only have center on right
            key = roomLinkMap.find(2);
            roomLink = key->second;

            CHECK_EQUAL(roomLink.down, -1);
            CHECK_EQUAL(roomLink.up, -1);
            CHECK_EQUAL(roomLink.left, -1);
            CHECK_EQUAL(roomLink.right, 0);

            // Down room should only have center on up
            key = roomLinkMap.find(3);
            roomLink = key->second;

            CHECK_EQUAL(roomLink.down, -1);
            CHECK_EQUAL(roomLink.up, 0);
            CHECK_EQUAL(roomLink.left, -1);
            CHECK_EQUAL(roomLink.right, -1);

            // Up room should only have center on down
            key = roomLinkMap.find(4);
            roomLink = key->second;
            CHECK_EQUAL(roomLink.down, 0);
            CHECK_EQUAL(roomLink.up, -1);
            CHECK_EQUAL(roomLink.left, -1);
            CHECK_EQUAL(roomLink.right, -1);
        }
    }

}
