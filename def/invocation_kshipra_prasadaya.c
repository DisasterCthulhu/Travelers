#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Invocation");

void configure() {
    ::configure();
    set_invocation_name("kshipra prasadaya");
    set_invocation_summary("brahmanda-prakasa aura");
    set_invocation_key(
        "o lord Ganesha, master of all obstacles, for whom chaos never was and never will be, in my time of need "
        "help me see the path that I walk, the path starting when the world hatched from its egg, ending in eternity"
    );
    set_invocation_help(
        "This invocation entreats Ganesha for a brahmanda-prakasa, a world-egg, a reflection of the cosmos at its "
        "birth.  The brahmanda-brakasa manifests itself as an aura of starry, cosmic energy.  The aura will protect "
        "one in innumerable ways.  The higher one's karma phala, and ability with this invocation, the more effective "
        "its protection will be.  To release the brahmanda-prakasa, repeat the invocation.\n\f"
        "The aura can synchronize with one's chakras, if one has any.  Which chakra it is attuned to will affect its "
        "protective properties.  To change the chakra it is attuned to, simply tap the desired chakra."
    );
    set_invocation_cost(({
        ({ Energy_Spiritual, 100 }),
    }));
    set_invocation_cost_bypass_condition(([
        Condition_Type_Code                 : Condition_Type_Item,
        Condition_Info                      : "BRAHMANDA-PRAKASA",
    ]));
    set_invocation_bestowal("brahmanda-prakasa");
    set_invocation_ability_maximum(Travelers_Invocation_Ability_Maximum_Very_High);
    set_invocation_delay(Time_Second * 8);
    set_invocation_ability_determinants(([
        Condition_Skill_Composite           : True,
        Skill_Theurgy                         : 0.25,
        Skill_Conjuration                   : 0.25,
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
    set_invocation_execute_do_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        int sonority = query_invocation_sonority(who);
        if(Condition_Apply(query_invocation_cost_bypass_condition(), who, 0)) {
            object aura = present("BRAHMANDA-PRAKASA", who, who);
            who->display(([
                Message_Content         : ({
                    0, ({ "feel", 0 }), "Ganesha work to blend", ({ 's', 0, aura }), "with the extant cosmos"
                }),
                Message_Senses          : Message_Sense_Cognitive,
            ]));
            who->message(([
                Message_Content         : ({
                    self_color("starry", 0, "bubbles"), "swirl outward from", ({ 's', 0, aura }), "as it fades",
                    "into the surrounding", Description(Description_Type_Ambient_Medium_Colored_Name)
                }),
                Message_Senses          : Message_Sense_Astral | Message_Sense_Visual,
            ]));
            aura->remove();
        } else {
            object aura = new(Travelers_Misc("brahmanda-prakasa"));
            unless(aura->personal_enchantment_initialize(who, sonority))
                safe_destruct(aura);
        }
        return;
    :));
}
