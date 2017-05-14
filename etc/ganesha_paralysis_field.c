#include <Travelers.h>
#include <item.h>

inherit "/std/app/personal_enchantment";

nosave private descriptor paralysis_mod;

status is_ganesha_paralysis_field() {
    return True;
}

varargs void ganesha_paralysis_paralyze(object who) {
    who ||= personal_enchantment_query_owner();
    if(!who->query_paralyzed()) {
        who->message(([
            Message_Content                             : ({
                ({ 't', this_object() }), "around", 0, ({ "quicken", this_object() }),
            }),
            Message_Senses                              : Message_Sense_Auditory,
        ]));
        paralysis_mod = who->add_paralysis_modifier(([
            Modifier_Amount                             : 1,
            Modifier_Add_Display                        : ([
                Message_Content                         : ({
                    0, ({ "are", 0 }), "somehow paralyzed by", ({ 't', this_object() }),
                }),
                Message_Senses                          : Message_Sense_Kinesthetic,
                Message_Color                           : "alert: high",
            ]),
            Modifier_Remove_Display                     : ([
                Message_Content                         : ({
                    0, ({ "recover", 0 }), "control of", ({ 'l', 0 }),
                }),
                Message_Senses                          : Message_Sense_Kinesthetic,
                Message_Color                           : "status: recovery",
            ]),
            Modifier_Damage_Type                        : ({ "divine" }),
            Modifier_Flags                              : Modifier_Flag_Resist_Duration,
            Modifier_Duration                           : random_range(1, 4) * 2,
        ]));
    }
}

void ganesha_paralysis_field_at_start_combat(object who) {
    ganesha_paralysis_paralyze(who);
}

void configure() {
    ::configure();
    alter_identity(Identity_Type, Identity_Type_Custom);
    alter_identity(Identity_Adjectives, ({ "faint" }));
    alter_identity(Identity_Nouns, ({ "breeze" }));
    alter_identity(Identity_Special_Names, ({ "GANESHA_PARALYSIS_FIELD" }), Identity_Alter_Add);
    set_short(0);
    personal_enchantment_set_mod_description(([
        Description_Type                                : Description_Type_Viewer_Condition,
        Description_Index                               : Condition_Type_Hearing,
        Description_Content                             : ({
            ({ 'a', "%t" }), ({ "whisper", "%t" }), "in the", Description(Description_Type_Ambient_Medium_Colored_Name),
            "around", ({ 'o', "%o" }),
        }),
    ]));
    personal_enchantment_set_add_message(([
        Message_Content                                 : ({
            ({ 'a', "%t" }), ({ "begin", "%t" }), "to whisper in the", Description(Description_Type_Ambient_Medium_Colored_Name),
            "around", "%o",
        }),
        Message_Senses                                  : Message_Sense_Auditory,
    ]));
    personal_enchantment_set_remove_message(([
        Message_Content                                 : ({
            ({ 't', "%t" }), "whispering around", "%o", ({ "pass", "%t" }),
        }),
        Message_Senses                                  : Message_Sense_Auditory,
    ]));
    personal_enchantment_set_hooks(({
        ({
            At_Start_Combat,
            #'ganesha_paralysis_field_at_start_combat,
        }),
    }));
    armour()->set_armour_type(Armour_Type_Aura);
    armour()->set_race("human");
    armour()->set_ablative(False);
    set_indestructible(True);
    add_proportion(([
        Element_Type                                    : Material_Air,
        Element_Proportion                              : 1.000,
    ]));
    add_proportion(([
        Element_Type                                    : Material_Mana,
        Element_Proportion                              : 0.150,
        Element_Flags                                   : Element_Flag_Secondary,
    ]));
    add_proportion(([
        Element_Type                                    : Material_Shekhinah,
        Element_Proportion                              : 0.001,
        Element_Flags                                   : Element_Flag_Secondary,
    ]));
    add_hook(At_Start_Combat, #'ganesha_paralysis_field_at_start_combat);
    set_suppress_condition_display(True);
    set_suppress_size_display(True);
    set_suppress_weight_display(True);
    set_value(0);
}
