// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include "decepticon.h"

namespace tf {

Decepticon::Decepticon(unsigned int level, unsigned int strength, unsigned int fuel, const Weapon& weapon, unsigned int malice, bool stealthMode)
    : Transformer(level, strength, fuel, weapon), _malice(malice), _stealthMode(stealthMode) {}

unsigned int Decepticon::getMalice() const {
    return _malice;
}

bool Decepticon::isStealthMode() const {
    return _stealthMode;
}

void Decepticon::scheme() {
    return;
}

} // namespace tf
