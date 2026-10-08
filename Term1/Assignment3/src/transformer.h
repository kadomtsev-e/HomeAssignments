// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "weapon.h"
#include "pilot.h"

namespace tf {

class Transformer {
public:
    Transformer(unsigned int level, unsigned int strength, unsigned int fuel, const Weapon& weapon);
    virtual ~Transformer();
    
    // Non-copyable (Pilot* association could lead to confusion if copied)
    Transformer(const Transformer&) = delete;
    Transformer& operator=(const Transformer&) = delete;

    void setPilot(Pilot* pilot);
    Pilot* getPilot() const;

    unsigned int getLevel() const;
    unsigned int getStrength() const;
    unsigned int getFuel() const;
    
    // Actions
    bool move();
    bool jump();
    bool transform();
    bool fire();

protected:
    // Protected accessors for derived classes
    void setLevel(unsigned int level);
    void setStrength(unsigned int strength);
    void setFuel(unsigned int fuel);
    Weapon& getWeapon();
    const Weapon& getWeapon() const;

private:
    unsigned int _level;
    unsigned int _strength;
    unsigned int _fuel;
    Weapon _weapon;
    Pilot* _pilot;
};

} // namespace tf

#endif
