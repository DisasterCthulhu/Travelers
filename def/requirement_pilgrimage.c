#include <Travelers.h>
#include <daemon.h>
#include <Nexus.h>
#include <Caves_of_Sionell.h>
#include <Vanishing_Tower.h>
#include <Syllyac.h>
#include <Crystal_Cave.h>
#include <Terrace.h>
#include <Corpore_Scyros.h>
#include <Liathyr.h>
#include <Og.h>
#include <Temple_of_Ganesha.h>
#include <locations.h>

inherit Travelers_Definition("Requirement");

void travelers_requirement_pilgrimage_do_move(mapping args) {
	object challenge = Travelers_Find_Challenge(args["what"]);
	if(args["to"]->query_realm("Temple_of_Ganesha"))
		challenge->ganesha_challenge_overcome();
}

void configure() {
	::configure();
	set_requirement_name("pilgrimage");
	set_requirement_rarity(Rarity_Exotic);
	set_requirement_value(Travelers_Requirement_Value_Very_High);
	set_requirement_initialize_description("making pilgrimage to the temple of Ganesha");
	set_requirement_overcome_description("successfully completed your pilgrimage");
	set_requirement_eligibility_condition(([
		Condition_Type_Code                     : Condition_Type_Level,
		Condition_Value                         : 25,
	]));
	set_requirement_initialize_process((:
		object who = $1->ganesha_challenge_query_owner();
		string maze_room = "maze" + printable(1 + random(8));
		mapping destinations = ([
			Crystal_Cave_Room("rectangle")                                      : Rarity_Very_Common,
			Nexus_Room("statue")                                                : Rarity_Very_Common,
			Caves_of_Sionell_Room("map_0_3_-1")                                 : Rarity_Common,
			Syllyac_Room(maze_room)                                             : Rarity_Common,
			Og_Room("intersection5")                                            : Rarity_Unusual,
			Terrace_Room("larder")                                              : Rarity_Unusual,
			Vanishing_Tower_Room("ante")                                        : Rarity_Very_Unusual,
			Corpore_Scyros_Room("map_0_0_0")                                    : Rarity_Very_Unusual,
			Random_Location(({ Select_Include(Select_Realm("Spiderwood")) }))   : Rarity_Very_Unusual,
			Liathyr_Room("liathyr_-4e_-8n_-1u")                                 : Rarity_Very_Unusual,
		]);
		string destination = Rarity_Select(destinations, 0);
		who->move_living(destination, "in a {{prismatic}swirl of colors}", "in a {{prismatic}swirl of colors}");
		object field = Travelers_Find_Interdiction_Field(who);
		if(field) {
			string array realms = field->ganesha_interdiction_field_query_realms();
			realms += ({ "NT" });
			field->ganesha_interdiction_field_set_realms(realms);
		} else {
			field = new(Travelers_Misc("ganesha_interdiction_field"));
			field->ganesha_interdiction_field_set_realms(({ "NT" }));
			field->ganesha_interdiction_field_initialize(who);
		}
		field->set_info(query_requirement_modifier_tag(), True);
		Temple_of_Ganesha_Daemon("control")->temple_of_ganesha_move_temple();
		int array coords;
		string quadrant = 0;
		catch(coords = Temple_of_Ganesha_Daemon("control")->temple_of_ganesha_query_entrance_location()->query_global_coordinates());
		if(coords) {
		    quadrant = "";
		    if(coords[1] < 0)
		        quadrant = "south";
		    else
		        quadrant = "north";
		    if(coords[0] < 0)
		        quadrant += "west";
		    else
		        quadrant += "east";
		}
		object tyrice = Temple_of_Ganesha_NPC("tyrice")->load();
		if(tyrice)
			tyrice->init_command("traveler Let it be known that the temple of Ganesha is moving " + (quadrant ? "to the " + quadrant + " region of the world" : "") + ", so that " + who->query_full_name() + " might complete a pilgrimage.");
		return;
	:));
	set_requirement_overcome_process((:
		object who = $1->ganesha_challenge_query_owner();
		object array interdiction_fields = filter(all_inventory(who), (: $1->is_ganesha_interdiction_field() :));
		foreach(object interdiction_field : interdiction_fields)
			if(interdiction_field->query_info(query_requirement_modifier_tag()))
				interdiction_field->ganesha_interdiction_field_expire();
		return;
	:));
	add_requirement_hook(Do_Move, #'travelers_requirement_pilgrimage_do_move);
}
