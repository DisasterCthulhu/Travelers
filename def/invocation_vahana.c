#include <Travelers.h>
#include <daemon.h>
#include <Empathic_Bonds.h>
#include <bond.h>
#include <Exoma.h>

inherit Travelers_Definition("Invocation");

void configure() {
    ::configure();
    set_invocation_name("vahana");
    set_invocation_summary("call amphiptere");
    set_invocation_key(
        "o rider of the five vahanas, I besearch you that you grant me one, that I may travel to further lands and know "
        "more distant knowledge, that I may find greater obstacles and deliver more obstacles"
    );
    set_invocation_help(
        "This invocation requests of Ganesha that he offer you a divinely mediated bond with an amphiptere, a winged serpent, "
        "to serve as a holy mount."
        "\n\f"
        "An amphiptere conjured in this manner will remain with one permanently.  If you ever become separated from your serpent, "
        "performing this mantra again will summon it to your side once more."
    );
    set_invocation_cost(({
        ({ Energy_Spiritual, 100 })
    }));
    set_invocation_bestowal("amphiptere");
    set_invocation_delay(Time_Second * 15);
    set_invocation_ability_determinants(([
        Condition_Skill_Composite           : True,
        Skill_Conjuration                   : 0.50,
        Skill_Invocation                    : 0.50,
        ([
            Condition_Type_Code             : Condition_Type_Attribute,
            Condition_Info                  : Attr_Ego,
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
        if(!Empathic_Bonds_Can_Be_Consort(who))
            return Error(({
                who, ({ "cannot", who }), "become consort to a familiar",
            }));
        if(!who->sentience()->query_sentience_emotion())
            return Error(({
                who, ({ "cannot", who }), "use this invocation unless you are capable of feeling empathy",
            }));
        descriptor bond = Empathic_Bonds_Find_Affiliation_Bond(who, project_control());
        if(!bond)
            return True;
        object familiar = Bond_Query(bond, Bond_Familiar);
        if(!familiar)
            return Error(({
                who, ({ "do", who }), "not have an extant amphitere to summon"
            }));
        if(query_any_realm("NT", familiar) || query_any_realm("NT", who))
            return Error(({
                "some force is preventing", who, "from sending or calling to", familiar
            }));
        if(who->query_affiliation_link("Empathic Bonds")->empathic_bonds_query_familiar_dead(familiar))
            return Error(({
                familiar, "died and is being spiritually reconstituted; when the process has completed,",
                ({ 'p', familiar }), "will automatically return to", ({ 'o', 0 }),
            }));
        return True;
    :));
    set_invocation_execute_do_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        descriptor bond = Empathic_Bonds_Find_Affiliation_Bond(who, project_control());
        if(!bond) {
            who->message(([
                Message_Content         : ({
                    "a sonorous ring fills the", Description(Description_Type_Ambient_Medium_Colored_Name), ", as though a chime",
                    "has been sounded"
                }),
                Message_Senses          : Message_Sense_Auditory,
            ]));
            who->display(([
                Message_Content         : ({
                    0, ({ "sense", 0 }), "Ganesha working across cosmic distances, and", ({ "feel", 0 }), "the beginnings of a",
                    "strange emotional bond"
                }),
                Message_Senses          : Message_Sense_Cognitive,
            ]));
            Empathic_Bonds_Add_Bond_Means(who, ([
                Bond_Means              : "the divinity of Ganesha",
                Bond_Type               : "Ganesha vahana",
                Bond_Flags              : Bond_Flag_Disallow_Volitional_Bonding | Bond_Flag_Disallow_Volitional_Unbonding,
                Bond_Affiliation        : project_control(),
            ]));
            object familiar = new(Travelers_Monster("amphiptere"));
            familiar->move(environment(who));
            familiar->message(([
                Message_Content    : ({
                    0, ({ "appear", 0 }), "in the", Description(Description_Type_Ambient_Medium_Colored_Name), "in", 'a',
                    self_color("prismatic", "burst of", "sparks"),
                }),
                Message_Senses     : Message_Sense_Visual | Message_Sense_Astral,
            ]));
            familiar->message(([
                Message_Content    : ({
                    0, ({ familiar->query_move_mode(), 0 }), "to", ({ 's', who, "side" }),
                }),
                Message_Senses     : Message_Sense_Visual,
            ]));
            who->add_spell_points(-semirandom(1000));
            who->display(([
                Message_Content    : ({
                    0, ({ "feel", 0 }), "spiritually exhausted but deeply contented, as though",
                    ({ 'p', 0 }), ({ "have", 0 }), "been made more complete",
                }),
                Message_Senses     : Message_Sense_Spiritual,
            ]));
            who->query_affiliation_link("Empathic Bonds")->empathic_bonds_add_familiar(familiar, "the divinity of Ganesha");
            who->add_skill_exp(Skill_Empathy, Learn_Very_Rare);
            object type = Empathic_Bonds_Familiar_Type(Empathic_Bonds_Query_Familiar_Type_For_Object(familiar));
            string log_item = ctime() + ": ";
            log_item += printable(who) + " bonded with " + printable(familiar) + " via the divinity of Ganesha, familiar type " + Empathic_Bonds_Query_Familiar_Type_For_Object(familiar);
            log_file("dev/twilight.empathic_bonds", log_item);
        } else {
            object familiar = Bond_Query(bond, Bond_Familiar);
            if(environment(familiar) == environment(who)) {
                object cosm = Exoma_Daemon("control")->retrieve_personal_microcosm(who);
                familiar->message(([
                    Message_Content    : ({
                        0, ({ "disappear", 0 }), "suddenly in", 'a',
                        self_color("prismatic", "burst of", "sparks"),
                    }),
                    Message_Senses     : Message_Sense_Visual,
                ]));
                familiar->move(cosm);
                familiar->message(([
                    Message_Content    : ({
                        0, ({ "appear", 0 }), "suddenly in", 'a',
                        self_color("prismatic", "burst of", "sparks"),
                    }),
                    Message_Senses     : Message_Sense_Visual,
                ]));
                return True;
            } else {
                familiar->message(([
                    Message_Content    : ({
                        0, ({ "disappear", 0 }), "suddenly in", 'a',
                        self_color("prismatic", "burst of", "sparks"),
                    }),
                    Message_Senses     : Message_Sense_Visual,
                ]));
                familiar->move(environment(who));
                familiar->message(([
                    Message_Content    : ({
                        0, ({ "appear", 0 }), "suddenly at", ({ 's', who, "side" }), "in", 'a',
                        self_color("prismatic", "burst of", "sparks"),
                    }),
                    Message_Senses     : Message_Sense_Visual,
                ]));
            }
        }
        return;
    :));
}
