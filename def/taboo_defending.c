#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_defending_do_deflect(descriptor dxr) {
    object who = Attack_Query(dxr, Attack_Target);
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void travelers_taboo_defending_do_dodge(descriptor dxr) {
    object who = Attack_Query(dxr, Attack_Target);
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
    ::configure();
    set_taboo_name("defending");
    set_taboo_rarity(Rarity_Rare);
    set_taboo_value(Travelers_Taboo_Value_High);
    set_taboo_initialize_description("defending attacks by dodging them, parrying them, or otherwise deflecting them");
    set_taboo_overcome_description("defending");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 0.10,
        "experience"                            : 1.25,
        "exploration"                           : 0.10,
        "pilgrimage"                            : 0.10,
        "quest"                                 : 0.50,
        "skill experience"                      : 0.10,
        "neophyte guidance"                     : 0.50,
    ]));
    add_taboo_hook(Do_Deflect, #'travelers_taboo_defending_do_deflect);
    add_taboo_hook(Do_Dodge, #'travelers_taboo_defending_do_dodge);
}
