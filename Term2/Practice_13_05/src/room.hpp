#pragma once

#include <iosfwd>
#include <string>

class Room
{
public:
    Room(std::string name, std::string type, int capacity);

    void printInfo() const;
    void printInfo(std::ostream& out) const;
    void visit() noexcept;
    [[nodiscard]] int getVisitCount() const noexcept;
    [[nodiscard]] const std::string& getName() const noexcept;

private:
    std::string name;
    std::string type;
    int capacity;
    int visitCount = 0;
};
