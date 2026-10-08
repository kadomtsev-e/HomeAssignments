#pragma once

#include "room.hpp"

#include <iosfwd>
#include <string>

class Guide
{
public:
    explicit Guide(std::string name);

    void goToRoom(Room* room);
    void printCurrentRoomInfo() const;
    void printCurrentRoomInfo(std::ostream& out) const;
    [[nodiscard]] Room* getCurrentRoom() const noexcept;

private:
    std::string name;
    Room* currentRoom = nullptr;
};
