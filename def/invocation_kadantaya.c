#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Invocation");

void configure() {
    ::configure();
    set_invocation_name("kadantaya");
    set_invocation_summary("request challenge");
    set_invocation_key(
        "o lord Ganesha, the elephant-faced who is like the sun to the lotus-faced Parvati, placer of obstacles, grant "
        "me a challenge so that I might find wisdom"
    );
    set_invocation_help(
        "Ganesha not only removes but places obstacles.  The primary mode of adoration of a Traveler is to request one "
        "deliberately in obsecration.  By reciting this mantra, one entreats Ganesha to both show one a path, and to bar it.  "
        "Finding one's way to the end of the path in spite of the obstacle will remove the obstacle, bring a direct divine reward from "
        "Ganesha, and increasing one's karma phala, enlightenment, and freedom."
    );
    set_invocation_cost(({
        ({ Energy_Spiritual, 100 }),
    }));
    set_invocation_delay(Time_Second * 10);
    set_invocation_ability_determinants(([
        Condition_Skill_Composite           : True,
        Skill_Invocation                    : 1.00,
        ([
            Condition_Type_Code             : Condition_Type_Attribute,
            Condition_Info                  : Attr_Ego,
            Condition_Value                 : 1.00,
        ])                                  : True,
    ]));
    set_invocation_begin_can_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        object challenge = Travelers_Find_Challenge(who);
        if(challenge)
            return Error(({
                who, "already", ({ "have", who }), "a challenge from Ganesha"
            }));
        int allowed = who->query_info("Ganesha_Challenge_Allowed_Time");
        if(allowed && time() < allowed)
            return Error(({
                who, ({ "have", who }), "yielded to a challenge from Ganesha too recently to expect that he would be willing to give",
                ({ 'o', who }), "one again"
            }));
        if(member(Travelers_Challenge_Banned_Atmai, who->query_atman_name()) != Null)
            return Error(({
                who, ({ "have", who }), "been banned from receiving challenges, thereby sparing", ({ 'o', who }),
                "from the constant disappointment they apparently",
                "bring", ({ 'o', who }),
            }));
        return True;
    :));
    set_invocation_continue_can_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        object challenge = Travelers_Find_Challenge(who);
        if(challenge)
            return Error(({
                who, ({ "have", who }), "just acquired a challenge from Ganesha"
            }));
        return True;
    :));
    set_invocation_execute_do_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        object target = Process_Query(dxr, Process_Target);
        who->message(([
            Message_Content         : ({
                'a', self_color("prismatic", "flurry of", "flower petals"), "swirl around", 0, "momentarily"
            }),
            Message_Senses          : Message_Sense_Astral,
        ]));
        who->display(([
            Message_Content         : ({
                "Ganesha's power fills", 0, ", providing", ({ 'o', 0 }), "with an obstacle and a purpose"
            }),
            Message_Senses          : Message_Sense_Cognitive,
        ]));
        new(Travelers_Misc("challenge"))->ganesha_challenge_initialize(who, 0, True, True);
        return;
    :));
}
