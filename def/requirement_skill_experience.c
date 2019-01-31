#include <Travelers.h>
#include <learning_source.h>
#include <learning_type.h>
#include <daemon.h>

inherit Travelers_Definition("Requirement");

void travelers_requirement_experience_at_skill_experience_add(record event) {
    object who = event[Skill_Experience_Add_Event_Actor];
    float exp = event[Skill_Experience_Add_Event_Amount];
    unless(exp > 0)
        return;
    if(!event[Skill_Experience_Add_Event_Instructor]) {
        mixed skill = event[Skill_Experience_Add_Event_Skill];
        if(Skill(skill)->query_skill_class() == Skill_Class_Language)
            return;
    }
    string source;
    mixed sources = event[Skill_Experience_Add_Event_Sources];
    if(sources) {
        switch(typeof(sources)) {
        case T_STRING  :
            source = sources;
            break;
        case T_POINTER :
            if(Is_Learning_Source(sources)) {
                descriptor dxr = sources;
                if(Is_Learning_Type(dxr[Learning_Source_Type])){
                    switch(typeof(dxr[Learning_Source_Type][Learning_Type_Spec])) {
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
                foreach(descriptor dxr : sources) {
                    if(Is_Learning_Source(dxr) && Is_Learning_Type(dxr[Learning_Source_Type])) {
                        switch(typeof(dxr[Learning_Source_Type][Learning_Type_Spec])){
                        case T_STRING:
                            source = dxr[Learning_Source_Type][Learning_Type_Spec];
                            break;
                        case T_POINTER:
                            source = dxr[Learning_Source_Type][Learning_Type_Spec][0];
                            break;
                        default:
                            // Invalid Learning Type Spec
                            break;
                        }
                    }
                }
            }
            break;
        default:
            //"Invalid typeof(sources)
        }
    }
    if(source == "memories returning")
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
        exp_needed = diminishing_returns(exp_needed / 75, 50000);
        $1->set_info("Skill_Experience_Required", exp_needed);
        $1->set_info("Skill_Experience_Gained", 0.0);
        return;
    :));
    add_requirement_hook(At_Skill_Experience_Add, #'travelers_requirement_experience_at_skill_experience_add);
}
