#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <daemon.h>
#include <profanity.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_profanity_do_speak(descriptor dxr) {
    if(Speech_Flag_Check(dxr, Speech_Flag_OOC))
        return;
    object who = Speech_Query(dxr, Speech_Speaker);
    string orig = Speech_Query(dxr, Speech_Original_Message);
    if(!orig || orig == "")
        return;
    if(!Daemon_Profanity->contains_profanity(orig, Profanity_Level_High | Profanity_Level_Medium | Profanity_Level_Low))
        return;
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
    ::configure();
    set_creator("chaos");
    set_taboo_name("profanity");
    set_taboo_rarity(Rarity_Very_Unusual);
    set_taboo_value(Travelers_Taboo_Value_Very_Low);
    set_taboo_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_And,
        Condition_Info                          : ({
            ([
                Condition_Type_Code             : Condition_Type_Trait,
                Condition_Info                  : Trait_Muteness,
                Condition_Flags                 : Condition_Flag_Inverse,
            ]),
            ([
                Condition_Type_Code             : Condition_Type_Mental_Disorder,
                Condition_Info                  : ({ "Tourette's syndrome" }),
                Condition_Flags                 : Condition_Flag_Inverse | Condition_Flag_Skip_Validation,
            ]),
        }),
    ]));
    set_taboo_initialize_description("using profanity");
    set_taboo_overcome_description("using profanity");
    set_challenge_component_selection_adjustments(([
        "neophyte guidance"                     : 1.50,
        //
        "apophasis"                             : 0.00,
        "muteness"                              : 0.00,
        "deafness"                              : 0.50,
    ]));
    add_taboo_hook(Do_Speak, #'travelers_taboo_profanity_do_speak);
}
