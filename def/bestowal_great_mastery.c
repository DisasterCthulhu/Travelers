#include <daemon.h>
#include <Travelers.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("great mastery");
	set_bestowal_type(Travelers_Bestowal_Type_Recurring);
	set_bestowal_rarity(Rarity_Very_Rare);
	set_bestowal_universality(True);
	set_bestowal_value(Travelers_Bestowal_Value_Moderate);
	set_bestowal_description("tremendous mastery of physical skills");
	set_bestowal_eligibility_condition(([
	    Condition_Type_Code                     : Condition_Type_Skill_Pedagogy,
	    Condition_Info                          : Skill_Pedagogy_Physical_Training,
	]));
	set_bestowal_reward_process((:
		object who = $1;
		float reward = diminishing_returns(bestowal_query_total_skill_experience(who), 400);
		reward = max(reward, 40000.0);
		int array skills = ({});
		foreach(int skill : who->query_skills()) {
			object def = Skill(skill);
			if(def->query_skill_pedagogy() != Skill_Pedagogy_Physical_Training)
				continue;
			if(def->query_skill_class() == Skill_Class_Divine)
				continue;
			if(def->query_skill_require_ability() && !who->query_skill_known(skill))
				continue;
			skills += ({ skill });
		}
		if(!sizeof(skills))
			return;
		mapping to_reward = ([]);
		while(reward > 0) {
			float value = fuzz(8000);
			reward -= value;
			to_reward[random_element(skills)] += value;
		}
		if(!sizeof(to_reward))
			return;
		string array skill_list = map(keys(to_reward), (: Skill($1)->query_skill_subjective_description($2) :), who);
		who->display(([
			Message_Content                     : ({
				0, ({ "feel", 0 }), ({ 'r', 0, "mastery" }), "of", list_array(skill_list), "increasing",
			}),
			Message_Senses                      : Message_Sense_Kinesthetic,
			Message_Color                       : "status: enhancement",
		]));
		foreach(int skill, float amount : to_reward)
			who->add_skill_exp(skill, amount);
		return;
	:));
}
