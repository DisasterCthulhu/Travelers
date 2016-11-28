#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("clumsiness");
    set_obstacle_rarity(Rarity_Common);
    set_obstacle_value(Travelers_Obstacle_Value_High);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 1.25,
        "pilgrimage"            : 1.25,
        "quest"                 : 1.00,
        "skill experience"      : 1.00,
        "charity"               : 1.00,
        "neophyte guidance"     : 0.50,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "a tremendous clumsiness and disorentation settling over", ({ 'o', 0 }),
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
    add_obstacle_scaled_attribute_modifier(([
        Modifier_Index          : ({
            Attr_Per,
            Attr_Agi,
        }),
        Modifier_Amount         : 0.75,
    ]));
    add_obstacle_scaled_skill_modifier(([
        Modifier_Index          : ({
            Skill_Armour_Use,
            Skill_Balance,
            Skill_Calligraphy,
            Skill_Carpentry,
            Skill_Combat_Reflexes,
            Skill_Dancing,
            Skill_Dodge,
            Skill_Driving,
            Skill_Equestrian,
            Skill_Fishing,
            Skill_Flight,
            Skill_Instant_Stand,
            Skill_Lockpicking,
            Skill_Locksmithing,
            Skill_Mounted_Combat,
            Skill_Picking_Pockets,
            Skill_Precision_Strike,
            Skill_Quickness,
            Skill_Riding,
            Skill_Sewing,
            Skill_Steadiness,
            Skill_Stealth,
            Skill_Swimming,
            Skill_Tumbling,
            Skill_Weaving,
        }),
        Modifier_Amount         : 0.75,
    ]));
    add_obstacle_scaled_skill_modifier(([
        Modifier_Index          : Skill_Class(Skill_Class_Close_Combat_Weapon)->query_skill_class_contents()->query_skill_code(),
        Modifier_Amount         : 0.50,
    ]));
    add_obstacle_scaled_skill_modifier(([
        Modifier_Index          : Skill_Class(Skill_Class_Ranged_Weapon)->query_skill_class_contents()->query_skill_code(),
        Modifier_Amount         : 0.90,
    ]));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "much less clumsy as the disorientation departs from", ({ 'o', 0 }),
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
}
