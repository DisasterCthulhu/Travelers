#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("spiritual disjunction");
    set_obstacle_rarity(Rarity_Rare);
    set_obstacle_value(Travelers_Obstacle_Value_High);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 1.00,
        "pilgrimage"            : 1.50,
        "quest"                 : 1.00,
        "skill experience"      : 0.25,
        "charity"               : 0.50,
        "neophyte guidance"     : 0.50,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "a void opening within", ({ 'r', 0, "spirit" }),
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Spiritual,
    ]));
    add_obstacle_trait_modifier(([
        Modifier_Index          : ({
            Trait_Spiritual_Disjunction,
        }),
        Modifier_Amount         : 20,
    ]));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "the void within", ({ 'r', 0, "spirit" }), "being quelled"
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Visual,
    ]));
}
