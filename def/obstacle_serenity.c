#include <Travelers.h>
#include <conditions.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
	::configure();
	set_creator("chaos");
	set_obstacle_name("serenity");
	set_obstacle_rarity(Rarity_Rare);
	set_obstacle_value(Travelers_Obstacle_Value_Moderate);
	set_challenge_component_selection_adjustments(([
		"experience"            : 1.00,
		"exploration"           : 0.50,
		"pilgrimage"            : 0.20,
		"quest"                 : 1.00,
		"skill experience"      : 0.50,
		"charity"               : 0.10,
		"neophyte guidance"     : 0.10,
	]));
	set_obstacle_eligibility_condition(Condition_Type_Emotion);
	set_obstacle_initialize_display(([
		Message_Content         : ({
			0, ({ "feel", 0 }), "all capacity for fear, anger, pain, and pleasure draining out of", ({ 'o', 0 }),
		}),
		Message_Color           : "status: loss",
		Message_Senses          : Message_Sense_Emotive,
	]));
	add_obstacle_trait_modifier(([
		Modifier_Index          : ({
			Trait_Analgesia,
			Trait_Anagonia,
			Trait_Anhedonia,
			Trait_Aphobia,
		}),
		Modifier_Amount         : 1,
		Modifier_Flags          : Modifier_Flag_Static | Modifier_Flag_Override,
	]));
	set_obstacle_overcome_display(([
		Message_Content         : ({
			0, ({ "feel", 0 }), "an interdiction of", ({ 'r', 0, "capacity" }), "for fear, anger, pain, and pleasure lifting",
		}),
		Message_Color           : "status: enhancement",
		Message_Senses          : Message_Sense_Emotive,
	]));
}
