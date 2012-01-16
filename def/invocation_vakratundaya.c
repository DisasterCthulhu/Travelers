#include <Travelers.h>
#include <daemon.h>
#include <conceptual_navigation.h>

inherit Travelers_Definition("Invocation");

void configure() {
	::configure();
	set_invocation_name("vakratundaya");
	set_invocation_summary("travel to divine waypoint");
	set_invocation_key(
		"o greatest Ganesha, straightener of the crooked path, do not delay in straightening my path so that no "
		"obstacle may bar it, and let me find myself at the"
	);
	set_invocation_help(
	    "This invocation allows one to instantly traverse to a divine waypoint."
	);
	set_invocation_cost(({
		({ Energy_Spiritual, 500 })
	}));
	set_invocation_bestowal("divine waypoints");
	set_invocation_delay(Time_Second * 2);
	set_invocation_argument_required(True);
	set_invocation_argument("%s");
	set_invocation_argument_description("<nth> beacon you have lit for me");
	set_invocation_ability_determinants(([
		Condition_Skill_Composite           : True,
		Skill_Conjuration                   : 0.10,
		Skill_Theurgy                         : 0.10,
		Skill_Abjuration                    : 0.10,
		Skill_Invocation                    : 0.70,
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
	set_invocation_ability_maximum(Travelers_Invocation_Ability_Maximum_Very_High);
	set_invocation_begin_can_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		string arg = Process_Query_Info(dxr, "argument");
		string what;
		unless(sscanf(arg, "%s beacon you have lit for me", what) == 1)
			return Error(({
				"which beacon?",
			}));
		int which = denumerical(what, True);
		unless(which)
			return Error(({
				"which beacon?",
			}));
		object link = who->query_affiliation_link(project_control());
		int max = link->travelers_query_divine_waypoints_count();
		if(which > max)
			return Error(({
				who, ({ "have", who }), "only", things("divine waypoints", max), "available to", ({ 'o', who }),
				", and yet", who, ({ "have", who }), "requested the", numerical(which, Numerical_Flag_Ordinal),
			}));
		object where = link->travelers_query_divine_waypoint_room(which);
		unless(where)
			return Error(({
				who, ({ "have", who }), "yet to mark a location for that point",
			}));
		object here = environment(who);
		status field = any(deep_inventory(here), (: $1->is_ganesha_interdiction_field() && sizeof($1->ganesha_interdiction_field_query_realms()) && member($1->ganesha_interdiction_query_realms(), "NT") != Null :));
		if(field)
		    return Error(({
		        "Ganesha will not intervene to remove his own interdiction."
		    }));
		if(query_any_realm("Temple_of_Ganesha", where) || query_any_realm("Temple_of_Ganesha", here))
		    return Error(({
		        "Ganesha will not remove his transplanar warding from his own temple."
		    }));
		Process_Set(dxr, Process_Target, where);
		return True;
	:));
	set_invocation_execute_do_call((:
		descriptor dxr = $1;
		Process_Interrupt_Off(dxr, Process_Interrupt_Movement | Process_Interrupt_Ambulation);
		object who = Process_Query(dxr, Process_Actor);
		object where = Process_Query(dxr, Process_Target);
		who->display(([
			Message_Content         : ({
				0, ({ "sense", 0 }), "Ganesha intently clearing obstacles between", ({ 'o', 0 }), "and", ({ 'r', 0, "destination" }),
			}),
			Message_Senses          : Message_Sense_Cognitive,
		]));
		string flower_color = "prismatic";
		object env = environment(who);
		object array convoy = ({ who });
		foreach(object obj : all_inventory(env))
			if(obj->is_character() && obj != who && (who->query_friend(obj) && obj->query_friend(who)) && obj != who->query_mount())
				array_specify(&convoy, obj);
		env->message(([
			Message_Content         : ({
			    'a', self_color("prismatic", "intense", "radiance"), "washes over the area, settling on", ({ 'j', convoy }),
			    "as", self_color("dazzling golden", "motes of", "light"),
			}),
			Message_Senses          : Message_Sense_Visual | Message_Sense_Astral,
		]));
		descriptor realm_mod_1;
		descriptor realm_mod_2;
		if(query_any_realm("NT", env))
		    realm_mod_1 = env->add_realm_modifier(([
		        Modifier_Index      : "NT",
		        Modifier_Amount     : -1000,
		    ]));
		if(query_any_realm("NT", where))
		    realm_mod_2 = where->add_realm_modifier(([
		        Modifier_Index      : "NT",
		        Modifier_Amount     : -1000,
		    ]));
		env->message(([
			Message_Content         : ({
			    ({ 'j', convoy }), ({ "vanish", convoy }),
			}),
			Message_Exclude         : convoy,
			Message_Senses          : Message_Sense_Visual | Message_Sense_Astral,
		]));
		convoy->display(([
		    Message_Content         : ({
		        0, ({ "have", 0 }), "an apophetic sense of coming into close contact with a divine presence, followed immediately by",
		        "a dizzying sensation of movement",
		    }),
		    Message_Senses          : Message_Sense_Kinesthetic | Message_Sense_Spiritual,
		]));
		convoy->move(where);
		where->message(([
			Message_Content         : ({
			    ({ 'j', convoy }), ({ "appear", convoy }), "in", 'a', self_color("dazzling golden", "burst of", "motes of light")
			}),
			Message_Exclude         : convoy,
			Message_Senses          : Message_Sense_Visual | Message_Sense_Astral,
		]));
		if(realm_mod_1)
		    env->remove_realm_modifier(&realm_mod_1);
		if(realm_mod_2)
		    where->remove_realm_modifier(&realm_mod_2);
        who->describe_room(where, who->query_depictions());
		return;
	:));
}
