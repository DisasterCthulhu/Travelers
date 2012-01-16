#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <conceptual_navigation.h>
#include <extant.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_conceptual_navigation_at_conceptual_navigation_terminate(descriptor dxr) {
    object postulant = Extant_Find(Conceptual_Navigation_Theorem_Query(dxr, Conceptual_Navigation_Theorem_Postulant));
//    object array corollaries = Conceptual_Navigation_Theorem_Query(dxr, Conceptual_Navigation_Theorem_Corollaries) || ({});
    foreach(object who : ({ postulant }) /* + corollaries */) {
        object challenge = Travelers_Find_Challenge(who);
        if(challenge) {
            if(challenge->ganesha_challenge_taboo() == this_object()) {
                taboo_violation(challenge);
            }
        }
    }
}

void configure() {
	::configure();
	set_taboo_name("conceptual navigation");
	set_taboo_rarity(Rarity_Unusual);
	set_taboo_value(Travelers_Taboo_Value_Low);
	set_taboo_initialize_description("using conceptual navigation");
	set_taboo_overcome_description("conceptual navigation");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 1.00,
        "decontextualization"                   : 0.00,
        "experience"                            : 1.00,
        "exploration"                           : 1.50,
        "pilgrimage"                            : 0.00,
        "quest"                                 : 0.75,
        "skill experience"                      : 1.00,
        "neophyte guidance"                     : 1.00,
    ]));
	add_taboo_hook("at_conceptual_navigation_terminate", #'travelers_taboo_conceptual_navigation_at_conceptual_navigation_terminate);
}
