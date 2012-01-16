#include <Travelers.h>
#include <daemon.h>
#include <properties.h>
#include <description.h>
#include <Wild_Talents.h>

inherit Travelers_Definition("Invocation");

object array invocation_sharanam_find_kazar_blades(object who) {
    return filter(all_inventory(who), (:
        if($1->query_property(Prop_Manifestation))
            return False;
        if($1->is_kazarithax())
            return True;
        if($1->id("KAZARZETH"))
            return True;
        return False;
    :));
}

void configure() {
	::configure();
	set_invocation_name("sharanam");
	set_invocation_summary("destroy kazar blades");
	set_invocation_bestowal("kazar blade destruction");
	set_invocation_key("o omnipresent Ganesha, pure and peaceful doer of all good, shield the world from fear and anger so that we can see thee everywhere and at all times");
	set_invocation_help(
	    "This mantra entreats Ganesha to destroy any kazar blades present on the invoker and to empower the invoker as a reward.  "
	    "The reasons for the effects of this mantra are obscured in history, but it is generally thought to be related to the "
	    "sacred nature of the katakacha."
	);
	set_invocation_cost(({
		({ Energy_Spiritual, 100 }),
	}));
	set_invocation_delay(Time_Second * 25);
	set_invocation_ability_minimum(Travelers_Invocation_Ability_Minimum_Moderate);
	set_invocation_ability_maximum(Travelers_Invocation_Ability_Maximum_Very_High);
	set_invocation_ability_determinants(([
		Condition_Skill_Composite           : True,
		Skill_Invocation                    : 0.50,
		Skill_Abjuration                    : 0.50,
		([
			Condition_Type_Code             : Condition_Type_Attribute,
			Condition_Info                  : Attr_Cha,
			Condition_Value                 : 1.00,
		])                                  : True,
	]));
	set_invocation_begin_can_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		object array blades = invocation_sharanam_find_kazar_blades(who);
		if(!blades || !sizeof(blades))
		    return Error(({
				"there are no kazar blades on", ({ 's', who, "person" }), 
			}));
		return True;
	:));
	set_invocation_continue_can_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		object array blades = invocation_sharanam_find_kazar_blades(who);
		if(!blades || !sizeof(blades))
		    return Error(({
				"there are no longer any kazar blades on", ({ 's', who, "person" }), 
			}));
		return True;
	:));
	set_invocation_execute_do_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		object array blades = invocation_sharanam_find_kazar_blades(who);
        string flower_color = random_element(({
            "paisley",
            "iridescent",
            "pink",
            "light blue",
            "prismatic",
            "milky",
        }));
        flower_color += "-";
        flower_color += random_element(({
            "mottled",
            "flecked",
            "spotted",
            "banded",
            "striped",
            "speckled",
            "streaked",
            "tinted",
        }));
        flower_color += " ";
        flower_color += random_element(Travelers_Rainbow);
		who->message(([
		    Message_Content         : ({
		        ({ 's', 0, blades }), "suddenly", ({ "explode", blades }), "into a",
		        "{{" + flower_color + "}swirling cloud of colorful flower petals},",
		        "which dissipate in the surrounding", Description(Description_Type_Ambient_Medium_Colored_Name),
		    }),
			Message_Senses          : Message_Sense_Astral | Message_Sense_Visual,
		]));
		int sonority = query_invocation_sonority(who);
		int duration = scale_conversion(sonority, 0, 100, Time_Hour, Time_Day) * sizeof(blades);
		Ganesha_Add_Karma_Phala(who, sizeof(blades));
		blades->remove();
		who->add_attribute_modifier(([
		    Modifier_Index          : Basic_Attributes,
		    Modifier_Amount         : 150,
		    Modifier_Bound          : 350,
		    Modifier_Flags          : Modifier_Flag_Mergeable,
		    Modifier_Duration       : duration,
		    Modifier_Add_Display    : ([
		        Message_Content     : ({
		            0, ({ "feel", 0 }), ({ 'l', 0 }), "come into momentary contact with the divine",
		            "as a rush of power from Ganesha surges through the core of", ({ 'r', 0, "being" }),
		        }),
		        Message_Senses      : Message_Sense_Spiritual,
		        Message_Color       : "status: enhancement",
		    ]),
		    Modifier_Remove_Display : ([
		        Message_Content     : ({
		            0, ({ "feel", 0 }), "the power of Ganesha infusing", ({ 'r', 0, "essence" }), "ebb and fade",
		        }),
		        Message_Senses      : Message_Sense_Spiritual,
		        Message_Color       : "status: loss",
		    ]),
		    Modifier_Merge_Display  : ([
		        Message_Content     : ({
		            0, ({ "feel", 0 }), ({ 'l', 0 }), "come into momentary contact with the divine",
		            "as another rush of power from Ganesha flows into the core of", ({ 'r', 0, "being" }),
		            ", reinforcing and prolonging his boon"
		        }),
		        Message_Senses      : Message_Sense_Spiritual,
		        Message_Color       : "status: enhancement",
		    ]),
		]));
		who->add_speed_modifier(([
		    Modifier_Amount         : 25,
		    Modifier_Duration       : duration,
		    Modifier_Flags          : Modifier_Flag_Mergeable,
		]));
		who->add_trait_modifier(([
		    Modifier_Index          : Trait_Order_Favour,
		    Modifier_Amount         : 5,
		    Modifier_Duration       : duration,
		    Modifier_Flags          : Modifier_Flag_Mergeable,
		]));
		Wild_Talents_Control->awakening_event(who, "mental enhancement", 500);
		Wild_Talents_Control->awakening_event(who, "psychic contact", 500);
		return;
	:));
}
