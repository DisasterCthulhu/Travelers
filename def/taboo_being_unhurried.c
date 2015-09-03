#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_being_unhurried_at_heart_beat(object who) {
    object challenge = Travelers_Find_Challenge(who);
    int time_taken = challenge->query_info("Time_Taken") + 2;
    challenge->set_info("Time_Taken", time_taken);
    if(time_taken > Time_Hour)
        taboo_violation(challenge);
}

void configure() {
	::configure();
	set_taboo_name("being unhurried");
	set_taboo_rarity(Rarity_Very_Rare);
	set_taboo_value(Travelers_Taboo_Value_Very_High);
	set_taboo_initialize_description("being unhurried -- that is, against taking longer than an hour to finish this challenge");
	set_taboo_overcome_description("being unhurried");
	set_taboo_eligibility_condition(([
	    Condition_Type_Code                     : Condition_Type_Trait,
	    Condition_Info                          : Trait_Assimilativity,
	    Condition_Value                         : -20,
	]));
    set_challenge_component_selection_adjustments(([
        "charity"                               : 1.00,
        "experience"                            : 1.00,
        "exploration"                           : 1.00,
        "pilgrimage"                            : 0.00,
        "quest"                                 : 1.00,
        "skill experience"                      : 1.00,
        "neophyte guidance"                     : 0.00,
    ]));
	add_taboo_hook(At_Heart_Beat, #'travelers_taboo_being_unhurried_at_heart_beat);
}
