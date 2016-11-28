#include <Travelers.h>
#include <conceptual_navigation.h>
#include <daemon.h>
#include <numerical.h>

inherit Travelers_Definition("Invocation");

void configure() {
    ::configure();
    set_invocation_name("vikataya");
    set_invocation_summary("travel to conceptual navigation point");
    set_invocation_key(
        "o greatest Ganesha, whose single tusk overcomes the duplicity of appearance, help me see this world as a dream and a drama, "
        "and let me dance across the texts that give everyone a role, and let me find myself in the"
    );
    set_invocation_help(
        "This invocation allows one to use the power of Ganesha to skip across the symbolic space that is the foundation for all cosmoi, "
        "and traverse the universe conceptually rather than physically.  The frequency with which one can use this invocation depends upon "
        "one's ability in it."
    );
    set_invocation_cost(({
        ({ Energy_Spiritual, 200 })
    }));
    set_invocation_bestowal("conceptual navigation");
    set_invocation_delay(Time_Second * 6);
    set_invocation_argument_required(True);
    set_invocation_argument("%s");
    set_invocation_argument_description("<nth> passage I have marked");
    set_invocation_ability_determinants(([
        Condition_Skill_Composite           : True,
        Skill_Conjuration                   : 0.30,
        Skill_Theurgy                         : 0.30,
        Skill_Invocation                    : 0.40,
        ([
            Condition_Type_Code             : Condition_Type_Attribute,
            Condition_Info                  : Attr_Ego,
            Condition_Value                 : 0.55,
        ])                                  : True,
        ([
            Condition_Type_Code             : Condition_Type_Attribute,
            Condition_Info                  : Attr_Int,
            Condition_Value                 : 0.45,
        ])                                  : True,
    ]));
    set_invocation_ability_maximum(Travelers_Invocation_Ability_Maximum_Very_High);
    set_invocation_begin_can_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        int sonority = query_invocation_sonority(who);
        int since = time() - who->query_info("Ganesha_Conceptual_Navigation_Time");
        int allowed = scale_conversion(sonority, 0, 100, Time_Hour / 2, Time_Minute);
        if(since < allowed) {
            return Error(({
                who, ({ "cannot", who }), "invoke Ganesha for translocation so often;", who, ({ "estimate", who }), "that",
                "at", ({ 'r', who, "current level of ability" }), ",", ({ 'p', who }), ({ "must", who }), "wait at least",
                "another", Daemon_Time->describe_time(allowed - since) 
            }));
        }
        string arg = Process_Query_Info(dxr, "argument");
        string what;
        unless(sscanf(arg, "%s passage i have marked", what) == 1)
            return Error(({
                "which passage?",
            }));
        int which = denumerical(what, Numerical_Flag_Ordinal);
        unless(which)
            return Error(({
                "which passage?",
            }));
        object link = who->query_affiliation_link(project_control());
        int max = link->travelers_query_conceptual_navigation_points_count();
        if(which > max)
            return Error(({
                who, ({ "have", who }), "only", things("conceptual navigation points", max), "available to", ({ 'o', who }),
                ", and yet", who, ({ "have", who }), "requested the", numerical(which, Numerical_Flag_Ordinal),
            }));
        object where = link->travelers_query_conceptual_navigation_point_room(which);
        unless(where)
            return Error(({
                who, ({ "have", who }), "yet to mark a location for that point",
            }));
        if(query_any_realm("NT", who))
            return Error(({
                who, ({ "sense", who }), "some force present locally working to prevent teleportation of any kind",
            }));
        if(query_any_realm("NT", where))
            return Error(({
                who, ({ "sense", who }), "some force at", ({ 'r', who, "desired destination" }), "working to prevent teleportation of any kind",
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
                0, ({ "sense", 0 }), "Ganesha join", ({ 'o', 0 }), "with the symbolic mappings that underpin the cosmos",
            }),
            Message_Senses          : Message_Sense_Cognitive,
        ]));
        string flower_color = Travelers_Flower_Color;
        object env = environment(who);
        env->message(([
            Message_Content         : ({
                "{{" + flower_color + "}colorful flower petals} materialize and swirl through the area"
            }),
            Message_Senses          : Message_Sense_Visual | Message_Sense_Astral,
        ]));
        object array corollaries = ({});
        foreach(object obj : all_inventory(env))
            if(obj->is_character() && obj != who && (who->query_friend(obj) && obj->query_friend(who)) && obj != who->query_mount())
                array_require(&corollaries, obj);
        Conceptual_Navigation_Start(([
            Conceptual_Navigation_Theorem_Postulant         : who,
            Conceptual_Navigation_Theorem_Corollaries       : corollaries,
            Conceptual_Navigation_Theorem_Objective         : where,
            Conceptual_Navigation_Theorem_Exit_Message      : ({
                ", enveloped by the {{" + flower_color + "}petals}",
            }),
            Conceptual_Navigation_Theorem_Enter_Message     : ({
                "in a {{" + flower_color + "}swirl of colorful flower petals}",
            }),
            Conceptual_Navigation_Theorem_Start_Flags       : Conceptual_Navigation_Flag_Exclude_Companions | Conceptual_Navigation_Flag_Slow_Exit,
        ]));
        who->set_info("Ganesha_Conceptual_Navigation_Time", time());
        env->message(([
            Message_Content                                 : ({
                "the {{" + flower_color + "}flower petals} vanish into the surrounding", Description(Description_Type_Ambient_Medium_Colored_Name),
                "with a final breezy twist"
            }),
            Message_Senses                                  : Message_Sense_Visual | Message_Sense_Astral,
        ]));
        return;
    :));
}
