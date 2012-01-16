#include <daemon.h>
#include <quests.h>
#include <Travelers.h>

inherit Travelers_Definition("Requirement");

void travelers_requirement_quest_at_quest_update(mapping info) {
	object who = info["who"];
	object challenge = Travelers_Find_Challenge(who);
	challenge->ganesha_challenge_overcome();
}

void configure() {
	::configure();
	set_requirement_name("quest");
	set_requirement_rarity(Rarity_Common);
	set_requirement_value(Travelers_Requirement_Value_Moderate);
	set_requirement_eligibility_condition(([
	    Condition_Type_Code                     : Condition_Type_Total,
	    Condition_Value                         : 80,
	    Condition_Flags                         : Condition_Flag_Inverse,
	    Condition_Info                          : ({
	        ([
		        Condition_Type_Code             : Condition_Type_Metric,
		        Condition_Info                  : "quest percentage",
		    ]),
            ([
                Condition_Type_Code             : Condition_Type_Pacifist,
                Condition_Value                 : 30,
            ]),
        }),
	]));
	set_requirement_initialize_description("completing a quest");
	set_requirement_overcome_description("successfully quested");
	add_requirement_hook(At_Quest_Update, #'travelers_requirement_quest_at_quest_update);
}
