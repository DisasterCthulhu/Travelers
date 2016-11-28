#include <Travelers.h>
#include <daemon.h>
#include <usage.h>

inherit Travelers_Definition("Obstacle");

void traveler_obstacle_flight_nullify_wings(object who) {
    int array wings = who->existing_limbs(Limb_Type_Wing, Existing_Limb_Functional_or_Disabled);
    if(!sizeof(wings))
        return;
    descriptor no_pain;
    unless(who->query_trait(Trait_Analgesia))
        no_pain = who->add_trait_modifier(([
            Modifier_Index                      : Trait_Analgesia,
            Modifier_Amount                     : 100,
            Modifier_Add_Display                : ([
                Message_Content                 : ({
                    0, ({ "feel", 0 }), "a powerful force entering", ({ 'r', 0, "mind and body" }),
                    "and acting as a powerful analgesic",
                }),
                Message_Senses                  : Message_Sense_Kinesthetic | Message_Sense_Spiritual,
                Message_Color                   : "status: enhancement",
            ]),
            Modifier_Remove_Display             : ([
                Message_Content                 : ({
                    0, ({ "feel", 0 }), "the analgesic force leave", ({ 'o', 0 }), "as quickly as it came",
                }),
                Message_Senses                  : Message_Sense_Kinesthetic | Message_Sense_Spiritual,
                Message_Color                   : "status: loss",
            ]),
        ]));
    float how_much = who->do_damage(([
        Attack_Damage_Type                      : "divine",
        Attack_Limb                             : wings,
        Attack_Damage                           : (accumulate(map(wings, (: $2->query_max_limb_hp($1) :), who)) + 1) * 100,
        Attack_Flags                            : Attack_Flag_Limb_Preset | Attack_Flag_Bypass_Armour | Attack_Flag_Untreatable | Attack_Flag_Display_Damage_Message,
    ]));
    wings = who->existing_limbs(Limb_Type_Wing, Existing_Limb_Functional_or_Disabled);
    if(sizeof(wings))
        foreach(int i : wings)
            who->sever_limb(i);
    if(no_pain)
        who->remove_trait_modifier(&no_pain);
}

void traveler_obstacle_flight_proscription_reject_mounts_and_carriers(object who) {
    object mount = who->query_mount();
    if(mount) {
        who->display(([
            Message_Content                     : ({
                0, ({ "feel", 0 }), "something forcing", ({ 'o', 0 }), "to dismount from", mount
            }),
            Message_Senses                      : Message_Sense_Kinesthetic | Message_Sense_Spiritual,
            Message_Color                       : "alert: medium",
        ]));
        who->set_mount(0, True);
    }
    object here = environment(who);
    unless(here && here->is_character())
        return;
    object destination;
    if(!environment(here)->is_character())
        destination = environment(here);
    else
    destination = Random_Location_Safe;
    who->display(([
        Message_Content                         : ({
            0, ({ "feel", 0 }), "something forcing", ({ 'o', 0 }), "out of", ({ 's', here, "inventory" }),
        }),
        Message_Senses                          : Message_Sense_Kinesthetic | Message_Sense_Spiritual,
        Message_Color                           : "alert: medium",
    ]));
    who->move_living(destination, "in a {{prismatic}flash of prismatic radiance}", "in a {{prismatic}flash of prismatic radiance}");
}

void traveler_obstacle_flight_proscription_restore_wings(object who) {
    unless(who)
        return;
    int array wings = who->existing_limbs(Limb_Type_Wing, Existing_Limb_Amputated);
    if(!sizeof(wings))
        return;
    who->message(([
        Message_Content                         : ({
            ({ 's', 0, wings }), ({ "are", wings }), "restored in a {{prismatic}flurried burst of prismatic sparks}"
        }),
        Message_Senses                          : Message_Sense_Kinesthetic | Message_Sense_Astral | Message_Sense_Visual,
    ]));
    foreach(int i : wings)
        who->restore_limb(i);
}

status traveler_obstacle_flight_proscription_can_have_limb_restored(mapping args) {
    int limb = args["limb"];
    object who = args["who"];
    if(Limb_Type(who->query_limb_type(limb))->query_limb_type_base() == Limb_Type_Wing)
        return Error(({
            "a powerful force prevents", who, "from having", ({ 'r', who, limb }), "restored",
        }));
    return True;
}

status traveler_obstacle_flight_proscription_can_mount(mapping args) {
    object who = args["who"];
    object mount = args["what"];
    return Error(({
        "a powerful force settles between", who, "and", mount, "preventing", ({ 'o', who }),
        "from mounting", ({ 'o', mount }),
    }));
    return False;
}

status traveler_obstacle_flight_proscription_can_move(mapping args) {
    object where = args["to"];
    if(!where || !where->is_character())
        return True;
    object who = args["who"];
    return Error(({
        "a powerful force settles between", who, "and", where, "preventing", ({ 'o', where }),
        "from carrying", ({ 'o', who }),
    }));
}

void traveler_obstacle_flight_proscription_at_limb_added(mapping args) {
    object who = args["who"];
    traveler_obstacle_flight_nullify_wings(who);
}

void configure() {
    ::configure();
    set_obstacle_name("flight proscription");
    set_obstacle_prepend_indefinite_article(True);
    set_obstacle_rarity(Rarity_Very_Unusual);
    set_obstacle_value(Travelers_Obstacle_Value_High);
    set_challenge_component_selection_adjustments(([
        "experience"                            : 0.00,
        "exploration"                           : 2.50,
        "pilgrimage"                            : 1.50,
        "quest"                                 : 0.50,
        "skill experience"                      : 0.00,
        "charity"                               : 1.00,
        "neophyte guidance"                     : 1.00,
    ]));
    set_obstacle_initialize_display(([
        Message_Content                         : ({
            0, ({ "sense", 0 }), "a powerful force proscribing", ({ 'o', 0 }), "from flying and prescribing that", ({ 'o', 0 }),
            "travel the world by", ({ 'r', 0, "own effort" }),
        }),
        Message_Senses                          : Message_Sense_Spiritual | Message_Sense_Cognitive,
        Message_Color                           : "status: loss",
    ]));
    add_obstacle_trait_modifier(([
        Modifier_Index                          : ({
            Trait_Wingless_Flight,
        }),
        Modifier_Amount                         : -5000,
    ]));
    add_obstacle_hook(Can_Have_Limb_Restored, #'traveler_obstacle_flight_proscription_can_have_limb_restored);
    add_obstacle_hook(At_Limb_Added, #'traveler_obstacle_flight_proscription_at_limb_added);
    add_obstacle_hook(Can_Move, #'traveler_obstacle_flight_proscription_can_move);
    add_obstacle_hook(Can_Mount, #'traveler_obstacle_flight_proscription_can_mount);
    set_obstacle_attach_process((:
        object who = $1->ganesha_challenge_query_owner();
        traveler_obstacle_flight_nullify_wings(who);
        traveler_obstacle_flight_proscription_reject_mounts_and_carriers(who);
        return;
    :));
    set_obstacle_overcome_process((:
        object who = $1->ganesha_challenge_query_owner();
        obstacle_disengage_hooks(who);
        traveler_obstacle_flight_proscription_restore_wings(who);
    :));
    set_obstacle_overcome_display(([
        Message_Content                         : ({
            0, ({ "sense", 0 }), "the proscription against flying being lifted from", ({ 'o', 0 }),
        }),
        Message_Senses                          : Message_Sense_Spiritual | Message_Sense_Cognitive,
        Message_Color                           : "status: enhancement",
    ]));
}
