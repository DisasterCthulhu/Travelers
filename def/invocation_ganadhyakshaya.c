#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Invocation");

void configure() {
    ::configure();
    set_invocation_name("ganadhyakshaya");
    set_invocation_summary("divest challenge");
    set_invocation_key(
        "o lord Ganesha, he who is the elephant and the serpent and the lion and the mouse and the peacocke and the horse, placer of obstacles,"
    );
    set_invocation_help(
        "Ganesha can place an obstacle for one who requests it, even if he is not a Traveler himself.  For this invocation to work, "
        "the target must explicitly trust you; Ganesha will not inflict these challenges upon those that do not desire them.  If the target "
        "completes this challenge, it will be partly by your action that this result came about, and your karma phala will perforce increase."
    );
    set_invocation_cost(({
        ({ Energy_Spiritual, 100 }),
    }));
    set_invocation_argument_required(True);
    set_invocation_argument("%s");
    set_invocation_argument_description("<target> requests enlightenment and challenge");
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
        string arg = Process_Query_Info(dxr, "argument");
        string what;
        unless(sscanf(arg, "%s requests enlightenment and challenge", what) == 1)
            return Error(({
                "who desires a challenge?"
            }));
        object target = who->locate_name(what);
        unless(target)
            return Error(({
                who, ({ "do", who }), "not see any '" + what + "' here"
            }));
        unless(target->is_character() && environment(target) == environment(who))
            return Error(({
                who, ({ "cannot", who }), "use this invocation in that manner"
            }));
                unless(target->is_incarnos() && !target->query_trait(Trait_Spiritual_Disjunction))
            return Error(({
                target, ({ "cannot", target }), "receive challenges",
            }));
        if(target == who)
            return Error(({
                "there is another invocation for requesting a challenge for", ({ 'l', who })
            }));
        object challenge = Travelers_Find_Challenge(target);
        if(challenge)
            return Error(({
                target, "already", ({ "have", target }), "a challenge from Ganesha"
            }));
        int allowed = target->query_info("Ganesha_Challenge_Allowed_Time");
        if(allowed && time() < allowed)
            return Error(({
                target, ({ "have", target }), "yielded to a challenge from Ganesha too recently to expect that he would be willing to give",
                ({ 'o', target }), "one again"
            }));
        if(member(Travelers_Challenge_Banned_Atmai, target->query_atman_name()) != Null)
            return Error(({
                target, ({ "have", target }), "been banned from receiving challenges, thereby sparing", ({ 'o', target }),
                "from the constant disappointment they apparently",
                "bring", ({ 'o', target }),
            }));
        Process_Set(dxr, Process_Target, target);
        return True;
    :));
    set_invocation_continue_can_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        object target = Process_Query(dxr, Process_Target);
        if(!target)
            return Error(({
                target, ({ "have", target }), "left", ({ 's', who, "vicinity" }),
            }));
        object challenge = Travelers_Find_Challenge(target);
        if(challenge)
            return Error(({
                target, ({ "have", target }), "just acquired a challenge from Ganesha"
            }));
        if(environment(target) != environment(who))
            return Error(({
                target, ({ "have", target }), "left", ({ 's', who, "vicinity" }),
            }));
        return True;
    :));
    set_invocation_execute_do_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        object target = Process_Query(dxr, Process_Target);
        if(!target->query_trusts(who)) {
            who->display(([
                Message_Content         : ({
                    "Ganesha's power flows through", 0, "and into", target, ", but it stops abruptly, sensing some",
                    "trepidation or lack of trust within", ({ 'o', target }),
                }),
                Message_Senses          : Message_Sense_Cognitive,
            ]));
        } else {
            target->message(([
                Message_Content         : ({
                    'a', self_color("prismatic", "flurry of", "flower petals"), "swirl around", 0, "momentarily"
                }),
                Message_Senses          : Message_Sense_Astral,
            ]));
            who->display(([
                Message_Content         : ({
                    "Ganesha's power flows through", 0, "and into", target, ", providing", ({ 'o', target }), "with an obstacle and a purpose"
                }),
                Message_Senses          : Message_Sense_Cognitive,
            ]));
            new(Travelers_Misc("challenge"))->ganesha_challenge_initialize(who, target, True, True);
        }
        return;
    :));
}
