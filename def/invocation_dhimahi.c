#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Invocation");

void configure() {
    ::configure();
    set_invocation_name("dhimahi");
    set_invocation_summary("divest advanced challenge");
    set_invocation_bestowal("advanced challenges");
    set_invocation_key(
        "o lord Ganesha, whose massive body is supreme and perfect, to whom we meditate and pray and bow and revere, "
        "let it be known to you that it is known to"
    );
    set_invocation_help(
        "Those who are serious about enlightenment can receive an advanced challenge, which has not only an obstacle and a requirement, "
        "but also a taboo and a forfeit.  The taboo is something that must not be done.  If the taboo is violated, then the challenge "
        "ends in failure, and a punishment, the forfeit, is brought down upon the challengee.  These challenges are considerably more "
        "difficult but also considerably more rewarding than ordinary challenges."
        "\n\f"
        "For this invocation to work, "
        "the target must explicitly trust you; Ganesha will not inflict these challenges upon those that do not desire them.  If the target "
        "completes this challenge, it will be partly by your action that this result came about, and your karma phala will perforce increase."
    );
    set_invocation_cost(({
        ({ Energy_Spiritual, 500 }),
    }));
    set_invocation_argument_required(True);
    set_invocation_argument("%s");
    set_invocation_argument_description("<target> that the most severe obstacle brings the most lucid enlightenment");
    set_invocation_delay(Time_Second * 18);
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
        unless(sscanf(arg, "%s that the most severe obstacle brings the most lucid enlightenment", what) == 1)
            return Error(({
                "who desires an advanced challenge?"
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
        unless(Ganesha_Query_Karma_Phala(target) >= 250)
            return Error(({
                target, ({ "are", target }), "not yet ready for advanced challenges, as", ({ 'p', target }), ({ "have", target }), "not yet",
                "acquired even a basic degree of karmic enlightenment"
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
                    'a', self_color("dazzling golden", "flurry of", "flower petals"), "swirl around", 0, "momentarily"
                }),
                Message_Senses          : Message_Sense_Astral,
            ]));
            who->display(([
                Message_Content         : ({
                    "Ganesha's power flows through", 0, "and into", target, ", providing", ({ 'o', target }), "with an obstacle, a purpose,",
                    "a taboo, and a forfeit"
                }),
                Message_Senses          : Message_Sense_Cognitive,
            ]));
            new(Travelers_Misc("challenge"))->ganesha_challenge_initialize(who, target, True, True, True, True);
        }
        return;
    :));
}
