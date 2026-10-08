#pragma once

#include "room.hpp"

#include <cstddef>
#include <iosfwd>

void printVisitedRooms(Room* const rooms[], std::size_t count, std::ostream& out);
Room* findMostVisitedRoom(Room* const rooms[], std::size_t count) noexcept;
void printMostVisitedRoom(Room* const rooms[], std::size_t count, std::ostream& out);
