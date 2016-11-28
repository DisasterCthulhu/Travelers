#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("bad luck");
    set_obstacle_rarity(Rarity_Common);
    set_obstacle_value(Travelers_Obstacle_Value_Very_Low);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 0.50,
        "pilgrimage"            : 0.00,
        "quest"                 : 1.00,
        "skill experience"      : 0.75,
        "charity"               : 0.50,
        "neophyte guidance"     : 1.00,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "strange, almost as though the cosmos were frowning upon", ({ 'o', 0 }),
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
    add_obstacle_trait_modifier(([
        Modifier_Index          : ({
            Trait_Luck,
        }),
        Modifier_Amount         : -200,
    ]));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "strange, as though a pestering cosmic force has been lifted from", ({ 'o', 0 }),
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
}
