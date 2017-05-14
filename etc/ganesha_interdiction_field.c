#include <Travelers.h>
#include <item.h>

inherit "/std/item";

nosave private object array hooked = ({});
nosave private mapping mods = ([]);
private string array realms;

void ganesha_interdiction_field_do_move(mapping args);

status is_ganesha_interdiction_field() {
    return True;
}

void ganesha_interdiction_field_set_realms(string array sa) {
    realms = sa;
}

string array ganesha_interdiction_field_query_realms() {
    return realms || ({});
}

void ganesha_interdiction_field_unapply_effects() {
    foreach(object obj, descriptor mod : mods)
        if(obj)
            obj->remove_realm_modifier(mod);
    mods = ([]);
    foreach(object obj : hooked)
        if(obj)
            obj->remove_hook(Do_Move, #'ganesha_interdiction_field_do_move);
    hooked = ({});
}

void ganesha_interdiction_field_apply_effects() {
    foreach(object obj : all_environment()) {
        descriptor mod = obj->add_realm_modifier(([
            Modifier_Index  : ganesha_interdiction_field_query_realms(),
            Modifier_Amount : 10,
        ]));
        if(mod)
            mods[obj] = mod;
    }
    foreach(object obj : all_environment())
        if(obj->add_hook(Do_Move, #'ganesha_interdiction_field_do_move))
            hooked += ({ obj });
}

void ganesha_interdiction_field_do_move(mapping args) {
    ganesha_interdiction_field_unapply_effects();
    ganesha_interdiction_field_apply_effects();
}

status ganesha_interdiction_field_initialize(object who) {
    status res = (move(who) == Move_Succeed);
    object array lib_fields = filter(all_inventory(who), (: $1->id("GANESHA_LIBERATION_FIELD") :));
    if(res) {
        lib_fields->remove();
        who->add_unbound_armour(this_object());
        who->message(([
            Message_Content     : ({
                ({ 'a', this_object() }), "settles around", 0
            }),
            Message_Senses      : Message_Sense_Olfactory,
        ]));
    }
}

void ganesha_interdiction_field_expire() {
    object who = environment() && environment()->is_character() && environment();
    if(who)
        who->message(([
            Message_Content     : ({
                ({ 't', this_object() }), "lingering around", 0, "passes",
            }),
            Message_Senses      : Message_Sense_Olfactory,
        ]));
    remove();
}

varargs void remove(int flags) {
    ganesha_interdiction_field_unapply_effects();
    ::remove(flags);
}

mixed extra_look(object who) {
    return ({
        Message(([
            Message_Content : ({
                ({ 'a', this_object() }), "whispers through the",
                Description(Description_Type_Ambient_Medium_Colored_Name),
                "near", ({ 'o', environment() }),
            }),
            Message_Senses  : Message_Sense_Olfactory,
        ])),
    });
}


void configure() {
    ::configure();
    alter_identity(Identity_Type, Identity_Type_Custom);
    alter_identity(Identity_Adjunct_Nouns, ({ "scent" }));
    alter_identity(Identity_Nouns, ({ "wildflowers", "flowers" }));
    alter_identity(Identity_Special_Names, ({ "GANESHA_INTERDICTION_FIELD" }), True);
    set_short(0);
    armour()->set_armour_type(Armour_Type_Aura);
    armour()->set_race("human");
    armour()->set_ablative(False);
    set_indestructible(True);
    add_proportion(([
        Element_Type        : Material_Air,
        Element_Proportion  : 1.000,
    ]));
    add_proportion(([
        Element_Type        : Material_Mana,
        Element_Proportion  : 0.150,
        Element_Flags       : Element_Flag_Secondary,
    ]));
    add_proportion(([
        Element_Type        : Material_Shekhinah,
        Element_Proportion  : 0.001,
        Element_Flags       : Element_Flag_Secondary,
    ]));
    add_hook(Do_Move, #'ganesha_interdiction_field_do_move);
    set_suppress_condition_display(True);
    set_suppress_size_display(True);
    set_suppress_weight_display(True);
    set_value(0);
}

status query_auto_keep(object obj) {
    return True;
}

status drop() {
    return True;
}

status get() {
    return True;
}
