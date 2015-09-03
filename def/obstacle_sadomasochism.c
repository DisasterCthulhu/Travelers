#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
	::configure();
	set_creator("chaos");
	set_obstacle_name("sadomasochism");
	set_obstacle_rarity(Rarity_Rare);
	set_obstacle_value(Travelers_Obstacle_Value_Low);
	set_challenge_component_selection_adjustments(([
		"experience"            : 1.05,
		"exploration"           : 0.50,
		"pilgrimage"            : 0.20,
		"quest"                 : 0.80,
		"skill experience"      : 0.50,
		"charity"               : 0.10,
		"neophyte guidance"     : 0.00,
	]));
	set_obstacle_initialize_display(([
		Message_Content         : ({
			0, ({ "feel", 0 }), "a jittery nervous energy come over", ({ 'o', 0 }),
		}),
		Message_Color           : "status: loss",
		Message_Senses          : Message_Sense_Emotive | Message_Sense_Kinesthetic,
	]));
	add_obstacle_trait_modifier(([
		Modifier_Index          : ({
			Trait_Masochism,
			Trait_Sadism,
		}),
		Modifier_Amount         : 20,
	]));
	set_obstacle_overcome_display(([
		Message_Content         : ({
			0, ({ "feel", 0 }), "the jittery nervous energy that had come over", ({ 'o', 0 }), "dissipate",
		}),
		Message_Color           : "status: enhancement",
		Message_Senses          : Message_Sense_Emotive | Message_Sense_Kinesthetic,
	]));
}
