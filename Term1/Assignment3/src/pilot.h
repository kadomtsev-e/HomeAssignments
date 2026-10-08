// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#ifndef PILOT_H
#define PILOT_H

#include <string>

namespace tf {

class Pilot {
public:
    Pilot(const std::string& name);
    std::string getName() const;
    void command();

private:
    std::string _name;
};

} // namespace tf

#endif
