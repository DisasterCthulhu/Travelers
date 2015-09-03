#ifndef _Travelers_Included
#define _Travelers_Included

#include <Guilds.h>

#define Travelers                                       Guilds "Travelers/"

#define Travelers_Armour(x)                             (Travelers + "arm/" + x)
#define Travelers_Comestible(x)                         (Travelers + "f&d/" + x)
#define Travelers_Daemon(x)                             (Travelers + "dmn/" + x)
#define Travelers_Data(x)                               (Travelers + "dat/" + x)
#define Travelers_Definition(x)                         (Travelers + "def/" + x)
#define Travelers_Extension(x)                          (Travelers + "ext/" + x)
#define Travelers_Help(x)                               (Travelers + "hlp/" + x)
#define Travelers_Misc(x)                               (Travelers + "etc/" + x)
#define Travelers_Module(x)                             (Travelers + "mod/" + x)
#define Travelers_Monster(x)                            (Travelers + "mon/" + x)
#define Travelers_NPC(x)                                (Travelers + "npc/" + x)
#define Travelers_Room(x)                               (Travelers + "rms/" + x)
#define Travelers_Text(x)                               (Travelers + "txt/" + x)
#define Travelers_Weapon(x)                             (Travelers + "wpn/" + x)

#define Travelers_Obstacle(x)                           Travelers_Daemon("dharma")->obstacle(x)
#define Is_Travelers_Obstacle(x)                        Travelers_Daemon("dharma")->is_obstacle(x)
#define Travelers_Requirement(x)                        Travelers_Daemon("dharma")->requirement(x)
#define Is_Travelers_Requirement(x)                     Travelers_Daemon("dharma")->is_requirement(x)
#define Travelers_Taboo(x)                              Travelers_Daemon("dharma")->taboo(x)
#define Is_Travelers_Taboo(x)                           Travelers_Daemon("dharma")->is_taboo(x)
#define Travelers_Forfeit(x)                            Travelers_Daemon("dharma")->forfeit(x)
#define Is_Travelers_Forfeit(x)                         Travelers_Daemon("dharma")->is_forfeit(x)
#define Travelers_Bestowal(x)                           Travelers_Daemon("dharma")->bestowal(x)
#define Is_Travelers_Bestowal(x)                        Travelers_Daemon("dharma")->is_bestowal(x)
#define Travelers_Invocation(x)                         Travelers_Daemon("dharma")->invocation(x)
#define Is_Travelers_Invocation(x)                      Travelers_Daemon("dharma")->is_invocation(x)

#define Travelers_Find_Challenge(x)                     Travelers_Daemon("dharma")->find_challenge(x)
#define Travelers_Find_Interdiction_Field(x)            Travelers_Daemon("dharma")->find_interdiction_field(x)

#define Ganesha_Query_Karma_Phala(x)                    Travelers_Daemon("dharma")->ganesha_query_karma_phala(x)
#define Ganesha_Set_Karma_Phala(x, y)                   Travelers_Daemon("dharma")->ganesha_set_karma_phala(x, y)
#define Ganesha_Add_Karma_Phala(x, y)                   Travelers_Daemon("dharma")->ganesha_add_karma_phala(x, y)

#define Travelers_Minimum_Goodness                      100
#define Travelers_Minimum_Orderliness                   100

#define Travelers_Rainbow                               ({\
    "red",\
    "violet",\
    "indigo",\
    "blue",\
    "green",\
    "yellow",\
    "orange",\
})

#define Travelers_Flower_Color                          Travelers_Daemon("control")->travelers_flower_color()

#define Travelers_Bestowal_Type_Recurring               0
#define Travelers_Bestowal_Type_Specialty_Access        1
#define Travelers_Bestowal_Type_Invocation              2
#define Travelers_Bestowal_Type_Attribute_Adjustment    3
#define Travelers_Bestowal_Type_Katakacha_Enhancement   4
#define Travelers_Bestowal_Type_Kabatha_Resistance      5
#define Travelers_Bestowal_Type_Singular                6

#define Travelers_Invocation_Ability_Minimum_Very_Low   0
#define Travelers_Invocation_Ability_Minimum_Low        100
#define Travelers_Invocation_Ability_Minimum_Moderate   200
#define Travelers_Invocation_Ability_Minimum_High       300
#define Travelers_Invocation_Ability_Minimum_Very_High  400

#define Travelers_Invocation_Ability_Maximum_Very_Low   300
#define Travelers_Invocation_Ability_Maximum_Low        400
#define Travelers_Invocation_Ability_Maximum_Moderate   500
#define Travelers_Invocation_Ability_Maximum_High       600
#define Travelers_Invocation_Ability_Maximum_Very_High  700

#define Travelers_Obstacle_Value_Very_Low               1
#define Travelers_Obstacle_Value_Low                    2
#define Travelers_Obstacle_Value_Moderate               3
#define Travelers_Obstacle_Value_High                   4
#define Travelers_Obstacle_Value_Very_High              5

#define Travelers_Requirement_Value_Very_Low            1
#define Travelers_Requirement_Value_Low                 2
#define Travelers_Requirement_Value_Moderate            3
#define Travelers_Requirement_Value_High                4
#define Travelers_Requirement_Value_Very_High           5

#define Travelers_Taboo_Value_Very_Low                  1
#define Travelers_Taboo_Value_Low                       2
#define Travelers_Taboo_Value_Moderate                  3
#define Travelers_Taboo_Value_High                      4
#define Travelers_Taboo_Value_Very_High                 5

#define Travelers_Forfeit_Value_Very_Low                1
#define Travelers_Forfeit_Value_Low                     2
#define Travelers_Forfeit_Value_Moderate                3
#define Travelers_Forfeit_Value_High                    4
#define Travelers_Forfeit_Value_Very_High               5

#define Travelers_Bestowal_Value_Very_Low               1
#define Travelers_Bestowal_Value_Low                    2
#define Travelers_Bestowal_Value_Moderate               3
#define Travelers_Bestowal_Value_High                   4
#define Travelers_Bestowal_Value_Very_High              5

#define Travelers_Challenge_Banned_Atmai                ({\
})

#endif
