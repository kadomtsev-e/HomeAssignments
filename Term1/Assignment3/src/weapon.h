// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#ifndef WEAPON_H
#define WEAPON_H

namespace tf {

class Weapon {
public:
    Weapon(unsigned int damage, unsigned int ammo);
    unsigned int getDamage() const;
    unsigned int getAmmo() const;
    void fire();
    void reload(unsigned int ammo);

private:
    unsigned int _damage;
    unsigned int _ammo;
};

} // namespace tf

#endif
