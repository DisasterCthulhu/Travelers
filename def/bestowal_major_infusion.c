#include <daemon.h>
#include <Travelers.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("major infusion");
	set_bestowal_type(Travelers_Bestowal_Type_Recurring);
	set_bestowal_rarity(Rarity_Very_Unusual);
	set_bestowal_universality(True);
	set_bestowal_value(Travelers_Bestowal_Value_Low);
	set_bestowal_description("considerable infusion of esoteric energies");
	set_bestowal_eligibility_condition(([
	    Condition_Type_Code                     : Condition_Type_Or,
	    Condition_Info                          : ({
	    	([
	    		Condition_Type_Code             : Condition_Type_Skill_Pedagogy,
	    		Condition_Info                  : Skill_Pedagogy_Body_Alteration,
			]),
	    	([
	    		Condition_Type_Code             : Condition_Type_Skill_Pedagogy,
	    		Condition_Info                  : Skill_Pedagogy_Spirit_Alteration,
			]),
		}),
	]));
	set_bestowal_reward_process((:
		object who = $1;
		float reward = diminishing_returns(bestowal_query_total_skill_experience(who), 80);
		reward = max(reward, 15000.0);
		int array skills = ({});
		foreach(int skill : who->query_skills()) {
			object def = Skill(skill);
			if(def->query_skill_require_ability() && !who->query_skill_known(skill))
				continue;
			switch(def->query_skill_pedagogy()) {
			case Skill_Pedagogy_Body_Alteration     :
			case Skill_Pedagogy_Spirit_Alteration   :
				if(def->query_skill_class() != Skill_Class_Divine)
				    if(skill != Skill_Qlippotic_Affinity)
					    skills += ({ skill });
				break;
			}
		}
		if(!sizeof(skills))
			return;
		mapping to_reward = ([]);
		while(reward > 0) {
			float value = fuzz(4000);
			reward -= value;
			to_reward[random_element(skills)] += value;
		}
		if(!sizeof(to_reward))
			return;
		string array skill_list = map(keys(to_reward), (: Skill($1)->query_skill_subjective_description($2) :), who);
		who->display(([
			Message_Content                     : ({
				0, ({ "feel", 0 }), "energies of", list_array(skill_list), "flowing into", ({ 'o', 0 }),
			}),
			Message_Senses                      : Message_Sense_Spiritual,
			Message_Color                       : "status: enhancement",
		]));
		foreach(int skill, float amount : to_reward)
			who->add_skill_exp(skill, amount);
		return;
	:));
}
