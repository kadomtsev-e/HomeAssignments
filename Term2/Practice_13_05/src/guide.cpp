#include "guide.hpp"

#include <iostream>
#include <ostream>
#include <utility>

Guide::Guide(std::string newName)
    : name(std::move(newName))
{
}

void Guide::goToRoom(Room* room)
{
    if (room == nullptr) {
        return;
    }
    currentRoom = room;
    currentRoom->visit();
}

void Guide::printCurrentRoomInfo() const
{
    printCurrentRoomInfo(std::cout);
}

void Guide::printCurrentRoomInfo(std::ostream& out) const
{
    if (currentRoom == nullptr) {
        out << "Группа пока не находится ни в одной комнате.\n";
        return;
    }
    currentRoom->printInfo(out);
}

Room* Guide::getCurrentRoom() const noexcept
{
    return currentRoom;
}
