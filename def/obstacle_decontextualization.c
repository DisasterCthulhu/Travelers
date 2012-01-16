#include <Travelers.h>
#include <conceptual_navigation.h>
#include <daemon.h>
#include <usage.h>

inherit Travelers_Definition("Obstacle");

void traveler_obstacle_decontextualization_do_autoheal(object who) {
	unless(!random(20))
		return;
	if(who->query_attacker() && !random(2))
		return;
	if(who->query_mortal_wound())
		return;
	if(query_any_realm("NT", who))
		return;
	if(query_input_pending(who))
		return;
	object env = who->query_parent_object();
	if(!env)
		return;
	string color = Travelers_Flower_Color;
	env->message(([
		Message_Content                             : ({
			"{{" + color + "}colorful flower petals} materialize and swirl through the area"
		}),
		Message_Senses                              : Message_Sense_Visual | Message_Sense_Astral,
	]));
	Conceptual_Navigation_Start(([
		Conceptual_Navigation_Theorem_Postulant     : who,
		Conceptual_Navigation_Theorem_Objective     : Random_Location_Safe,
		Conceptual_Navigation_Theorem_Exit_Message  : ({
			", enveloped by the {{" + color + "}petals}",
		}),
		Conceptual_Navigation_Theorem_Enter_Message : ({
			"in a {{" + color + "}swirl of colorful flower petals}",
		}),
	]));
	env->message(([
		Message_Content                             : ({
			"the {{" + color + "}flower petals} vanish into the surrounding",
			Description(Description_Type_Ambient_Medium_Colored_Name),
			"with a final breezy twist"
		}),
		Message_Senses                              : Message_Sense_Visual | Message_Sense_Astral,
	]));
}

void configure() {
	::configure();
	set_creator("chaos");
	set_obstacle_name("decontextualization");
	set_obstacle_rarity(Rarity_Very_Unusual);
	set_obstacle_value(Travelers_Obstacle_Value_High);
	set_challenge_component_selection_adjustments(([
		"experience"                                : 1.00,
		"exploration"                               : 1.00,
		"pilgrimage"                                : 0.00,
		"quest"                                     : 0.00,
		"skill experience"                          : 1.00,
		"charity"                                   : 1.00,
		"neophyte guidance"                         : 0.00,
	]));
	set_obstacle_initialize_display(([
		Message_Content                             : ({
			0, ({ "feel", 0 }), "a sudden disorientation, as if the question of where", ({ 'o', 0 }), ({ "are", 0 }),
			"is ambiguous and ill-defined",
		}),
		Message_Color                               : "status: loss",
		Message_Senses                              : Message_Sense_Cognitive,
	]));
	add_obstacle_hook(Do_Autoheal, #'traveler_obstacle_decontextualization_do_autoheal);
	set_obstacle_overcome_display(([
		Message_Content                             : ({
			({ 's', 0, "sense of disorientation" }), "fades, leaving", ({ 'o', 0 }), "feeling firmly grounded once more",
		}),
		Message_Color                               : "status: enhancement",
		Message_Senses                              : Message_Sense_Cognitive,
	]));
}
