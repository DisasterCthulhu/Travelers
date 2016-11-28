#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("frailty");
    set_obstacle_rarity(Rarity_Unusual);
    set_obstacle_value(Travelers_Obstacle_Value_High);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 0.50,
        "pilgrimage"            : 1.00,
        "quest"                 : 1.00,
        "skill experience"      : 0.75,
        "charity"               : 0.50,
        "neophyte guidance"     : 0.50,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "a tremendous weakness and fragility settling into", ({ 'r', 0, "body" }),
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
    add_obstacle_scaled_attribute_modifier(([
        Modifier_Index          : ({
            Attr_Str,
            Attr_Vit,
        }),
        Modifier_Amount         : 0.75,
    ]));
    add_obstacle_scaled_attribute_modifier(([
        Modifier_Index          : ({
            Attr_Agi,
        }),
        Modifier_Amount         : 0.35,
    ]));
    add_obstacle_scaled_skill_modifier(([
        Modifier_Index          : ({
            Skill_Armour_Use,
            Skill_Balance,
            Skill_Hardiness,
            Skill_Load_Bearing,
            Skill_Massive_Blow,
            Skill_Massive_Exertion,
            Skill_Pain_Tolerance,
            Skill_Recuperation,
            //Skill_Regeneration,
            Skill_Resilience,
            Skill_Robustness,
            Skill_Stamina,
            Skill_Steadiness,
            Skill_Supernal_Durability,
        }),
        Modifier_Amount         : 0.90,
    ]));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "much stronger and healthier as the strange frailty departs", ({ 'r', 0, "body" }),
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
}
