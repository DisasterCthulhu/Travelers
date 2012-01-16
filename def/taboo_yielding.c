#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void configure() {
	::configure();
	set_taboo_name("yielding");
	set_taboo_rarity(Rarity_Unusual);
	set_taboo_value(Travelers_Taboo_Value_Moderate);
	set_taboo_initialize_description("yielding to this challenge");
	set_taboo_overcome_description("yielding");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 0.50,
        "experience"                            : 1.00,
        "exploration"                           : 1.00,
        "pilgrimage"                            : 2.00,
        "quest"                                 : 1.00,
        "skill experience"                      : 0.50,
        "neophyte guidance"                     : 3.00,
        //
        "magick proscription"                   : 0.00,
    ]));
}
