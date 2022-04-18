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

#include "RoomLink.h"

bool RoomLink::roomPosition(const QRect& thisRoom, RoomPosition roomPosition, const QRect& thatRoom, const int offset)
{
    // Valid rects
    Q_ASSERT(thisRoom.isValid());
    Q_ASSERT(thatRoom.isValid());

    // Rooms must be next to each other
    Q_ASSERT(offset >= 0);

    // No overlapping
    Q_ASSERT(thisRoom.intersects(thatRoom) == false);

    // Must be same dimensions (for now)
    Q_ASSERT(thisRoom.width() == thatRoom.width());
    Q_ASSERT(thisRoom.height() == thatRoom.height());

    switch (roomPosition)
    {
        case RoomPosition::LEFT:
            // Rooms must be next to each other
            if (thatRoom.x() - (thisRoom.x() + thisRoom.width()) == 0)
            {
                // Is it within the offset and not exceeding?
                auto const thisToThatRoomTopyOffset = std::abs(thatRoom.y() - thisRoom.y());
                if (thisToThatRoomTopyOffset < thisRoom.height() && (thisToThatRoomTopyOffset >=0 && thisToThatRoomTopyOffset <= offset))
                {
                    return true;
                }
            }
            break;
        case RoomPosition::RIGHT:
            // Rooms must be next to each other
            if (thisRoom.x() - (thatRoom.x() + thatRoom.width()) == 0)
            {
                // Is it within the offset and not exceeding?
                auto const thisToThatRoomTopyOffset = std::abs(thatRoom.y() - thisRoom.y());
                if (thisToThatRoomTopyOffset < thisRoom.height() && (thisToThatRoomTopyOffset >=0 && thisToThatRoomTopyOffset <= offset))
                {
                    return true;
                }
            }
            break;
        case RoomPosition::DOWN:
            // Rooms must be next to each other
            if (thisRoom.y() - (thatRoom.y() + thatRoom.height()) == 0)
            {
                // Is it within the offset and not exceeding?
                auto const thisToThatRoomTopyOffset = std::abs(thatRoom.x() - thisRoom.x());
                if (thisToThatRoomTopyOffset < thisRoom.width() && (thisToThatRoomTopyOffset >=0 && thisToThatRoomTopyOffset <= offset))
                {
                    return true;
                }
            }
            break;
        case RoomPosition::UP:
            // Rooms must be next to each other
            if (thatRoom.y() - (thisRoom.y() + thisRoom.height()) == 0)
            {
                // Is it within the offset and not exceeding?
                auto const thisToThatRoomTopyOffset = std::abs(thatRoom.x() - thisRoom.x());
                if (thisToThatRoomTopyOffset < thisRoom.width() && (thisToThatRoomTopyOffset >=0 && thisToThatRoomTopyOffset <= offset))
                {
                    return true;
                }
            }
            break;
        default:
            Q_ASSERT(false);
            return false;
    }
    return false;
}

RoomLink::RoomLinkMap RoomLink::linkRooms(const std::vector<QRect>& rooms, const int offset)
{
    // Rooms must be next to each other
    Q_ASSERT(offset >= 0);

    RoomLinkMap roomLinkMap;

    // Check no rooms overlap each other
    for (auto rit = rooms.cbegin(); rit != rooms.cend(); ++rit)
    {
        Q_ASSERT(rit->isValid());
        for (auto it = rooms.cbegin(); it != rit; ++it)
        {
            Q_ASSERT(rit->intersects(*it) == false);
        }
    }

    // Make links
    for (auto rit = rooms.cbegin(); rit != rooms.cend(); ++rit)
    {
        LinkedRoom roomLink;
        auto const currentRoom = *rit;

        for (auto it = rooms.cbegin(); it != rooms.cend(); ++it)
        {
            // Don't check yourself
            if (it != rit)
            {
                auto const checkRoom = *it;
                if (roomPosition(checkRoom, RoomPosition::LEFT, currentRoom, offset))
                {
                    Q_ASSERT(roomLink.left == std::optional<QRect>());
                    roomLink.left = checkRoom;
                }
                else if (roomPosition(checkRoom, RoomPosition::RIGHT, currentRoom, offset))
                {
                    Q_ASSERT(roomLink.right == std::optional<QRect>());
                    roomLink.right = checkRoom;
                }
                else if (roomPosition(checkRoom, RoomPosition::UP, currentRoom, offset))
                {
                    Q_ASSERT(roomLink.up == std::optional<QRect>());
                    roomLink.up = checkRoom;
                }
                else if (roomPosition(checkRoom, RoomPosition::DOWN, currentRoom, offset))
                {
                    Q_ASSERT(roomLink.down == std::optional<QRect>());
                    roomLink.down = checkRoom;
                }
            }
        }
        roomLinkMap[currentRoom] = roomLink;
    }

    return roomLinkMap;
}
