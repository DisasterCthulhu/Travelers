#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
	::configure();
	set_obstacle_name("apophasis");
	set_obstacle_rarity(Rarity_Very_Rare);
	set_obstacle_value(Travelers_Obstacle_Value_High);
	set_challenge_component_selection_adjustments(([
	    "celebration"           : 0.00,
		"experience"            : 0.75,
		"exploration"           : 0.50,
		"pilgrimage"            : 1.00,
		"quest"                 : 1.25,
		"skill experience"      : 2.00,
		"charity"               : 1.00,
		"neophyte guidance"     : 0.00,
	]));
	set_obstacle_initialize_display(([
		Message_Content         : ({
			0, ({ "are", 0 }), "overwhelmed by a sudden inability to use or understand language, or to even grasp a concept for more than a fleeting moment",
		}),
		Message_Color           : "status: loss",
		Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
	]));
	add_obstacle_scaled_attribute_modifier(([
		Modifier_Index          : ({
			Attribute_Intelligence,
		}),
		Modifier_Amount         : 0.95,
	]));
	add_obstacle_scaled_skill_modifier(([
		Modifier_Index          : ({
			Skill_Linguistics,
		}),
		Modifier_Amount         : 0.95,
	]));
	add_obstacle_scaled_skill_modifier(([
		Modifier_Index          : Skill_Class(Skill_Class_Language)->query_skill_class_contents()->query_skill_code(),
		Modifier_Amount         : 1.50,
	]));
	set_obstacle_overcome_display(([
		Message_Content         : ({
			0, ({ "feel", 0 }), ({ 'r', 0, "linguistic and conceptual powers", }), "returning in a heady rush",
		}),
		Message_Color           : "status: enhancement",
		Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
	]));
}
