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

#include <QRect>
#include <QHash>
#include <optional>
#include <vector>

class RoomLink
{
public:

    struct LinkedRoom
    {
        std::optional<QRect> left;
        std::optional<QRect> right;
        std::optional<QRect> up;
        std::optional<QRect> down;
    };

    using RoomIndex = int;

    struct RoomLinkIndex
    {
        RoomIndex left;
        RoomIndex right;
        RoomIndex up;
        RoomIndex down;
    };

    using LinkedRoomMap = QHash<QRect, LinkedRoom>;
    using RoomLinkMap = std::unordered_map<RoomIndex, RoomLinkIndex>;

    // Given a set of rooms, links them together such that each room will be linked to [0,4] other rooms
    // Assumes that rooms do NOT overlap each other
    // Assumes rooms are NOT rotated at an angle
    // Assumes rooms are all the same size (TODO: Adapt to rooms of different sizes)
    // Returns a map for each room to a vector of rooms that it links to
    static LinkedRoomMap linkRooms(const std::vector<QRect>& rooms, const int offset);

    static RoomLinkMap associateRoomsWithLinks(const std::vector<QRect>& rooms, const LinkedRoomMap& linkedRoomMap);

    enum class RoomPosition
    {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    static bool roomPosition(const QRect& thisRoom, RoomPosition roomPosition, const QRect& thatRoom, const int offset);

};
