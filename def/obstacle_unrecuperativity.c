#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("unrecuperativity");
    set_obstacle_rarity(Rarity_Very_Rare);
    set_obstacle_value(Travelers_Obstacle_Value_Very_High);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 1.00,
        "pilgrimage"            : 1.00,
        "quest"                 : 1.00,
        "skill experience"      : 1.00,
        "charity"               : 1.00,
        "neophyte guidance"     : 1.00,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "a void opening within", ({ 'r', 0, "body" }),
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Kinesthetic,
    ]));
    add_obstacle_trait_modifier(([
        Modifier_Index          : ({
            Trait_Unrecuperativity,
        }),
        Modifier_Amount         : 20,
    ]));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "the void within", ({ 'r', 0, "body" }), "being quelled"
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Kinesthetic,
    ]));
}
