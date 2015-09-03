#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <daemon.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_speaking_other_than_by_shouting_do_speak(descriptor dxr) {
	if(Speech_Flag_Check(dxr, Speech_Flag_OOC) || Speech_Flag_Check(dxr, Speech_Flag_Gestural))
		return;
	if(Speech_Query(dxr, Speech_Command) == Speech_Command_Shout)
		return;
	object who = Speech_Query(dxr, Speech_Speaker);
	object challenge = Travelers_Find_Challenge(who);
	taboo_violation(challenge);
}

void configure() {
	::configure();
	set_creator("chaos");
	set_taboo_name("speaking other than by shouting");
	set_taboo_rarity(Rarity_Rare);
	set_taboo_value(Travelers_Taboo_Value_Moderate);
	set_taboo_eligibility_condition(([
		Condition_Type_Code                     : Condition_Type_Trait,
		Condition_Info                          : Trait_Muteness,
		Condition_Flags                         : Condition_Flag_Inverse,
	]));
	set_taboo_initialize_description("speaking other than by shouting");
	set_taboo_overcome_description("speaking other than by shouting");
	set_challenge_component_selection_adjustments(([
		"charity"                               : 0.50,
		"experience"                            : 1.00,
		"exploration"                           : 0.25,
		"pilgrimage"                            : 0.75,
		"quest"                                 : 1.00,
		"skill experience"                      : 1.25,
		"neophyte guidance"                     : 0.00,
		//
		"apophasis"                             : 0.10,
		"madness"                               : 0.00,
		"muteness"                              : 0.00,
		"deafness"                              : 0.25,
	]));
	add_taboo_hook(Do_Speak, #'travelers_taboo_speaking_other_than_by_shouting_do_speak);
}
