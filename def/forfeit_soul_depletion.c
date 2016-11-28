#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <die.h>

inherit Travelers_Definition("Forfeit");

void configure() {
    ::configure();
    set_forfeit_name("soul depletion");
    set_forfeit_rarity(Rarity_Unusual);
    set_forfeit_value(Travelers_Forfeit_Value_Moderate);
    set_forfeit_initialize_description("soul depletion");
    set_forfeit_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_And,
        Condition_Info                          : ({
            ([
                Condition_Type_Code             : Condition_Type_Trait,
                Condition_Info                  : Trait_Perdition,
                Condition_Flags                 : Condition_Flag_Inverse,
            ]),
            ([
                Condition_Type_Code             : Condition_Type_Lives,
                Condition_Value                 : 2,
            ]),
        }),
    ]));
    set_challenge_component_selection_adjustments(([
        "dying"                                 : 0.00,
    ]));
    set_forfeit_display(([
        Message_Content                         : ({
            0, ({ "feel", 0 }), ({ 'r', 0, "very soul" }), "weakening",
        }),
        Message_Senses                          : Message_Sense_Spiritual,
        Message_Color                           : "status: danger",
    ]));
    set_forfeit_process((:
        object who = $1->ganesha_challenge_query_owner();
        int lives = who->query_lives();
        while(lives > 1) {
            who->remove_life();
            lives--;
            if(!random(2))
                break;
        }
        return;
    :));
}
