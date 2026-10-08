// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include "weapon.h"

namespace tf {

Weapon::Weapon(unsigned int damage, unsigned int ammo) : _damage(damage), _ammo(ammo) {}

unsigned int Weapon::getDamage() const {
    return _damage;
}

unsigned int Weapon::getAmmo() const {
    return _ammo;
}

void Weapon::fire() {
    if (_ammo > 0) {
        _ammo--;
    }
}

void Weapon::reload(unsigned int ammo) {
    _ammo += ammo;
}

} // namespace tf
