#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("hesitancy");
    set_obstacle_rarity(Rarity_Rare);
    set_obstacle_value(Travelers_Obstacle_Value_High);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 0.50,
        "pilgrimage"            : 0.25,
        "quest"                 : 1.00,
        "skill experience"      : 0.00,
        "charity"               : 0.00,
        "neophyte guidance"     : 0.25,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "sense", 0 }), "a powerful force preventing", ({ 'o', 0 }), "from taking the initiative in combat",
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
    set_obstacle_initialize_process((:
        object who = $1->ganesha_challenge_query_owner();
        object field = new(Travelers_Misc("ganesha_paralysis_field"));
        field->personal_enchantment_initialize(who);
        field->set_info(query_obstacle_modifier_tag(), True);
        return;
    :));
    set_obstacle_overcome_process((:
        object who = $1->ganesha_challenge_query_owner();
        foreach(object paralysis_field : who->query_child_objects("is_ganesha_paralysis_field"))
            if(paralysis_field->query_info(query_obstacle_modifier_tag()))
                paralysis_field->personal_enchantment_expire();
        return;
    :));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "sense", 0 }), "lifting of the force previously preventing", ({ 'o', 0 }), "from taking the initiative in combat",
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
}
