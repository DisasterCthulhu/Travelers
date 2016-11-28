#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Requirement");

void travelers_requirement_exploration_do_move(mapping info) {
    object who = info["what"];
    object challenge = Travelers_Find_Challenge(who);
    int points = who->query_exploration_points();
    if(points >= challenge->query_info("Exploration_Points_Required"))
        challenge->ganesha_challenge_overcome();
    else
        requirement_check_notification(challenge, points, "Exploration_Points_Begun_With", "Exploration_Points_Required");
}

void configure() {
    ::configure();
    set_requirement_name("exploration");
    set_requirement_rarity(Rarity_Common);
    set_requirement_value(Travelers_Requirement_Value_Low);
    set_requirement_initialize_description("exploration of the world");
    set_requirement_overcome_description("explored enough of the world");
    set_requirement_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_Metric,
        Condition_Info                          : "global exploration percentage",
        Condition_Value                         : 83,
        Condition_Flags                         : Condition_Flag_Inverse,
    ]));
    set_requirement_initialize_process((:
        int points = $1->ganesha_challenge_query_owner()->query_exploration_points();
        $1->set_info("Exploration_Points_Begun_With", points);
        $1->set_info("Exploration_Points_Required", points + semirandom(300));
        return;
    :));
    add_requirement_hook(Do_Move, #'travelers_requirement_exploration_do_move);
}
