#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Forfeit");

object travelers_forfeit_banishment_query_banishment_location() {
	return Random_Location(({
		Select_Include_Danger(#'>=, Location_Danger_Moderate),
		Select_Include_Egress(#'>=, Location_Egress_Difficult),
		Select_Adjust_Danger(#'<, Location_Danger_Moderate, 0.02),
		Select_Adjust_Danger(#'==, Location_Danger_Moderate, 0.10),
		Select_Adjust_Plane("the Prime", 0.10),
		Select_Adjust_Plane("the Exoma", 0.50),
	}));
}

void configure() {
	::configure();
	set_creator("chaos");
	set_forfeit_name("banishment");
	set_forfeit_rarity(Rarity_Very_Unusual);
	set_forfeit_value(Travelers_Forfeit_Value_Very_Low);
	set_forfeit_initialize_description("banishment");
	set_challenge_component_selection_adjustments(([
		"conceptual navigation"                 : 0.00,
	]));
	set_forfeit_process((:
		object who = $1->ganesha_challenge_query_owner();
		object where;
		do {
			where = travelers_forfeit_banishment_query_banishment_location();
		} while(where->query_area() == who->query_parent_object()->query_area());
		who->move_living(where);
		who->query_parent_object()->add_realm_modifier(([
			Modifier_Index                      : ({ "NT" }),
			Modifier_Duration                   : semirandom(60),
			Modifier_Damage_Type                : ({ "goetia" }),
		]));
		return;
	:));
}
