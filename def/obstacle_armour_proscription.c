#include <Travelers.h>
#include <daemon.h>
#include <usage.h>

inherit Travelers_Definition("Obstacle");

status traveler_obstacle_armour_proscription_is_proscribed(object obj) {
    if(obj->armour()->query_absorption() + obj->armour()->query_encumbrance() >= 13.5)
        return True;
    if(obj->query_property(Prop_Magical))
        return True;
    return False;
}

status traveler_obstacle_armour_proscription_can_equip_item(mapping args) {
    if(args["usage"] == Usage_Wear && traveler_obstacle_armour_proscription_is_proscribed(args["what"]))
        return Message(([
            Message_Content     : ({
                "a strange force settles between", 0, "and", args["what"], ", preventing", ({ 'o', 0 }),
                "from wearing", ({ 'o', args["what"] }),
            }),
            Message_Senses      : Message_Sense_Kinesthetic,
        ]));
    return True;
}

void configure() {
    ::configure();
    set_obstacle_name("armour proscription");
    set_obstacle_prepend_indefinite_article(True);
    set_obstacle_rarity(Rarity_Very_Unusual);
    set_obstacle_value(Travelers_Obstacle_Value_Moderate);
    set_challenge_component_selection_adjustments(([
        "experience"            : 1.00,
        "exploration"           : 1.00,
        "pilgrimage"            : 1.00,
        "quest"                 : 1.00,
        "skill experience"      : 0.00,
        "charity"               : 0.00,
        "neophyte guidance"     : 0.25,
    ]));
    set_obstacle_initialize_display(([
        Message_Content         : ({
            0, ({ "sense", 0 }), "a powerful force proscribing", ({ 'o', 0 }), "from using serious armour of any sort",
        }),
        Message_Color           : "status: loss",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
    add_obstacle_hook(Can_Equip_Item, #'traveler_obstacle_armour_proscription_can_equip_item);
    set_obstacle_attach_process((:
        object who = $1->ganesha_challenge_query_owner();
        object array armours = filter(who->query_wearing(), (: traveler_obstacle_armour_proscription_is_proscribed($1) :));
        if(sizeof(armours)) {
            who->display(([
                Message_Content     : ({
                    "a strange force settles between", 0, "and", ({ 'j', armours }), ", preventing", ({ 'o', 0 }),
                    "from wearing", ({ 'o', armours }),
                }),
                Message_Senses      : Message_Sense_Kinesthetic,
            ]));
            foreach(object armour : armours)
                who->deutilize_item(armour, Move_Flags_Force);
        }
        return;
    :));
    set_obstacle_overcome_display(([
        Message_Content         : ({
            0, ({ "sense", 0 }), "the proscription against using armour being lifted from", ({ 'o', 0 }),
        }),
        Message_Color           : "status: enhancement",
        Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
}
