#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

void configure() {
    ::configure();
    set_obstacle_name("akrasia");
    set_obstacle_rarity(Rarity_Very_Rare);
    set_obstacle_value(Travelers_Obstacle_Value_Very_High);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 0.50,
        "pilgrimage"            : 0.50,
        "quest"                 : 1.00,
        "skill experience"      : 1.00,
        "charity"               : 0.50,
        "neophyte guidance"     : 0.00,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            "every possible action, mental and physical, suddenly seems immensely difficult;", 0, "momentarily",
            ({ "attempt", 0 }), "to fight to remain in controlled focus, but it is hopeless, and", ({ 'p', 0 }),
            ({ "reliquish", 0 }), "guidance over", ({ 'r', 0, "body and mind" }),
        }),
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
        Message_Color           : "status: loss",
    ]));
    add_obstacle_scaled_attribute_modifier(([
        Modifier_Index          : ({
            Attribute_Willpower,
        }),
        Modifier_Amount         : 0.95,
    ]));
    add_obstacle_scaled_attribute_modifier(([
        Modifier_Index          : ({
            Attribute_Ego,
        }),
        Modifier_Amount         : 0.45,
    ]));
    add_obstacle_scaled_skill_modifier(([
        Modifier_Index          : ({
            Skill_Somatesthesia,
            Skill_Concentration,
            Skill_Courage,
            Skill_Discipline,
            Skill_Meditation,
            Skill_Pain_Tolerance,
            Skill_Practice,
            Skill_Tenacity,
            Skill_Subordination,
            Skill_Massive_Exertion,
        }),
        Modifier_Amount         : 0.95,
    ]));
    add_obstacle_scaled_skill_modifier(([
        Modifier_Index          : ({
            Skill_Carousing,
            Skill_Centering,
            Skill_Equilibrium,
            Skill_Intimidation,
            Skill_Killer_Instinct,
            Skill_Leadership,
        }),
        Modifier_Amount         : 0.45,
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
            0, ({ "feel", 0 }), "an overbearing difficulty hanging over every possible action lift as",
            ({ 'r', 0, "ability", }), "to make decisions and control", ({ 'r', 0, "body" }), "return",
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
}
