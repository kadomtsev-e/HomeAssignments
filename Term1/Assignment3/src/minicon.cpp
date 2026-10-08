// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include "minicon.h"

namespace tf {

MiniCon::MiniCon(unsigned int level, unsigned int strength, unsigned int fuel, const Weapon& weapon, unsigned int sizeClass, bool isLinked)
    : Transformer(level, strength, fuel, weapon), _sizeClass(sizeClass), _isLinked(isLinked) {}

unsigned int MiniCon::getSizeClass() const {
    return _sizeClass;
}

bool MiniCon::isLinked() const {
    return _isLinked;
}

void MiniCon::powerLink() {
    _isLinked = !_isLinked;
}

} // namespace tf
