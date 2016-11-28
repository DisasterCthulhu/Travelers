#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Invocation");

void configure() {
    ::configure();
    set_invocation_name("lambodaraya");
    set_invocation_summary("request katakacha");
    set_invocation_key(
        "o leader of the pramada ganas, one who is the color of vermillion, with an elephant's visage and a hanging belly, "
        "I offer obeisance to you first, and ask that you grant me a tool with which I may seek more knowledge"
    );
    set_invocation_help(
        "This invocation requests of Ganesha that he offer you a divinely mediated bond with a katakacha, a sessile, "
        "metamorphic creature made of beautiful crystal.  Katakacha are an ancient people who were nearly driven to extinction "
        "by the kazar until a climactic intervention by Ganesha preserved them.  They aid the Travelers out of gratitude for "
        "this act."
        "\n\f"
        "A katakacha conjured in this manner will remain with one permanently.  If it is ever lost or destroyed for some reason, "
        "performing this mantra again will conjure another."
    );
    set_invocation_cost(({
        ({ Energy_Spiritual, 500 })
    }));
    set_invocation_bestowal("katakacha");
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
        object katakacha = present("KATAKACHA", who, who);        
        if(katakacha)
            return Error(({
                who, "already", ({ "have", who }), "a katakacha"
            }));
        return True;
    :));
    set_invocation_execute_do_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
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
        object katakacha = new(Travelers_Weapon("katakacha"));
        if(katakacha->move(who) != Move_Succeed) {
            who->display(([
                Message_Content     : ({
                    0, ({ "sense", 0 }), "Ganesha's power run its course prematurely; something has gone wrong"
                }),
                Message_Senses      : Message_Sense_Cognitive,
            ]));
            katakacha->remove();
        } else {
            who->message(([
                Message_Content         : ({
                    ({ 'a', katakacha }), ({ "materialize", katakacha }), "in", ({ 's', 0, "inventory" }), "out of a strange",
                    "disturbance in the nearby", Description(Description_Type_Ambient_Medium_Colored_Name)
                }),
                Message_Senses          : Message_Sense_Visual | Message_Sense_Tactile_For_Participants,
            ]));
        }
        return;
    :));
}
