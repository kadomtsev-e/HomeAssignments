// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include <gtest/gtest.h>
#include "../src/transformer.h"
#include "../src/autobot.h"
#include "../src/decepticon.h"
#include "../src/minicon.h"
#include "../src/weapon.h"
#include "../src/pilot.h"

using namespace tf;

TEST(TransformerTest, Initialization) {
    Weapon w(50, 10);
    Transformer t(1, 100, 50, w);
    EXPECT_EQ(t.getLevel(), 1);
    EXPECT_EQ(t.getStrength(), 100);
}

TEST(TransformerTest, PilotAssociation) {
    Weapon w(50, 10);
    Transformer t(1, 100, 50, w);
    Pilot p("Spike");
    
    t.setPilot(&p);
    EXPECT_EQ(t.getPilot()->getName(), "Spike");
}

TEST(TransformerTest, WeaponUsage) {
    Weapon w(50, 1);
    Transformer t(1, 100, 50, w);
    // Initial ammo check is hard because getWeapon isn't exposed in Transformer, 
    // but we can verify fire() logic if we exposed weapon or check side effects.
    // Given the constraints, just checking simple methods.
    EXPECT_TRUE(t.fire());
}

TEST(AutobotTest, Specifics) {
    Weapon w(20, 5);
    Autobot opt(5, 200, 100, w, 90, true);
    EXPECT_EQ(opt.getEmpathy(), 90);
    EXPECT_TRUE(opt.isGuardianMode());
}

TEST(DecepticonTest, Specifics) {
    Weapon w(30, 10);
    Decepticon meg(10, 500, 200, w, 100, true);
    EXPECT_EQ(meg.getMalice(), 100);
    EXPECT_TRUE(meg.isStealthMode());
}

TEST(MiniConTest, Specifics) {
    Weapon w(5, 2);
    MiniCon m(1, 20, 10, w, 1, false);
    EXPECT_FALSE(m.isLinked());
    m.powerLink();
    EXPECT_TRUE(m.isLinked());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
