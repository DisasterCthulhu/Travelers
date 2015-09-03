#include <Travelers.h>
#include <daemon.h>
#include <experience.h>

inherit Travelers_Definition("Requirement");

void travelers_requirement_experience_at_experience_change(mapping info) {
	object who = info["who"];
	int exp = info["experience"];
	object challenge = Travelers_Find_Challenge(who);
	if(!challenge) {
		requirement_disengage_hooks(who);
		return;
	}
	int gained = challenge->query_info("Experience_Gained");
	gained += exp;
	if(gained >= challenge->query_info("Experience_Required")) {
		challenge->ganesha_challenge_overcome();
	} else {
		challenge->set_info("Experience_Gained", gained);
		requirement_check_notification(challenge, gained, 0, "Experience_Required");
	}
}

void configure() {
	::configure();
	set_requirement_name("experience");
	set_requirement_rarity(Rarity_Common);
	set_requirement_value(Travelers_Requirement_Value_High);
	set_requirement_eligibility_condition(([
	    Condition_Type_Code                     : Condition_Type_And,
	    Condition_Info                          : ({
	        ([
		        Condition_Type_Code             : Condition_Type_Level,
		        Condition_Value                 : Maximum_Level - 2,
		        Condition_Flags                 : Condition_Flag_Inverse,
		    ]),
		    ([
		        Condition_Type_Code             : Condition_Type_Kills,
		        Condition_Value                 : 1,
		    ]),
		}),
	]));
	set_requirement_initialize_description("acquiring experience");
	set_requirement_overcome_description("acquired enough experience");
	set_requirement_initialize_process((:
		int level = $1->ganesha_challenge_query_owner()->query_level();
		int exp_needed = (Level_Experience(level + 1) - Level_Experience(level)) / 2;
		exp_needed = round(diminishing_returns(exp_needed, 50000));
		$1->set_info("Experience_Required", exp_needed);
		$1->set_info("Experience_Gained", 0);
		return;
	:));
	add_requirement_hook(At_Experience_Change, #'travelers_requirement_experience_at_experience_change);
}
