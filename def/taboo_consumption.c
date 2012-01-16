#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_consumption_at_comestible_consumed(mapping args) {
    int operation = args["operation"];
    unless(operation & (Comestible_Operation_Drink | Comestible_Operation_Eat | Comestible_Operation_Swallow))
        return;
    object who = args["who"];
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
	::configure();
	set_taboo_name("consumption");
	set_taboo_rarity(Rarity_Unusual);
	set_taboo_value(Travelers_Taboo_Value_Low);
    set_taboo_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_Trait,
        Condition_Info                          : Trait_Diet,
        Condition_Value                         : Diet_Ametabolic,
        Condition_Flags                         : Condition_Flag_Inverse,
    ]));
	set_taboo_initialize_description("ingesting any food or drink");
	set_taboo_overcome_description("consumption");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 0.25,
        "experience"                            : 0.75,
        "exploration"                           : 0.25,
        "pilgrimage"                            : 1.50,
        "quest"                                 : 0.25,
        "skill experience"                      : 0.25,
        "neophyte guidance"                     : 0.10,
    ]));
	add_taboo_hook(At_Comestible_Consumed, #'travelers_taboo_consumption_at_comestible_consumed);
}
