#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Obstacle");

nosave private mixed array harassment_damage_type_sets;
nosave private int array harassment_power_levels;

void traveler_obstacle_harassment_execute(object who) {
    descriptor spc = Special_Attack(([
        Special_Attack_Type         : random_element(harassment_damage_type_sets),
        Special_Attack_Rating       : semirandom(5000),
        Special_Attack_Power        : random_element(harassment_power_levels),
        Special_Attack_Pass_Flags   : Special_Attack_Pass_Flag_Suppress_Result_Damage,
        Special_Attack_From         : "nowhere",
    ]));
    Special_Attack_Execute(spc, 0, who);
}

void traveler_obstacle_harassment_at_heart_beat(object who) {
    if(random(10))
        return;
    traveler_obstacle_harassment_execute(who);
}

void preinit() {
    ::preinit();
    harassment_damage_type_sets = ({
        ({ "qlippotic", "sephirotic", "divine" }),
        ({ "divine" }),
        ({ "goetia" }),
        ({ "goetia", "divine" }),
    });
    harassment_power_levels = ({
        Special_Attack_Power_Feeble,
        Special_Attack_Power_Feeble,
        Special_Attack_Power_Very_Weak,
        Special_Attack_Power_Very_Weak,
        Special_Attack_Power_Weak,
        Special_Attack_Power_Weak,
        Special_Attack_Power_Somewhat_Weak,
        Special_Attack_Power_Somewhat_Weak,
        Special_Attack_Power_Moderate,
        Special_Attack_Power_Moderate,
        Special_Attack_Power_Somewhat_Strong,
        Special_Attack_Power_Strong,
        Special_Attack_Power_Very_Strong,
    });
}

void configure() {
    ::configure();
    set_creator("chaos");
    set_obstacle_name("harassment");
    set_obstacle_rarity(Rarity_Very_Unusual);
    set_obstacle_value(Travelers_Obstacle_Value_Moderate);
    set_challenge_component_selection_adjustments(([
        "experience"                                : 1.00,
        "exploration"                               : 1.00,
        "pilgrimage"                                : 1.00,
        "quest"                                     : 1.00,
        "skill experience"                          : 1.00,
        "charity"                                   : 1.00,
        "neophyte guidance"                         : 1.00,
    ]));
    set_obstacle_initialize_display(([
        Message_Content                             : ({
            0, ({ "feel", 0 }), "persecuted",
        }),
        Message_Color                               : "status: loss",
        Message_Senses                              : Message_Sense_Emotive,
    ]));
    add_obstacle_hook(At_Heart_Beat, #'traveler_obstacle_harassment_at_heart_beat);
    set_obstacle_overcome_display(([
        Message_Content                             : ({
            ({ 's', 0, "sense of persecution" }), "fades away",
        }),
        Message_Color                               : "status: enhancement",
        Message_Senses                              : Message_Sense_Emotive,
    ]));
}
