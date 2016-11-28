#include <daemon.h>
#include <services_mental_disorders.h>
#include <Travelers.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("madness");
    set_obstacle_rarity(Rarity_Common);
    set_obstacle_value(Travelers_Obstacle_Value_High);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 0.50,
        "pilgrimage"            : 0.25,
        "quest"                 : 1.00,
        "skill experience"      : 1.00,
        "charity"               : 0.50,
        "neophyte guidance"     : 0.10,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), ({ 'r', 0, "mental pathways" }), "twist and depart from their normal functioning"
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
    add_obstacle_scaled_skill_modifier(([
        Modifier_Index          : ({
            Skill_Psychic_Integrity,
        }),
        Modifier_Amount         : 0.90,
    ]));
    add_obstacle_scaled_skill_modifier(([
        Modifier_Index          : ({
            Skill_Concentration,
            Skill_Courage,
            Skill_Discipline,
            Skill_Imagination,
            Skill_Introspection,
            Skill_Memory,
            Skill_Tenacity,
        }),
        Modifier_Amount         : 0.30,
    ]));
    set_obstacle_initialize_process((:
        object who = $1->ganesha_challenge_query_owner();
        for(int i = 0, int count = semirandom(6); i < count; i++) {
            string type = Daemon_Mental_Disorders->query_random_new_mental_disorder(who);
            if(type) {
                object disorder = type->inflict_mental_disorder(who, 0, True);
                if(disorder) {
                    disorder->set_info(query_obstacle_modifier_tag(), True);
                    disorder->set_indestructible(True);
                }
            } else {
                break;
            }
        }
        return;
    :));
    set_obstacle_overcome_process((:
        object who = $1->ganesha_challenge_query_owner();
        foreach(object disorder : filter(all_inventory(who), (: $1->is_mental_disorder() :)))
            if(disorder && disorder->query_info(query_obstacle_modifier_tag()))
                disorder->remove();
        return;
    :));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), ({ 'r', 0, "mind" }), "straighten and return to normal functioning"
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
}
