#include <Travelers.h>
#include <learning_source.h>
#include <learning_type.h>
#include <daemon.h>

inherit Travelers_Definition("Requirement");

void travelers_requirement_experience_at_skill_experience_add(mapping info) {
	object who = info["who"];
	float exp = info["amount"];
	unless(exp > 0)
	    return;
	if(!info["instructor"]) {
	    mixed skill = info["skill"];
	    if(Skill(skill)->query_skill_class() == Skill_Class_Language)
	        return;
	}
    string source;
    if(info["sources"]){
        switch(typeof(info["sources"])) {
        case T_STRING  :
            source = info["sources"];
        case T_POINTER :
            if(Is_Learning_Source(info["sources"])){
                descriptor dxr = info["sources"];
                if(Is_Learning_Type(dxr[Learning_Source_Type])){
                    switch(typeof(dxr[Learning_Source_Type][Learning_Type_Spec])){
                    case T_STRING:
                        source = dxr[Learning_Source_Type][Learning_Type_Spec];
                        break;
                    case T_POINTER:
                        source = dxr[Learning_Source_Type][Learning_Type_Spec][0];
                        break;
                    default:
                        //"Invalid Learning Type Spec");
                        break;
                    }
                }
            } else {
                foreach(descriptor dxr : info["sources"])
                    if(Is_Learning_Source(dxr) && Is_Learning_Type(dxr[Learning_Source_Type])){
                        switch(typeof(dxr[Learning_Source_Type][Learning_Type_Spec])){
                        case T_STRING:
                            source = dxr[Learning_Source_Type][Learning_Type_Spec];
                            break;
                        case T_POINTER:
                            source = dxr[Learning_Source_Type][Learning_Type_Spec][0];
                            break;
                        default:
                            // Invalid Learning Type Spec
                            continue;
                            break;
                        }
                    }
                }
            break;
        default:
            //"Invalid typeof(info['sources'])
        }
    }
    if(source && member(({"memories returning"}), source) != Null)
        return;
	object challenge = Travelers_Find_Challenge(who);
	float gained = challenge->query_info("Skill_Experience_Gained");
	gained += exp;
	if(gained >= challenge->query_info("Skill_Experience_Required")) {
		challenge->ganesha_challenge_overcome();
	} else {
		challenge->set_info("Skill_Experience_Gained", gained);
		requirement_check_notification(challenge, gained, 0, "Skill_Experience_Required");
	}
}

void configure() {
	::configure();
	set_requirement_name("skill experience");
	set_requirement_rarity(Rarity_Very_Unusual);
	set_requirement_value(Travelers_Requirement_Value_Very_Low);
	set_requirement_initialize_description("acquiring skill experience");
	set_requirement_overcome_description("acquired enough skill experience");
	set_requirement_initialize_process((:
		object who = $1->ganesha_challenge_query_owner();
		float exp_needed = 0.0;
		foreach(int skill : who->query_skills())
			exp_needed += who->query_skill_exp(skill);
		exp_needed = diminishing_returns(exp_needed / 50, 100000);
		$1->set_info("Skill_Experience_Required", exp_needed);
		$1->set_info("Skill_Experience_Gained", 0.0);
		return;
	:));
	add_requirement_hook(At_Skill_Experience_Add, #'travelers_requirement_experience_at_skill_experience_add);
}
