#include <Travelers.h>
#include <daemon.h>
#include <usage.h>

inherit Travelers_Definition("Obstacle");

status traveler_obstacle_weapons_proscription_can_equip_item(mapping args) {
	if(args["usage"] != Usage_Hold)
		return True;
	object obj = args["what"];
	return Message(([
		Message_Content     : ({
			"a strange force settles between", 0, "and", obj, ", preventing", ({ 'o', 0 }),
			"from holding", ({ 'o', obj }),
		}),
		Message_Senses      : Message_Sense_Kinesthetic,
	]));
}

void configure() {
	::configure();
	set_obstacle_name("weapons proscription");
	set_obstacle_prepend_indefinite_article(True);
	set_obstacle_rarity(Rarity_Rare);
	set_obstacle_value(Travelers_Obstacle_Value_High);
	set_obstacle_eligibility_condition(([
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
	set_challenge_component_selection_adjustments(([
		"experience"            : 1.00,
		"exploration"           : 0.25,
		"pilgrimage"            : 0.25,
		"quest"                 : 1.25,
		"skill experience"      : 0.75,
		"charity"               : 0.10,
		"neophyte guidance"     : 1.25,
	]));
	set_obstacle_initialize_display(([
		Message_Content         : ({
			0, ({ "sense", 0 }), "a powerful force proscribing", ({ 'o', 0 }), "from using weapons of any sort"
		}),
		Message_Color           : "status: loss",
		Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
	]));
	add_obstacle_hook(Can_Equip_Item, #'traveler_obstacle_weapons_proscription_can_equip_item);
	set_obstacle_attach_process((:
		object who = $1->ganesha_challenge_query_owner();
		object array weapons = who->query_wielded_weapons();
		if(sizeof(weapons)) {
			who->display(([
				Message_Content     : ({
					"a strange force settles between", 0, "and", ({ 'j', weapons }), ", preventing", ({ 'o', 0 }),
					"from holding", ({ 'o', weapons }),
				}),
				Message_Senses      : Message_Sense_Kinesthetic,
			]));
			foreach(object weapon : weapons)
				who->deutilize_item(weapon, Move_Flags_Force);
		}
		return;
	:));
	set_obstacle_overcome_display(([
		Message_Content         : ({
			0, ({ "sense", 0 }), "the proscription against using weapons being lifted from", ({ 'o', 0 }),
		}),
		Message_Color           : "status: enhancement",
		Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
	]));
}
