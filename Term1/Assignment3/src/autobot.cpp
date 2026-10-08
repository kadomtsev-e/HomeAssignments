// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include "autobot.h"

namespace tf {

Autobot::Autobot(unsigned int level, unsigned int strength, unsigned int fuel, const Weapon& weapon, unsigned int empathy, bool guardianMode)
    : Transformer(level, strength, fuel, weapon), _empathy(empathy), _guardianMode(guardianMode) {}

unsigned int Autobot::getEmpathy() const {
    return _empathy;
}

bool Autobot::isGuardianMode() const {
    return _guardianMode;
}

void Autobot::rollOut() {
    return;
}

} // namespace tf
