#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <die.h>

inherit Travelers_Definition("Forfeit");

void configure() {
    ::configure();
    set_forfeit_name("death");
    set_forfeit_rarity(Rarity_Unusual);
    set_forfeit_value(Travelers_Forfeit_Value_Moderate);
    set_forfeit_initialize_description("death");
    set_forfeit_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_Trait,
        Condition_Info                          : Trait_Perdition,
        Condition_Flags                         : Condition_Flag_Inverse,
    ]));
    set_challenge_component_selection_adjustments(([
        "dismemberment"                         : 0.00,
        "dying"                                 : 0.00,
    ]));
    set_forfeit_message(([
        Message_Content                         : ({
            'a',
            ([
                Message_Content                 : ({
                    "ominous",
                }),
                Message_Senses                  : Message_Sense_Emotive,
            ]),
            "gong sound fills the", Description(Description_Type_Ambient_Medium_Colored_Name),
        }),
        Message_Senses                          : Message_Sense_Auditory,
    ]));
    set_forfeit_display(([
        Message_Content                         : ({
            0, ({ "feel", 0 }), "the very life being sapped from", ({ 'o', 0 }),
        }),
        Message_Senses                          : Message_Sense_Spiritual | Message_Sense_Kinesthetic,
        Message_Color                           : "status: danger",
    ]));
    set_forfeit_process((:
        object who = $1->ganesha_challenge_query_owner();
        object here = environment(who);
        descriptor nl = here && here->add_realm_modifier(([
            Modifier_Index                      : ({
                "NL",
            }),
            Modifier_Amount                     : 1000,
        ]));
        who->die(0, Die_Flag_Suppress_Life_Saving | Die_Flag_Immediate);
        if(here)
            here->remove_realm_modifier(&nl);
        return;
    :));
}
