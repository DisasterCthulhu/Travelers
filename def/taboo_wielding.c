#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <daemon.h>
#include <daemon.h>

inherit Travelers_Definition("Taboo");

status travelers_taboo_wielding_do_equip_item(mapping args) {
	if(args["usage"] != Usage_Hold)
		return True;
	object who = args["who"];
	object challenge = Travelers_Find_Challenge(who);
	taboo_violation(challenge);
}

void configure() {
	::configure();
	set_creator("chaos");
	set_taboo_name("wielding");
	set_taboo_rarity(Rarity_Rare);
	set_taboo_value(Travelers_Taboo_Value_High);
	set_taboo_eligibility_condition(([
		Condition_Type_Code                     : Condition_Type_And,
		Condition_Info                          : ({
			([
				Condition_Type_Code             : Condition_Type_Limb,
				Condition_Info                  : Limb_Type_Hand,
			]),
			([
				Condition_Type_Code             : Condition_Type_Forced_Wield_Weapon,
				Condition_Flags                 : Condition_Flag_Inverse,
			]),
		}),
	]));
	set_taboo_initialize_description("wielding or holding any weapon or item");
	set_taboo_overcome_description("wielding");
	set_challenge_component_selection_adjustments(([
	    "charity"                               : 0.05,
		"experience"                            : 1.10,
		"exploration"                           : 0.20,
		"neophyte guidance"                     : 1.00,
		"pilgrimage"                            : 0.20,
		"quest"                                 : 1.20,
		//
		"weapons proscription"                  : 0.00,
	]));
	set_taboo_attach_process((:
		object who = $1->ganesha_challenge_query_owner();
		object array weapons = who->query_wielded_weapons();
		if(sizeof(weapons)) {
			who->display(([
				Message_Content                 : ({
					"a strange force settles between", 0, "and", ({ 'j', weapons }), ", preventing", ({ 'o', 0 }),
					"from holding", ({ 'o', weapons }),
				}),
				Message_Senses                  : Message_Sense_Kinesthetic,
			]));
			foreach(object weapon : weapons)
				who->deutilize_item(weapon, Move_Flags_Force);
		}
		return;
	:));
	add_taboo_hook(Do_Equip_Item, #'travelers_taboo_wielding_do_equip_item);
}
