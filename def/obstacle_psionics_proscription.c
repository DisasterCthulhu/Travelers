#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
	::configure();
	set_obstacle_name("psionics proscription");
	set_obstacle_prepend_indefinite_article(True);
	set_obstacle_rarity(Rarity_Very_Rare);
	set_obstacle_value(Travelers_Obstacle_Value_Moderate);
	set_challenge_component_selection_adjustments(([
		"experience"            : 1.00,
		"exploration"           : 1.00,
		"pilgrimage"            : 0.50,
		"quest"                 : 1.00,
		"skill experience"      : 0.10,
		"charity"               : 0.00,
		"neophyte guidance"     : 1.00,
	]));
	set_obstacle_eligibility_condition(([
		Condition_Type_Code                     : Condition_Type_Or,  
		Condition_Info                          : ({
			([
				Condition_Type_Code             : Condition_Type_Affiliation,
				Condition_Info                  : "the Wild Talents",
			]),
			([
				Condition_Type_Code             : Condition_Type_Affiliation,
				Condition_Info                  : "the Void Paladins",
			]),
		}),
	]));   
	set_obstacle_initialize_display(([
		Message_Content         : ({
			0, ({ "sense", 0 }), "a powerful force proscribing", ({ 'o', 0 }), "and anyone around", ({ 'o', 0 }),
			"from using psionics",
		}),
		Message_Color           : "status: loss",
		Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
	]));
	set_obstacle_initialize_process((:
		object who = $1->ganesha_challenge_query_owner();
		object field = Travelers_Find_Interdiction_Field(who);
		if(field) {
			string array realms = field->ganesha_interdiction_field_query_realms();
			realms += ({ "NP" });
			field->ganesha_interdiction_field_set_realms(realms);
		} else {
			field = new(Travelers_Misc("ganesha_interdiction_field"));
			field->ganesha_interdiction_field_set_realms(({ "NP" }));
			field->ganesha_interdiction_field_initialize(who);
		}
		field->set_info(query_obstacle_modifier_tag(), True);
		return;
	:));
	set_obstacle_overcome_process((:
		object who = $1->ganesha_challenge_query_owner();
		object array interdiction_fields = filter(all_inventory(who), (: $1->is_ganesha_interdiction_field() :));
		foreach(object interdiction_field : interdiction_fields)
			if(interdiction_field->query_info(query_obstacle_modifier_tag()))
				interdiction_field->ganesha_interdiction_field_expire();
		return;
	:));
	set_obstacle_overcome_display(([
		Message_Content         : ({
			0, ({ "sense", 0 }), "the proscription against using psionics being lifted from", ({ 'o', 0 }),
		}),
		Message_Color           : "status: enhancement",
		Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
	]));
}
