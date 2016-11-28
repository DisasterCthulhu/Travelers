#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_being_mortally_wounded_do_be_mortally_wounded(mapping args) {
    object who = args["who"];
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
    ::configure();
    set_creator("chaos");
    set_taboo_name("being mortally wounded");
    set_taboo_rarity(Rarity_Unusual);
    set_taboo_value(Travelers_Taboo_Value_Low);
    set_taboo_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_And,
        Condition_Info                          : ({
            ([
                Condition_Type_Code             : Condition_Type_Limb_Class,
                Condition_Info                  : Limb_Class_Bleed,
            ]),
            ([
                Condition_Type_Code             : Condition_Type_Trait,
                Condition_Info                  : Trait_Anhumouricity,
                Condition_Flags                 : Condition_Flag_Inverse,
            ]),
        }),
    ]));
    set_taboo_initialize_description("becoming mortally wounded");
    set_taboo_overcome_description("becoming mortally wounded");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 0.05,
        "experience"                            : 1.00,
        "exploration"                           : 0.10,
        "pilgrimage"                            : 0.25,
        "quest"                                 : 0.75,
        "skill experience"                      : 0.05,
        "neophyte guidance"                     : 0.50,
        //
        "frailty"                               : 2.00,
    ]));
    add_taboo_hook(Do_Be_Mortally_Wounded, #'travelers_taboo_being_mortally_wounded_do_be_mortally_wounded);
}
