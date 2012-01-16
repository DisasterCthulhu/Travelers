#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_swimming_do_move(mapping args) {
    object who = args["what"];
    if(who->query_move_mode() == "swim") {
        object challenge = Travelers_Find_Challenge(who);
        taboo_violation(challenge);
    }
}

void configure() {
	::configure();
	set_taboo_name("swimming");
	set_taboo_rarity(Rarity_Very_Unusual);
	set_taboo_value(Travelers_Taboo_Value_Moderate);
    set_taboo_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_Terrain,
        Condition_Info                          : Terrain_Water,
        Condition_Flags                         : Condition_Flag_Inverse,
    ]));
	set_taboo_initialize_description("swim or otherwise become submerged in water");
	set_taboo_overcome_description("swimming");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 1.00,
        "experience"                            : 1.00,
        "exploration"                           : 0.25,
        "pilgrimage"                            : 1.00,
        "quest"                                 : 1.00,
        "skill experience"                      : 0.50,
        "neophyte guidance"                     : 1.00,
        //
        "flight proscription"                   : 2.50,
    ]));
	add_taboo_hook(Do_Move, #'travelers_taboo_swimming_do_move);
}
