#include <Travelers.h>
#include <daemon.h>
#include <specialty_access.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("greater hyperspatiality");
	set_bestowal_type(Travelers_Bestowal_Type_Singular);
	set_bestowal_rarity(Rarity_Very_Exotic);
	set_bestowal_universality(False);
	set_bestowal_value(Travelers_Bestowal_Value_Very_High);
	set_bestowal_description("a greater degree of hyperspatial awareness");
	set_bestowal_eligibility_condition(([
	    Condition_Type_Code                     : Condition_Type_And,
	    Condition_Info                          : ({
	        ([
	    		Condition_Type_Code             : Condition_Type_Trait,
	    		Condition_Info                  : Trait_Hyperspatiality,
	    		Condition_Value                 : 1,
		    ]),
	        ([
	    		Condition_Type_Code             : Condition_Type_Trait,
	    		Condition_Info                  : Trait_Hyperspatiality,
	    		Condition_Value                 : 2,
	    		Condition_Flags                 : Condition_Flag_Inverse,
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
                0, ({ "feel", 0 }), ({ 'r', 0, "consciousness" }), "expanding to encompass the fifth spatial dimension",
            }),
            Message_Senses                      : Message_Sense_Cognitive,
            Message_Color                       : "status: enhancement",
        ]));
        $1->add_permanent_trait_adjustment(Trait_Hyperspatiality, 1);
        return;
    :));
}
