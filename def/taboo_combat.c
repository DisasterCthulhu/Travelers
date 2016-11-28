#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_combat_at_start_combat(object who) {
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
    ::configure();
    set_taboo_name("combat");
    set_taboo_rarity(Rarity_Very_Unusual);
    set_taboo_value(Travelers_Taboo_Value_High);
    set_taboo_initialize_description("combat of any sort");
    set_taboo_overcome_description("combat");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 0.75,
        "experience"                            : 0.00,
        "exploration"                           : 0.75,
        "pilgrimage"                            : 1.00,
        "quest"                                 : 1.00,
        "skill experience"                      : 1.00,
        "neophyte guidance"                     : 1.00,
    ]));
    add_taboo_hook(At_Start_Combat, #'travelers_taboo_combat_at_start_combat);
}
