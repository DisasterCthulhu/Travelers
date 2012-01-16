#include <add_experience.h>
#include <daemon.h>
#include <experience.h>
#include <Travelers.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("minor experience");
	set_bestowal_type(Travelers_Bestowal_Type_Recurring);
	set_bestowal_rarity(Rarity_Common);
	set_bestowal_universality(True);
	set_bestowal_value(Travelers_Bestowal_Value_Very_Low);
	set_bestowal_eligibility_condition(([
		Condition_Type_Code                     : Condition_Type_Level,
		Condition_Value                         : Maximum_Level,
		Condition_Flags                         : Condition_Flag_Inverse,
	]));
	set_bestowal_description("a bit of experience");
	set_bestowal_reward_process((:
		object who = $1;
		int xp = Experience_Reward(Effect_Class_Minor, who);
		who->add_experience(xp, Add_Experience_Flag_Unsplit | Add_Experience_Flag_Emotion);
		return;
	:));
}
