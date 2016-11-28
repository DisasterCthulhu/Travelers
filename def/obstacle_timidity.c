#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("timidity");
    set_obstacle_rarity(Rarity_Very_Unusual);
    set_obstacle_value(Travelers_Obstacle_Value_Moderate);
    set_challenge_component_selection_adjustments(([
        "experience"            : 0.75,
        "exploration"           : 0.50,
        "pilgrimage"            : 0.75,
        "quest"                 : 0.50,
        "skill experience"      : 1.00,
        "charity"               : 0.50,
        "neophyte guidance"     : 0.10,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "a tremendous weakness and timidity taking hold of", ({ 'r', 0, "very personality" }),
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
    add_obstacle_scaled_attribute_modifier(([
        Modifier_Index          : ({
            Attr_Ego,
        }),
        Modifier_Amount         : 0.9,
    ]));
    add_obstacle_scaled_attribute_modifier(([
        Modifier_Index          : ({
            Attr_Wil,
        }),
        Modifier_Amount         : 0.3,
    ]));
    add_obstacle_scaled_skill_modifier(([
        Modifier_Index          : ({
            Skill_Manipulation,
            Skill_Summoning,
            Skill_Diplomacy,
            Skill_Carousing,
            Skill_Rhetoric,
            Skill_Fast_Talk,
            Skill_Introspection,
            Skill_Erotic_Arts,
            Skill_Leadership,
            Skill_Subordination,
            Skill_Haggling,
            Skill_Ownership,
            Skill_Etiquette,
            Skill_Concentration,
            Skill_Invocation,
            Skill_Evocation,
            Skill_Power_Channeling,
            Skill_Telesma,
        }),
        Modifier_Amount         : 0.9,
    ]));
    add_obstacle_scaled_skill_modifier(([
        Modifier_Index          : ({
            Skill_Courage,
            Skill_Tenacity,
            Skill_Discipline,
        }),
        Modifier_Amount         : 0.3,
    ]));
    set_obstacle_attach_process((:
        object who = $1->ganesha_challenge_query_owner();
        who->constrain_effort();
        return;
    :));
    set_obstacle_detach_process((:
        object who = $1->ganesha_challenge_query_owner();
        who->constrain_effort();
        return;
    :));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "the strength of", ({ 'r', 0, "personal" }), "influence returning",
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
}
