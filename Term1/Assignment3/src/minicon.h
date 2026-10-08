// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#ifndef MINICON_H
#define MINICON_H

#include "transformer.h"

namespace tf {

class MiniCon : public Transformer {
public:
    MiniCon(unsigned int level, unsigned int strength, unsigned int fuel, const Weapon& weapon, unsigned int sizeClass, bool isLinked);
    
    unsigned int getSizeClass() const;
    bool isLinked() const;
    void powerLink();

private:
    unsigned int _sizeClass;
    bool _isLinked;
};

} // namespace tf

#endif
