#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_deflecting_do_deflect(descriptor dxr) {
    object who = Attack_Query(dxr, Attack_Target);
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
	::configure();
	set_taboo_name("deflecting");
	set_taboo_rarity(Rarity_Unusual);
	set_taboo_value(Travelers_Taboo_Value_Very_Low);
	set_taboo_initialize_description("deflecting attacks");
	set_taboo_overcome_description("deflecting");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 0.10,
        "experience"                            : 1.00,
        "exploration"                           : 0.20,
        "pilgrimage"                            : 0.10,
        "quest"                                 : 0.25,
        "skill experience"                      : 0.10,
        "neophyte guidance"                     : 0.50,
    ]));
	add_taboo_hook(Do_Deflect, #'travelers_taboo_deflecting_do_deflect);
}
