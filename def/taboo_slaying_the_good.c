#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_slaying_the_good_at_kill(mapping args) {
    object who = args["attacker"];
    object target = args["who"];
    if(target->query_alignment(Alignment_Good_Evil) < 1)
        return;
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
	::configure();
	set_taboo_name("slaying the good");
	set_taboo_rarity(Rarity_Unusual);
	set_taboo_value(Travelers_Taboo_Value_Low);
	set_taboo_initialize_description("participating in the killing of anyone of good alignment");
	set_taboo_overcome_description("slaying the good");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 0.05,
        "experience"                            : 1.25,
        "exploration"                           : 0.10,
        "pilgrimage"                            : 0.10,
        "quest"                                 : 0.75,
        "skill experience"                      : 0.05,
        "neophyte guidance"                     : 0.05,
    ]));
	add_taboo_hook(At_Kill, #'travelers_taboo_slaying_the_good_at_kill);
}
