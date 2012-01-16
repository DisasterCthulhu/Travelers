#include <Travelers.h>
#include <daemon.h>
#include <conceptual_navigation.h>

inherit Travelers_Definition("Invocation");

void configure() {
	::configure();
	set_invocation_name("ekadantaya");
	set_invocation_summary("list divine waypoints");
	set_invocation_key(
		"o Ganesha, lord of all ancestors, he who sees all places and knows all troubles, "
		"reveal the paths for my cosmic journey that you have lit with your bright face"
	);
	set_invocation_help(
		"This invocation requests of Ganesha that he reminds you of the locations that he has marked for you as divine waypoints."
	);
	set_invocation_cost(({
		({ Energy_Spiritual, 50 })
	}));
	set_invocation_bestowal("divine waypoints");
	set_invocation_delay(Time_Second * 2);
	set_invocation_ability_determinants(([
		Condition_Skill_Composite           : True,
		Skill_Invocation                    : 1.00,
		([
			Condition_Type_Code             : Condition_Type_Attribute,
			Condition_Info                  : Attr_Cha,
			Condition_Value                 : 1.00,
		])                                  : True,
	]));
	set_invocation_execute_do_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		who->display(([
			Message_Content         : ({
				0, ({ "sense", 0 }), "knowledge and memories of", ({ 'r', 0, "travels" }), ", past and future, flow from Ganesha into",
				({ 'r', 0, "mind" }),
			}),
			Message_Senses          : Message_Sense_Cognitive,
		]));
		object array points = who->query_affiliation_link(project_control())->travelers_query_divine_waypoint_rooms();
		int count = 1;
		string array ordinals = ({});
		string array room_names = ({});
		string array area_names = ({});
		foreach(object point : points) {
			ordinals += ({ "{{blue}" + numerical(count, Numerical_Flag_Ordinal) + "}" });
			if(!point) {
				room_names += ({ "{{black}not yet specified}" });
				area_names += ({ "{{black}N/A}" });
			} else {
				room_names += ({ "{{green}" + point->query_short() + "}" });
				string area_name = point->project_control()->query_area();
				if(area_name)
					area_names += ({ "{{orange}" + area_name + "}" });
				else
					area_names += ({ "{{black}N/A}" });
			}
			count++;
		}
		who->display(Table_Render(([
			Table_Columns           : ({
				([
					Column_Items    : ordinals,
					Column_Head     : "{{violet}point}",
				]),
				([
					Column_Items    : room_names,
					Column_Head     : "{{violet}specific place}",
				]),
				([
					Column_Items    : area_names,
					Column_Head     : "{{violet}greater area}",
				]),
			}),
			Table_Title          : "{{yellow}Traveler Divine Waypoints}",
			Table_Flags          : Table_Flag_No_Expansion,
		]), who));
		return;
	:));
}
