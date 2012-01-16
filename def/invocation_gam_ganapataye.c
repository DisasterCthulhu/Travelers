#include <Travelers.h>
#include <daemon.h>
#include <conceptual_navigation.h>

inherit Travelers_Definition("Invocation");

void configure() {
	::configure();
	set_invocation_name("gam ganapataye");
	set_invocation_summary("mark conceptual navigation point");
	set_invocation_key(
		"o Ganesha, lord of all journeys and travel, who wanders and gathers knowledge into his great belly, "
		"let me mark the significance of this place for my own journeys; count it as the"
	);
	set_invocation_help(
		"This invocation lets one mark a passage in the cosmos symbolically, as one that one desires to return to "
		"or use as a landmark on one's future journeys."
	);
	set_invocation_cost(({
		({ Energy_Spiritual, 100 })
	}));
	set_invocation_bestowal("conceptual navigation");
	set_invocation_delay(Time_Second * 6);
	set_invocation_argument_required(True);
	set_invocation_argument("%s");
	set_invocation_argument_description("<nth> landmark to guide me on my road");
	set_invocation_ability_determinants(([
		Condition_Skill_Composite           : True,
		Skill_Theurgy                         : 0.50,
		Skill_Invocation                    : 0.50,
		([
			Condition_Type_Code             : Condition_Type_Attribute,
			Condition_Info                  : Attr_Cha,
			Condition_Value                 : 0.75,
		])                                  : True,
		([
			Condition_Type_Code             : Condition_Type_Attribute,
			Condition_Info                  : Attr_Int,
			Condition_Value                 : 0.25,
		])                                  : True,
	]));
	set_invocation_begin_can_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		int sonority = query_invocation_sonority(who);
		string arg = Process_Query_Info(dxr, "argument");
		string what;
		unless(sscanf(arg, "%s landmark to guide me on my road", what) == 1)
			return Error(({
				"which landmark?",
			}));
		int which = denumerical(what, True);
		unless(which)
			return Error(({
				"which landmark?",
			}));
		object link = who->query_affiliation_link(project_control());
		int max = link->travelers_query_conceptual_navigation_points_count();
		if(which > max)
			return Error(({
				who, ({ "have", who }), "only", things("conceptual navigation points", max), "available to", ({ 'o', who }),
				", and yet", who, ({ "have", who }), "requested to designate the", numerical(which, Numerical_Flag_Ordinal),
			}));
		object where = environment(who);
		unless(where)
			return Error(({
				who, ({ "have", who }), "no environment",
			}));
		if(where->is_character())
		    return Error(({
		        who, ({ "cannot", who }), "set an animate being as a landmark"
		    }));
		Process_Set(dxr, Process_Target, where);
		return True;
	:));
	set_invocation_execute_do_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		object where = Process_Query(dxr, Process_Target);
		string what;
		sscanf(Process_Query_Info(dxr, "argument"), "%s landmark to guide me on my road", what);
		int which = denumerical(what, True);
		who->display(([
			Message_Content         : ({
				0, ({ "sense", 0 }), "Ganesha help", ({ 'p', 0 }), ({ "mark", 0 }), "this place for use in future travels",
			}),
			Message_Senses          : Message_Sense_Cognitive,
		]));
		who->query_affiliation_link(project_control())->travelers_set_conceptual_navigation_point(which, where);
		where->message(([
			Message_Content         : ({
				"the surrounding", Description(Description_Type_Ambient_Medium_Colored_Name), "races with",
				self_color("shimmering " + random_element(Travelers_Rainbow), 0, "runes"),
			}),
			Message_Senses          : Message_Sense_Astral,
		]));
		return;
	:));
}
