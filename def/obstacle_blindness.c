#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("blindness");
    set_obstacle_rarity(Rarity_Rare);
    set_obstacle_value(Travelers_Obstacle_Value_Very_High);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 1.00,
        "pilgrimage"            : 1.00,
        "quest"                 : 1.00,
        "skill experience"      : 1.00,
        "charity"               : 1.00,
        "neophyte guidance"     : 0.10,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "a pall of darkness fall over", ({ 'r', 0, "visual faculty" }),
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Visual,
    ]));
    add_obstacle_trait_modifier(([
        Modifier_Index          : ({
            Trait_Paravision,
        }),
        Modifier_Amount         : -10,
    ]));
    add_obstacle_trait_modifier(([
        Modifier_Index          : ({
            Trait_Blindness,
        }),
        Modifier_Specification  : Null,
        Modifier_Amount         : 20,
    ]));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "the darkness covering", ({ 'r', 0, "vision" }), "being lifted"
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Visual,
    ]));
}
