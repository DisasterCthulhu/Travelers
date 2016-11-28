#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("deafness");
    set_obstacle_rarity(Rarity_Common);
    set_obstacle_value(Travelers_Obstacle_Value_Low);
    set_challenge_component_selection_adjustments(([
        "experience"            : 0.75,
        "exploration"           : 0.10,
        "pilgrimage"            : 0.10,
        "quest"                 : 1.25,
        "skill experience"      : 0.75,
        "charity"               : 0.75,
        "neophyte guidance"     : 0.00,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "an oppressive pall of silence shutting down", ({ 'r', 0, "auditory faculty" }),
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Auditory,
    ]));
    add_obstacle_trait_modifier(([
        Modifier_Index          : ({
            Trait_Deafness,
        }),
        Modifier_Amount         : 20,
    ]));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), "the pall of silence over", ({ 'r', 0, "auditory faculty" }), "being lifted"
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Auditory,
    ]));
}
