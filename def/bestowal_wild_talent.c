#include <daemon.h>
#include <specialty_access.h>
#include <Travelers.h>
#include <Wild_Talents.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("wild talent");
	set_bestowal_type(Travelers_Bestowal_Type_Singular);
	set_bestowal_rarity(Rarity_Very_Exotic);
	set_bestowal_universality(True);
	set_bestowal_value(Travelers_Bestowal_Value_Very_High);
	set_bestowal_description("a wild talent");
	set_bestowal_eligibility_condition(([
	    Condition_Type_Code                     : Condition_Type_And,
	    Condition_Info                          : ({
	        ([
		        Condition_Type_Code             : Condition_Type_Psychic_Race,
		    ]),
	        ([
		        Condition_Type_Code             : Condition_Type_Metric,
		        Condition_Info                  : "karma phala",
		        Condition_Value                 : 1000,
		    ]),
		}),
	]));
    set_bestowal_reward_process((:
        object who = $1;
        who->display(([
            Message_Content                     : ({
                0, ({ "feel", 0 }), "a magisterial presence enter into and transform", ({ 'r', 0, "mind" }),
            }),
            Message_Senses                      : Message_Sense_Cognitive,
            Message_Color                       : "sparkling golden",
        ]));
        Wild_Talents_Control->grant_talent(who, "consciousness alteration", 1000, ([
            "planeswalker"                      : 400,
            "fetch"                             : 400,
            "mindreader"                        : 200,
            "mindshielder"                      : 200,
            "levitator"                         : 100,
        ]));
        return;
    :));
}
