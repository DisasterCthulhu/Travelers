#include <daemon.h>
#include <experience.h>
#include <Travelers.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("great experience");
    set_bestowal_type(Travelers_Bestowal_Type_Recurring);
    set_bestowal_rarity(Rarity_Rare);
    set_bestowal_universality(True);
    set_bestowal_value(Travelers_Bestowal_Value_Moderate);
    set_bestowal_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_Level,
        Condition_Value                         : Maximum_Level,
        Condition_Flags                         : Condition_Flag_Inverse,
    ]));
    set_bestowal_description("tremendous experience");
    set_bestowal_reward_process((:
        object who = $1;
        int xp = Experience_Reward(Effect_Class_Great, who);
        if(who->query_worships("ganesha")) {
            xp *= 3;
            xp /= 2;
        }
        who->add_experience(xp, Add_Experience_Flag_Unsplit | Add_Experience_Flag_Emotion);
        return;
    :));
}
