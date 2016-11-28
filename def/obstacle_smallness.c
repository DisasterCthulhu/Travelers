#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("smallness");
    set_obstacle_rarity(Rarity_Very_Rare);
    set_obstacle_value(Travelers_Obstacle_Value_High);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 0.50,
        "pilgrimage"            : 1.00,
        "quest"                 : 0.50,
        "skill experience"      : 0.25,
        "charity"               : 0.25,
        "neophyte guidance"     : 0.50,
    ]));
    set_obstacle_eligibility_condition(([
        Condition_Type_Code     : Condition_Type_Attribute,
        Condition_Info          : Attribute_Size,
        Condition_Value         : 15,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), ({ 'l', 0 }), "shrinking",
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
            Attr_Siz,
        }),
        Modifier_Amount         : 1.00,
    ]));
    set_obstacle_attach_process((:
        object who = $1->ganesha_challenge_query_owner();
        who->message(([
            Message_Content     : ({
                0, ({ "shrink", 0 }), "in size",
            }),
            Message_Senses      : Message_Sense_Visual,
        ]));
        return;
    :));
    set_obstacle_detach_process((:
        object who = $1->ganesha_challenge_query_owner();
        who->message(([
            Message_Content     : ({
                0, ({ "grow", 0 }), "in size",
            }),
            Message_Senses      : Message_Sense_Visual,
        ]));
        return;
    :));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "feel", 0 }), ({ 'l', 0 }), "returning to", ({ 'r', 0, "normal size" }),
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
}
