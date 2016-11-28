#include <daemon.h>
#include <specialty_access.h>
#include <Travelers.h>
#include <Wild_Talents.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("soul strengthening");
    set_bestowal_type(Travelers_Bestowal_Type_Recurring);
    set_bestowal_rarity(Rarity_Common);
    set_bestowal_universality(True);
    set_bestowal_value(Travelers_Bestowal_Value_Very_Low);
    set_bestowal_description("a strengthening of the soul");
    set_bestowal_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_And,
        Condition_Info                          : ({
            Condition_Type_Incarnos,
            ([
                Condition_Type_Code             : Condition_Type_Trait,
                Condition_Info                  : Trait_Perdition,
                Condition_Flags                 : Condition_Flag_Inverse,
            ]),
            ([
                Condition_Type_Code             : Condition_Type_Lives,
                Condition_Value                 : Max_Lives,
                Condition_Flags                 : Condition_Flag_Inverse,
            ]),
        }),
    ]));
    set_bestowal_reward_process((:
        object who = $1;
        who->display(([
            Message_Content                     : ({
                0, ({ "feel", 0 }), "a magisterial presence enter into and strengthen", ({ 'r', 0, "soul" }),
                ", fortifying it against the eventuality of dissipation upon death"
            }),
            Message_Senses                      : Message_Sense_Cognitive,
            Message_Color                       : "dazzling golden",
        ]));
        who->add_life();
        return;
    :));
}
