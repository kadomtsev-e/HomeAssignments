#include "room.hpp"

#include <iostream>
#include <ostream>
#include <utility>

Room::Room(std::string newName, std::string newType, const int newCapacity)
    : name(std::move(newName))
    , type(std::move(newType))
    , capacity(newCapacity)
{
}

void Room::printInfo() const
{
    printInfo(std::cout);
}

void Room::printInfo(std::ostream& out) const
{
    out << "Название: " << name << '\n'
        << "Тип: " << type << '\n'
        << "Вместимость: " << capacity << '\n'
        << "Количество посещений: " << visitCount << '\n';
}

void Room::visit() noexcept
{
    ++visitCount;
}

int Room::getVisitCount() const noexcept
{
    return visitCount;
}

const std::string& Room::getName() const noexcept
{
    return name;
}
