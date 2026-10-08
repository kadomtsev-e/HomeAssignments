// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#ifndef DECEPTICON_H
#define DECEPTICON_H

#include "transformer.h"

namespace tf {

class Decepticon : public Transformer {
public:
    Decepticon(unsigned int level, unsigned int strength, unsigned int fuel, const Weapon& weapon, unsigned int malice, bool stealthMode);
    
    unsigned int getMalice() const;
    bool isStealthMode() const;
    void scheme();

private:
    unsigned int _malice;
    bool _stealthMode;
};

} // namespace tf

#endif
