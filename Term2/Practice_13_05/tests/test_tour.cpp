#include "guide.hpp"
#include "room.hpp"
#include "tour.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace {

void require(const bool condition, const std::string& message)
{
    if (!condition) {
        std::cerr << "Test failed: " << message << '\n';
        std::exit(1);
    }
}

void require_contains(const std::string& text, const std::string& part, const std::string& message)
{
    require(text.find(part) != std::string::npos, message);
}

} // namespace

int main()
{
    Room* rooms[5] = {
        new Room("Переговорка Толстой", "переговорная", 12),
        new Room("Кухня 3 этаж", "кухня", 20),
        new Room("Open space Backend", "рабочая зона", 40),
        new Room("Зона отдыха", "отдых", 15),
        new Room("Серверная", "техническая", 5),
    };
    Guide guide("Тестовый гид");
    require(guide.getCurrentRoom() == nullptr, "guide starts without a current room");

    std::ostringstream initialRoom;
    guide.printCurrentRoomInfo(initialRoom);
    require_contains(
        initialRoom.str(),
        "Группа пока не находится ни в одной комнате.",
        "initial current-room message is exact");

    std::ostringstream noneVisited;
    printVisitedRooms(rooms, 5, noneVisited);
    require_contains(noneVisited.str(), "Пока не посещено ни одной комнаты.", "empty visited message is exact");

    std::ostringstream noMaximum;
    printMostVisitedRoom(rooms, 5, noMaximum);
    require_contains(noMaximum.str(), "Пока нет посещённых комнат.", "empty maximum message is exact");

    guide.goToRoom(rooms[1]);
    guide.goToRoom(rooms[3]);
    guide.goToRoom(rooms[1]);
    require(guide.getCurrentRoom() == rooms[1], "current room updates after a visit");
    require(rooms[1]->getVisitCount() == 2, "repeat visits increment the counter");
    require(rooms[3]->getVisitCount() == 1, "another room has its own counter");
    require(findMostVisitedRoom(rooms, 5) == rooms[1], "most visited room is selected");

    std::ostringstream visited;
    printVisitedRooms(rooms, 5, visited);
    require_contains(visited.str(), "Кухня 3 этаж", "visited list contains visited room");
    require_contains(visited.str(), "Зона отдыха", "visited list contains second visited room");
    require(visited.str().find("Серверная") == std::string::npos, "visited list excludes unvisited rooms");

    std::ostringstream maximum;
    printMostVisitedRoom(rooms, 5, maximum);
    require_contains(maximum.str(), "Самая посещаемая комната: Кухня 3 этаж", "maximum output names room");
    require_contains(maximum.str(), "Количество посещений: 2", "maximum output includes count");

    guide.goToRoom(nullptr);
    require(guide.getCurrentRoom() == rooms[1], "null destination does not change current room");

    for (Room* room : rooms) {
        delete room;
    }

    std::cout << "All office-tour tests passed.\n";
    return 0;
}
