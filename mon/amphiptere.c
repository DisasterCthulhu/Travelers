#include <Travelers.h>
#include <autonomon.h>

inherit "/std/autonomon";

void configure() {
    ::configure();
    set_race("amphiptere");
    set_level(20);
    add_description(Description_Type_Generic);
    add_skill_points(Skill_Swimming, 80);
    add_skill_points(Skill_Flight, 80);
    add_skill_points(Skill_Hardiness, 80);
    add_skill_points(Skill_Stamina, 80);
    add_skill_points(Skill_Recuperation, 80);
    add_skill_points(Skill_Regeneration, 80);
}
