#include <Travelers.h>
#include <daemon.h>
#include <experience.h>

inherit Travelers_Definition("Requirement");

void travelers_requirement_charity_at_wealth_given(mapping info) {
    object who = info["from"];
    object target = info["to"];
    int amount = info["amount"];
    if(amount < 1000)
        return;
    object challenge = Travelers_Find_Challenge(who);
    if(!challenge) {
        requirement_disengage_hooks(who);
        return;
    }
    string ident = target->query_extant() || object_name(target);
    string array individuals = challenge->query_info("Individuals");
    if(member(individuals, ident) != Null)
        return;
    int target_wealth = info["previous_wealth"] + target->query_info("SAVINGS");
    if(target_wealth >= 100)
        return;
    if(!target->sentience() || !target->sentience()->query_sentience_abstraction())
        return;
    if(target->query_extension(LS_Extension("banker")))
        return;
    mapping area_counts = challenge->query_info("Area_Counts");
    string area = 0;
    unless(target->is_incarnos()) {
        object ctl = target->project_control();
        if(!ctl)
            return;
        area = ctl->query_area();
        if(!area)
            return;
    }
    if(area_counts[area] >= 3)
        return;
    int performed = challenge->query_info("Instances_Performed") + 1;
    if(performed >= challenge->query_info("Instances_Required")) {
        challenge->ganesha_challenge_overcome();
    } else {
        challenge->set_info("Instances_Performed", performed);
        individuals += ({ ident });
        challenge->set_info("Individuals", individuals);
        if(area) {
            area_counts[area]++;
            challenge->set_info("Area_Counts", area_counts);
        }
        requirement_check_notification(challenge, performed, 0, "Instances_Required");
    }
}

void configure() {
    ::configure();
    set_creator("chaos");
    set_requirement_name("charity");
    set_requirement_rarity(Rarity_Very_Unusual);
    set_requirement_value(Travelers_Requirement_Value_Low);
    set_requirement_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_Wealth,
        Condition_Value                         : 5000,
    ]));
    set_requirement_initialize_description("providing charity");
    set_requirement_overcome_description("provided enough charity");
    set_requirement_initialize_process((:
        int level = $1->ganesha_challenge_query_owner()->query_level();
        int instances = semirandom(round(diminishing_returns(5 + level, 1)));
        $1->set_info("Instances_Required", instances);
        $1->set_info("Instances_Performed", 0);
        $1->set_info("Individuals", ({}));
        $1->set_info("Area_Counts", ([]));
        return;
    :));
    add_requirement_hook(At_Wealth_Given, #'travelers_requirement_charity_at_wealth_given);
}
