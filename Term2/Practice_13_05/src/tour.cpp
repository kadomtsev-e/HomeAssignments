#include "tour.hpp"

#include <ostream>

void printVisitedRooms(Room* const rooms[], const std::size_t count, std::ostream& out)
{
    bool found = false;
    for (std::size_t index = 0; index < count; ++index) {
        if (rooms[index] != nullptr && rooms[index]->getVisitCount() > 0) {
            if (!found) {
                out << "Посещённые комнаты:\n";
            }
            rooms[index]->printInfo(out);
            found = true;
        }
    }
    if (!found) {
        out << "Пока не посещено ни одной комнаты.\n";
    }
}

Room* findMostVisitedRoom(Room* const rooms[], const std::size_t count) noexcept
{
    Room* best = nullptr;
    for (std::size_t index = 0; index < count; ++index) {
        Room* room = rooms[index];
        if (room != nullptr && room->getVisitCount() > 0 &&
            (best == nullptr || room->getVisitCount() > best->getVisitCount())) {
            best = room;
        }
    }
    return best;
}

void printMostVisitedRoom(Room* const rooms[], const std::size_t count, std::ostream& out)
{
    Room* best = findMostVisitedRoom(rooms, count);
    if (best == nullptr) {
        out << "Пока нет посещённых комнат.\n";
        return;
    }
    out << "Самая посещаемая комната: " << best->getName() << '\n'
        << "Количество посещений: " << best->getVisitCount() << '\n';
}
