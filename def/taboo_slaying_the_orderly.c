#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_slaying_the_orderly_at_kill(mapping args) {
    object who = args["attacker"];
    object target = args["who"];
    if(target->query_alignment(Alignment_Order_Chaos) < 1)
        return;
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
	::configure();
	set_taboo_name("slaying the orderly");
	set_taboo_rarity(Rarity_Unusual);
	set_taboo_value(Travelers_Taboo_Value_Low);
	set_taboo_initialize_description("participating in the killing of anyone of ordered alignment");
	set_taboo_overcome_description("slaying the orderly");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 0.05,
        "experience"                            : 1.25,
        "exploration"                           : 0.10,
        "pilgrimage"                            : 0.10,
        "quest"                                 : 0.75,
        "skill experience"                      : 0.05,
        "neophyte guidance"                     : 0.05,
    ]));
	add_taboo_hook(At_Kill, #'travelers_taboo_slaying_the_orderly_at_kill);
}
