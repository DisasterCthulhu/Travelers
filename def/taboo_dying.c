#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_dying_do_die(mapping args) {
    object who = args["who"];
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
    ::configure();
    set_taboo_name("dying");
    set_taboo_rarity(Rarity_Very_Unusual);
    set_taboo_value(Travelers_Taboo_Value_Moderate);
    set_challenge_component_selection_adjustments(([
        "death"                                 : 0.00,
    ]));
    set_taboo_initialize_description("succumbing to death");
    set_taboo_overcome_description("dying");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 0.05,
        "experience"                            : 1.25,
        "exploration"                           : 0.10,
        "pilgrimage"                            : 0.50,
        "quest"                                 : 0.50,
        "skill experience"                      : 0.05,
    ]));
    add_taboo_hook(Do_Die, #'travelers_taboo_dying_do_die);
}
