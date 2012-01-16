#include <Travelers.h>
#include <daemon.h>
#include <conceptual_navigation.h>

inherit Travelers_Definition("Invocation");

void configure() {
	::configure();
	set_invocation_name("sloka bhalachandraya");
	set_invocation_summary("mark divine waypoint");
	set_invocation_key(
		"o Ganesha, wearer of white garments, who has a complexion like a full moon, "
		"let your four all-powerful hands touch this place; count it as the"
	);
	set_invocation_help(
	    "This invocation allows one to set a divine waypoint, a powerful marker for one's travels.  These waypoints "
	    "require the direct intervention of Ganesha, bypassing the potentially laborious task of conceptual navigation, "
	    "and overriding almost any enchantment preventing translocation."
	);
	set_invocation_cost(({
		({ Energy_Spiritual, 1000 })
	}));
	set_invocation_bestowal("divine waypoints");
	set_invocation_delay(Time_Second * 45);
	set_invocation_argument_required(True);
	set_invocation_argument("%s");
	set_invocation_argument_description("<nth> divine beacon to light my way");
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
		unless(sscanf(arg, "%s divine beacon to light my way", what) == 1)
			return Error(({
				"which landmark?",
			}));
		int which = denumerical(what, True);
		unless(which)
			return Error(({
				"which landmark?",
			}));
		object link = who->query_affiliation_link(project_control());
		int max = link->travelers_query_divine_waypoints_count();
		if(which > max)
			return Error(({
				who, ({ "have", who }), "only", things("divine waypoints", max), "available to", ({ 'o', who }),
				", and yet", who, ({ "have", who }), "requested to designate the", numerical(which, Numerical_Flag_Ordinal),
			}));
		object where = environment(who);
		unless(where)
			return Error(({
				who, ({ "have", who }), "no environment",
			}));
		if(query_any_realm("Temple_of_Ganesha", where))
		    return Error(({
		        who, ({ "cannot", who }), "use this power to make the Temple of Ganesha a waypoint",
		    }));
        if(where->is_character())
            return Error(({
                who, ({ "cannot", who }), "set an animate being as a waypoint"
            }));
		Process_Set(dxr, Process_Target, where);
		return True;
	:));
	set_invocation_execute_do_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		object where = Process_Query(dxr, Process_Target);
		string what;
		sscanf(Process_Query_Info(dxr, "argument"), "%s divine beacon to light my way", what);
		int which = denumerical(what, True);
		who->display(([
			Message_Content         : ({
				0, ({ "feel", 0 }), ({ 'r', 0, "mind", }), "go momentarily blank as a tiny fragment of Ganesha enters it, overwhelming",
				({ 'o', 0 }), "completely",
			}),
			Message_Senses          : Message_Sense_Cognitive | Message_Sense_Spiritual,
		]));
		who->display(([
			Message_Content         : ({
			    0, ({ "sense", 0 }), "Ganesha acting through", ({ 'o', 0 }), ", investing a tiny portion of his essence in this place, that",
			    ({ 'p', 0 }), "may use it in future travels",
			}),
			Message_Senses          : Message_Sense_Cognitive | Message_Sense_Spiritual,
		]));
		who->query_affiliation_link(project_control())->travelers_set_divine_waypoint(which, where);
		where->message(([
			Message_Content         : ({
				"the surrounding", Description(Description_Type_Ambient_Medium_Colored_Name), "pulses with {{prismatic}prismatic radiance}",
			}),
			Message_Senses          : Message_Sense_Astral,
		]));
		who->add_trait_modifier(([
		    Modifier_Index          : Trait_Spiritual_Disjunction,
		    Modifier_Amount         : 1,
		    Modifier_Duration       : fuzz(Time_Hour / 4),
		    Modifier_Add_Display    : ([
		        Message_Senses      : Message_Sense_Spiritual | Message_Sense_Cognitive,
		        Message_Color       : "status: loss",
		        Message_Content     : ({
		            ({ 's', 0, "sense of self" }), "returns as Ganesha departs from", ({ 'o', 0 }), ", but", ({ 'p', 0 }),
		            ({ "remain", 0 }), "spiritually disoriented by the experience",
		        }),
		    ]),
		    Modifier_Remove_Display : ([
		        Message_Senses      : Message_Sense_Spiritual | Message_Sense_Cognitive,
		        Message_Color       : "status: enhancement",
		        Message_Content     : ({
		            0, ({ "feel", 0 }), ({ 'r', 0, "mind", }), "recovering from the shock of Ganesha's presence",
		        }),
		    ]),
		]));
		return;
	:));
}
