#include "guide.hpp"
#include "room.hpp"
#include "tour.hpp"

#include <iostream>
#include <limits>

namespace {

constexpr std::size_t kRoomCount = 5;

void printMenu()
{
    std::cout << "\n===== Экскурсия по офису Яндекса =====\n"
              << "1. Посетить комнату\n"
              << "2. Информация о текущей комнате\n"
              << "3. Показать все посещённые комнаты\n"
              << "4. Самая посещаемая комната\n"
              << "5. Выход\n"
              << "Выберите пункт: ";
}

bool readChoice(int& choice)
{
    if (std::cin >> choice) {
        return true;
    }
    if (std::cin.eof()) {
        return false;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Некорректный ввод.\n";
    return true;
}

void visitRoom(Guide& guide, Room* const rooms[])
{
    std::cout << "\nДоступные комнаты:\n";
    for (std::size_t index = 0; index < kRoomCount; ++index) {
        std::cout << index + 1 << ". " << rooms[index]->getName() << '\n';
    }
    std::cout << "Введите номер комнаты: ";

    int roomNumber = 0;
    if (!(std::cin >> roomNumber)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректный номер комнаты.\n";
        return;
    }
    if (roomNumber < 1 || roomNumber > static_cast<int>(kRoomCount)) {
        std::cout << "Некорректный номер комнаты.\n";
        return;
    }
    Room* room = rooms[static_cast<std::size_t>(roomNumber - 1)];
    guide.goToRoom(room);
    std::cout << "Гид провёл группу в комнату: " << room->getName() << '\n';
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
    Guide guide("Иван Валентинович");

    bool running = true;
    while (running) {
        printMenu();
        int choice = 0;
        if (!readChoice(choice)) {
            break;
        }
        switch (choice) {
            case 1:
                visitRoom(guide, rooms);
                break;
            case 2:
                guide.printCurrentRoomInfo();
                break;
            case 3:
                printVisitedRooms(rooms, kRoomCount, std::cout);
                break;
            case 4:
                printMostVisitedRoom(rooms, kRoomCount, std::cout);
                break;
            case 5:
                running = false;
                break;
            default:
                std::cout << "Неизвестный пункт меню.\n";
                break;
        }
    }

    for (Room* room : rooms) {
        delete room;
    }
    return 0;
}
