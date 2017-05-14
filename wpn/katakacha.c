#include <Travelers.h>
#include <configuration.h>
#include <damage_types.h>
#include <interval.h>
#include <item.h>
#include <locations.h>
#include <metrics.h>
#include <realtime.h>
#include <transmute.h>

inherit "/std/item";

private string array enhancement;

descriptor katakacha_query_enhancement_identity();
string katakacha_clear_enhancement();
void katakacha_periodic_empathic_response();

// katakacha control metrics

varargs float katakacha_query_owner_spiritual_communion_factor(object who) {
    who ||= query_user();
    unless(who)
        return 0.0;
    int communion = Metric_Retrieve("spiritual communion with Ganesha", who);
    return scale_conversion(to_float(communion), 50.0, 250.0, 0.1, 1.0);
}

varargs float katakacha_query_owner_kapilaya_invocation_factor(object who) {
    who ||= query_user();
    unless(who)
        return 0.0;
    int kapilaya_sonority = Travelers_Invocation("kapilaya")->query_invocation_sonority(who);
    return scale_conversion(to_float(kapilaya_sonority), 0.0, 100.0, 0.1, 1.0);
}

varargs float katakacha_query_owner_gajakarnikaya_invocation_factor(object who) {
    who ||= query_user();
    unless(who)
        return 0.0;
    int gajakarnikaya_sonority = Travelers_Invocation("gajakarnikaya")->query_invocation_sonority(who);
    return scale_conversion(to_float(gajakarnikaya_sonority), 0.0, 100.0, 0.1, 1.0);
}

varargs float katakacha_query_owner_abjuration_factor(object who) {
    who ||= query_user();
    unless(who)
        return 0.0;
    float rating = Condition_Evaluate(([
        Condition_Skill_Composite               : True,
        Skill_Abjuration                        : 1.0,
        ([
            Condition_Type_Code                 : Condition_Type_Attribute,
            Condition_Info                      : Attr_Ego,
            Condition_Value                     : 1.0,
        ])                                      : True,
    ]), who, 0);
    return scale_conversion(rating, Travelers_Invocation_Ability_Minimum_Low, Travelers_Invocation_Ability_Maximum_High, 0.1, 1.0);
}

varargs float katakacha_query_owner_s_communion_with_ganesha(object who) {
    who ||= query_user();
    return 100 * katakacha_query_owner_spiritual_communion_factor(who);
}

varargs float katakacha_query_owner_s_communion_with_kacha_form(object who) {
    who ||= query_user();
    return 100 * katakacha_query_owner_spiritual_communion_factor(who) * katakacha_query_owner_kapilaya_invocation_factor(who);
}

varargs float katakacha_query_owner_s_communion_with_kacha_matter(object who) {
    who ||= query_user();
    return 100 * katakacha_query_owner_spiritual_communion_factor(who) * katakacha_query_owner_gajakarnikaya_invocation_factor(who);
}

// katakacha weapon type, element proportion, and craft configuration

void katakacha_configure(object who) {
    float level = katakacha_query_owner_s_communion_with_kacha_form(who);
    descriptor kacha = find_element(Material_Kacha);
    unless(kacha)
        error("katakacha has no kacha element");
    descriptor quartz = find_element(Material_Quartz);
    unless(quartz)
        error("katakacha has no quartz element");
    set_will_update_configuration(True);
    switch(round(level)) {
    case 41..75                                 :
        set_craft(Craft_Excellent);
        break;
    case 76..99                                 :
        set_craft(Craft_Exquisite);
        break;
    case 100                                    :
        set_craft(Craft_Unearthly);
        break;
    default                                     :
        set_craft(Craft_Good);
        break;
    }
    alter_element(&kacha, Element_Flags, Element_Flag_Untransmutable, False);
    alter_element(&quartz, Element_Flags, Element_Flag_Untransmutable, False);
    float volume = Element_Query(kacha, Element_Volume) + Element_Query(quartz, Element_Volume);
    float amount = max(volume * level / 100.0, 0.001);
    if(amount > Element_Query(kacha, Element_Volume)) {
        amount = min(amount - Element_Query(kacha, Element_Volume), Element_Query(quartz, Element_Volume) * 0.99);
        if(amount > 0.0)
            kacha = transmute_element(&quartz, ([
                Element_Type                    : Material_Kacha,
                Element_Volume                  : amount,
                Element_Flags                   : Element_Query(kacha, Element_Flags),
            ]), Transmute_Preserve_Color);
    } else if(amount < Element_Query(kacha, Element_Volume)) {
        amount = Element_Query(kacha, Element_Volume) - amount;
        if(amount > 0.0)
            quartz = transmute_element(&kacha, ([
                Element_Type                    : Material_Quartz,
                Element_Volume                  : amount,
                Element_Flags                   : Element_Query(quartz, Element_Flags),
            ]), Transmute_Preserve_Color);
    }
    alter_element(&kacha, Element_Flags, Element_Flag_Untransmutable, True);
    alter_element(&quartz, Element_Flags, Element_Flag_Untransmutable | Element_Flag_Secondary, True);
    update_configuration(True);
}

varargs void katakacha_set_weapon_type(object who, mixed type) {
    type ||= Weapon_Type_Shard;
    type = Weapon_Type(type)->query_weapon_type_code();
    if(type == weapon()->query_weapon_type())
        return;
    katakacha_clear_enhancement();
    user_message(([
        Message_Content                         : ({
            0, ({ "separate", 0 }), "into hundreds of tiny blocks of crystal, which rapidly shift and reassemble",
            "themselves into the shape of", ({ 'a', Weapon_Type(type)->query_weapon_type_identity() }),
        }),
        Message_Senses                          : Message_Sense_Visual,
    ]), who);
    set_will_update_configuration(True);
    set_shape(0);
    proportionalize_elements();
    weapon()->set_weapon_type(type);
    weapon()->set_damage_types(weapon()->type()->query_weapon_type_damage_types());
    update_configuration(True);
    katakacha_configure(who);
}

// katakacha combat effects.  some of these will only trigger once you get the bestowals that enable them

void katakacha_abjure_shield(object who, float force, object target, object shield) {
    object env = environment(target);
    float rating;
    if(!target->query_paralyzed() && !target->query_dying())
        rating = target->query_effective_skill(Skill_Massive_Exertion) + target->query_average_effective_skill(shield->weapon()->query_weapon_skills());
    rating = to_float(rating);
    rating = scale_conversion(rating, 0.0, 1500.0, 0.0, 100.0);
    if(target->query_stunned())
        rating = rating * 3 / 4;
    descriptor identity = copy(katakacha_query_enhancement_identity());
    Identity_Set(identity, Identity_Adjunct_Nouns, ({ "scintilla" }));
    user_message(([
        Message_Content                         : ({
            ({ 'a', identity }), "darts from", 0, "into", ({ 's', target, shield }),
        }),
        Message_Senses                          : Message_Sense_Astral | Message_Sense_Visual,
    ]), who);
    if(semirandom(rating) > semirandom(force)) {
        target->message(([
            Message_Content                     : ({
                ({ 's', 0, shield }), ({ "quiver", shield }), "and", ({ "jerk", shield }), "as if to escape from",
                ({ 'o', 0 }), ", but", ({ 'p', 0 }), ({ "hold", 0 }), "on to", ({ 'o', shield }),
            }),
            Message_Senses                      : Message_Sense_Visual | Message_Sense_Kinesthetic_For_Participants,
        ]));
    } else {
        if(force > random(300) && !query_any_realm("NT", shield)) {
            target->message(([
                Message_Content                 : ({
                    ({ 's', 0, shield }), ({ "quiver", shield }), "and", ({ "jerk", shield }), "as if to escape from", ({ 'o', 0 }),
                    ", going flying through the", Description(Description_Type_Ambient_Medium_Colored_Name),
                    "for a moment, then", ({ "disappear", shield }),
                    "in", 'a', self_color("dazzling " + random_element(Travelers_Rainbow), "ripple of", "color"),
                }),
                Message_Senses                  : Message_Sense_Visual | Message_Sense_Kinesthetic_For_Participants,
            ]));
            if(shield->move(env) == Move_Succeed) {
                object where;
                do {
                    where = Random_Location_Any;
                } while(where == env || query_any_realm("NT", where));
                if(shield->move(where) == Move_Succeed)
                    shield->message(([
                        Message_Content         : ({
                            ({ 'a', 0 }), ({ "appear", 0 }),
                            "in", 'a', self_color("dazzling " + random_element(Travelers_Rainbow), "ripple of", "color"),
                        }),
                        Message_Senses          : Message_Sense_Visual | Message_Sense_Kinesthetic_For_Participants,
                    ]));
                else
                    shield->message(([
                        Message_Content         : ({
                            ({ 't', 0 }), ({ "reappear", 0 }),
                            "in", 'a', self_color("dazzling " + random_element(Travelers_Rainbow), "ripple of", "color"),
                        }),
                        Message_Senses          : Message_Sense_Visual | Message_Sense_Kinesthetic_For_Participants,
                    ]));
            } else {
                target->message(([
                    Message_Content             : ({
                        "mysteriously, however,", ({ 't', shield }), "instantly returns to", ({ 's', 0, "grasp" }),
                    }),
                    Message_Senses              : Message_Sense_Visual | Message_Sense_Kinesthetic_For_Participants,
                ]));
            }
        } else {
            target->message(([
                Message_Content                 : ({
                    ({ 's', 0, shield }), ({ "quiver", shield }), "and", ({ "jerk", shield }), "away from", ({ 'o', 0 }),
                    ", going flying through the", Description(Description_Type_Ambient_Medium_Colored_Name),
                }),
                Message_Senses                  : Message_Sense_Visual | Message_Sense_Kinesthetic_For_Participants,
            ]));
            if(!shield->move(env) == Move_Succeed) {
                target->message(([
                    Message_Content             : ({
                        "mysteriously, however,", ({ 't', shield }), "instantly returns to", ({ 's', 0, "grasp" }),
                    }),
                    Message_Senses              : Message_Sense_Visual | Message_Sense_Kinesthetic_For_Participants,
                ]));
            }
        }
    }
}

void katakacha_abjure_protective_manifestation(object who, float force, object target, object manifestation) {
    float rating = 5.0;
    rating += manifestation->query_property_proportion(Prop_Sturdy) * 50;
    rating += manifestation->query_property_proportion(Prop_Primal) * 1000;
    rating += manifestation->query_dispel_resistance();
    descriptor identity = copy(katakacha_query_enhancement_identity());
    Identity_Set(identity, Identity_Adjunct_Nouns, ({ "flicker" }));
    user_message(([
        Message_Content                         : ({
            ({ 'a', identity }), "zigzags from", 0, "into", ({ 's', target, manifestation }),
        }),
        Message_Senses                          : Message_Sense_Astral | Message_Sense_Visual,
    ]), who);
    if(semirandom(rating) > semirandom(force)) {
        target->message(([
            Message_Content                     : ({
                ({ 's', 0, manifestation }), ({ "ripple", manifestation }), "and then", ({ "restabilize", manifestation }),
            }),
            Message_Senses                      : Message_Sense_Visual | Message_Sense_Astral,
        ]));
    } else {
        string flower_color = random_element(({
            "paisley",
            "iridescent",
            "pink",
            "light blue",
            "prismatic",
            "milky",
        }));
        flower_color += "-";
        flower_color += random_element(({
            "mottled",
            "flecked",
            "spotted",
            "banded",
            "striped",
            "speckled",
            "streaked",
            "tinted",
        }));
        flower_color += " ";
        flower_color += random_element(Travelers_Rainbow);
        target->message(([
            Message_Content                     : ({
                ({ 's', 0, manifestation }), ({ "explode", manifestation }), "into a",
                "{{" + flower_color + "}swirling cloud of colorful flower petals}"
            }),
            Message_Senses                      : Message_Sense_Visual | Message_Sense_Astral,
        ]));
        manifestation->remove();
    }
}

void katakacha_overwhelm_mortal_wound(object who, object target) {
    descriptor identity = copy(katakacha_query_enhancement_identity());
    Identity_Set(identity, Identity_Adjectives, ({ "motes of" }));
    Identity_Set(identity, Identity_Adjunct_Nouns, ({ "panoply" }));
    user_message(([
        Message_Content                         : ({
            ({ 'a', identity }), "spirals off the surface of", 0, "and surges into", ({ 's', target, "broken body" }),
        }),
        Message_Senses                          : Message_Sense_Visual | Message_Sense_Astral,
    ]), who);
    who->add_spell_points(-10.0);
    target->do_damage(([
        Attack_Damage_Type                      : enhancement,
        Attack_Damage                           : semirandom(5000),
        Attack_Limb                             : Null,
        Attack_Aggressor                        : who,
        Attack_Flags                            : Attack_Flag_Bypass_Armour | Attack_Flag_Display_Damage_Message | Attack_Flag_Untreatable | Attack_Flag_Limb_Preset | Attack_Flag_Suppress_Attacker_Value,
    ]));
    if(!target->query_dying())
        target->die(who);
}

private status katakacha_check_mortal_wound(object who, object target) {
    if(target->query_mortal_wound() && random(100) > target->query_resistance(enhancement)) {
        if(who->check_spell_points(10.0)) {
            katakacha_overwhelm_mortal_wound(who, target);
            return True;
        }
    }
    return False;
}

private status katakacha_check_armour_abjuration(object who, object target, float effective_communion, object obj) {
    if(obj->query_property(Prop_Manifestation) && !obj->query_indestructible() && !obj->is_curse()) {
        if(!who->check_spell_points(20.0))
            return True;
        katakacha_abjure_protective_manifestation(who, effective_communion, target, obj);
        who->add_skill_exp(Skill_Abjuration, Learn_Rare * 2, 0, 0, 0, Skill_Change_Flag_Inobvious);
        return True;
    }
    return False;
}

private status katakacha_check_target_armour_abjuration(object who, object target, float effective_communion) {
    foreach(object obj : target->query_armour_worn())
        if(katakacha_check_armour_abjuration(who, target, effective_communion, obj))
            return True;
    foreach(object obj : target->query_unbound_armour())
        if(katakacha_check_armour_abjuration(who, target, effective_communion, obj))
            return True;
    return False;
}

private status katakacha_check_shield_abjuration(object who, object target, float effective_communion, object env, object obj) {
    if(obj->check_move(env) != Move_Succeed)
        return False;
    if(!who->check_spell_points(10.0))
        return True;
    katakacha_abjure_shield(who, effective_communion, target, obj);
    who->add_skill_exp(Skill_Abjuration, Learn_Rare, 0, 0, 0, Skill_Change_Flag_Inobvious);
    return True;
}

private status katakacha_check_target_shield_abjuration(object who, object target, float effective_communion) {
    object env = environment(target);
    foreach(object obj : target->query_wielded_weapons())
        if(obj->is_weapon() && obj->weapon()->type()->query_weapon_type_shield())
            if(katakacha_check_shield_abjuration(who, target, effective_communion, env, obj))
                return True;
    return False;
}

void katakacha_end_of_round_effects(object who) {
    unless(enhancement)
        return;
    if(!who || who != query_user())
        return;
    if(query_any_realm("NM", who))
        return;
    who->clean_up_attackers();
    object att = who->query_attacker();
    if(!att)
        return;
    object array sec_atts = who->query_secondary_attackers();
    unless(who->combat_mode()->query_combat_mode_nonhostile()) {
        if(katakacha_check_mortal_wound(who, att))
            return;
        foreach(object sec_att : sec_atts)
            if(katakacha_check_mortal_wound(who, sec_att))
                return;
    }
    if(!random(2) && weapon()->query_discharges() && !att->query_dying()) {
        weapon()->discharge();
        return;
    }
    object link = who->query_affiliation_link(project_control());
    if(!link)
        return;
    if(!link->travelers_query_bestowals("katakacha abjuration"))
        return;
    float effective_communion = katakacha_query_owner_s_communion_with_ganesha(who) * katakacha_query_owner_abjuration_factor(who);
    if(effective_communion > random(200)) {
        if(katakacha_check_target_shield_abjuration(who, att, effective_communion))
            return;
        foreach(object sec_att : sec_atts)
            if(katakacha_check_target_shield_abjuration(who, sec_att, effective_communion))
                return;
    }
    if(effective_communion > random(400)) {
        if(katakacha_check_target_armour_abjuration(who, att, effective_communion))
            return;
        foreach(object sec_att : sec_atts)
            if(katakacha_check_target_armour_abjuration(who, sec_att, effective_communion))
                return;
    }
}

void katakacha_user_do_fight(mapping args) {
    call_out("katakacha_end_of_round_effects", 1, args["who"]);
}

// katakacha damage type enhancements and discharges

mixed array katakacha_query_enhancement_effect() {
    return enhancement && Damage_Type_Effect(enhancement);
}

descriptor katakacha_query_enhancement_identity() {
    mixed array effect = katakacha_query_enhancement_effect();
    unless(effect)
        return 0;
    descriptor vector_identity = copy(Vector(Vector_Mote)->query_vector_identity());
    descriptor out = Identity_Merge(copy(effect[Damage_Effect_Identity]), Identity_Pluralize(vector_identity), Knowledge_Unknown, 0, 0);
    return out;
}

varargs string katakacha_clear_enhancement(status suppress_message) {
    if(Interval_Query(#'katakacha_clear_enhancement) != Null)
        Interval_Remove(#'katakacha_clear_enhancement);
    weapon()->set_discharges(False);
    weapon()->set_damage_types(weapon()->type()->query_weapon_type_damage_types());
    while(remove_call_out("katakacha_clear_enhancement") != Null)
        ;
    string out;
    if(enhancement) {
        if(suppress_message)
            out = Identity_Render_Name(katakacha_query_enhancement_identity(), 0);
        else
            user_message(([
                Message_Content                 : ({
                    ({ 't', katakacha_query_enhancement_identity() }), "flickering across the surface of", 0, "fade away",
                }),
                Message_Senses                  : Message_Sense_Visual | Message_Sense_Astral,
            ]));
        enhancement = 0;
    }
    return out;
}

void katakacha_set_enhancement(object who, string array types) {
    who ||= query_user();
    unless(who)
        return;
    float rating = katakacha_query_owner_s_communion_with_kacha_matter(who);
    string existing_motes;
    if(enhancement)
        existing_motes = katakacha_clear_enhancement(True);
    weapon()->set_damage_types(types + weapon()->type()->query_weapon_type_damage_types());
    weapon()->set_discharges(True);
    weapon()->set_discharge_damage_types(types);
    weapon()->set_discharge_potency(rating * 0.75);
    weapon()->set_discharge_spell_point_cost(rating * 0.10);
    weapon()->set_discharge_endurance_cost(rating * 0.05);
    weapon()->set_discharge_strike(rating * 0.10);
    enhancement = types;
    mixed array message = ({});
    descriptor new_motes = katakacha_query_enhancement_identity();
    if(existing_motes && existing_motes != new_motes)
        message += ({
            ({ 't', existing_motes }), "flickering across the surface of", 0,
            "are interspersed with and then replaced by", ({ 'n', new_motes }),
        });
    else if(existing_motes)
        message += ({
            ({ 't', existing_motes }), "flickering across the surface of", 0,
            "momentarily spark excitedly before relaxing and resuming their hypnotic dance"
        });
    else
        message += ({
            ({ 'n', new_motes }), ({ "begin", new_motes }), "to flicker across the surface of", 0,
        });
    user_message(([
        Message_Content                         : message,
        Message_Senses                          : Message_Sense_Visual | Message_Sense_Astral,
    ]), who);
    int duration = scale_conversion(round(rating), 10, 100, Time_Minute * 15, Time_Hour * 2);
    Interval_Set(#'katakacha_clear_enhancement, duration);
}

void katakacha_periodic_empathic_response() {
    object who = query_user() || (environment() && environment()->is_character() && environment());
    float rating = katakacha_query_owner_s_communion_with_ganesha(who);
    unless(rating > 15.0)
        return;
    unless(who->sentience()->query_sentience_emotion())
        return;
    mixed array message;
    string color;
    string array feelings;
    switch(random(4)) {
        case    0                               :
            message = ({
                0, ({ "feel", 0 }), "a wave of peace and harmony wash over", who
            });
            color = "cerulean";
            feelings = ({ "peaceful", "harmonious" });
            who->add_spell_points(fuzz(rating * 2));
            break;
        case 1                                  :
            message = ({
                0, ({ "feel", 0 }), "a wave of relaxation and refreshment wash over", ({ 'o', 0 }),
            });
            color = "pale green";
            feelings = ({ "energized", "refreshed" });
            who->add_endurance(fuzz(rating * 4));
            break;
        case 2                                  :
            message = ({
                0, ({ "feel", 0 }), "more connected with", ({ 'r', 0, "dharma" }),
            });
            color = "deep blue";
            feelings = ({ "connected", "whole" });
            who->add_trait_modifier(([
                Modifier_Index                  : Trait_Order_Favour,
                Modifier_Amount                 : semirandom(round(rating / 15)),
                Modifier_Bound                  : 10,
                Modifier_Flags                  : Modifier_Flag_Mergeable,
                Modifier_Duration               : semirandom(Time_Minute * 20),
                Modifier_Max_Duration           : Time_Hour * 2,
                Modifier_Remove_Display         : ([
                    Message_Content             : ({
                        who, ({ "feel", who }), "less connected with", ({ 'r', who, "dharma" })
                    }),
                    Message_Senses              : Message_Sense_Emotive,
                    Message_Color               : "status: loss",
                ]),
            ]));
            break;
        case 3                                  :
            message = ({
                0, ({ "feel", 0 }), ({ 'r', 0, "body" }), "and", this_object(), "synchronize"
            });
            color = "reddish-purple";
            feelings = ({ "fluid", "graceful" });
            int duration = semirandom(Time_Minute * 7);
            float amount = rating / 20.0;
            weapon()->add_weapon_feature_modifier(([
                Modifier_Index                  : Weapon_Feature_Strike,
                Modifier_Amount                 : amount,
                Modifier_Duration               : duration,
            ]));
            weapon()->add_weapon_feature_modifier(([
                Modifier_Index                  : Weapon_Feature_Defend,
                Modifier_Amount                 : amount,
                Modifier_Duration               : duration,
            ]));
            weapon()->add_weapon_feature_modifier(([
                Modifier_Index                  : Weapon_Feature_Speed,
                Modifier_Amount                 : amount,
                Modifier_Duration               : duration,
            ]));
            who->add_skill_modifier(([
                Modifier_Index                  : Skill_Combat_Reflexes,
                Modifier_Amount                 : round(rating),
                Modifier_Duration               : duration,
                Modifier_Remove_Display         : ([
                    Message_Content             : ({
                        who, ({ "feel", who }), ({ 'r', who, "body" }), "fall out of sync with", this_object(),
                    }),
                    Message_Color               : "status: loss",
                    Message_Senses              : Message_Sense_Emotive | Message_Sense_Kinesthetic,
                ]),
            ]));
            break;
    }
    user_message(([
        Message_Content                         : ({
            0, ({ "pulse", 0 }), "rhythmically with", self_color(color, 0, "scintillae"),
        }),
        Message_Senses                          : Message_Sense_Visual | Message_Sense_Astral,
    ]), who);
    who->display(([
        Message_Content                         : message,
        Message_Senses                          : Message_Sense_Emotive,
        Message_Color                           : color,
    ]));
    foreach(string feeling : feelings)
        who->mentation(feeling, Thought_Type_Feel, True);
}

// hooks and stuffs for the config()

void katakacha_do_equip(mapping args) {
    object who = args["who"];
    who->add_hook(Do_Fight, #'katakacha_user_do_fight);
    Interval_Set(#'katakacha_periodic_empathic_response, semirandom(Time_Minute * 15));
}

void katakacha_do_unequip(mapping args) {
    object who = args["who"];
    who->remove_hook(Do_Fight, #'katakacha_user_do_fight);
    Interval_Remove(#'katakacha_periodic_empathic_response);
}

string katakacha_query_mote_description() {
    if(enhancement)
        return Identity_Render_Name(katakacha_query_enhancement_identity(), 0);
    else
        return "{{iridescent}iridescent motes of energy}";
}

void configure() {
    ::configure();
    set_creator("den");
    set_maintainer("twilight");
    alter_identity(([
        Identity_Code                           : "katakacha",
        Identity_Known_Nouns                    : ({ "katakacha" }),
        Identity_Flags                          : Identity_Flag_Suppress_Name_Code,
    ]));
    alter_identity(Identity_Special_Names, ({ "KATAKACHA" }), Identity_Alter_Add);
    set_craft(Craft_Good);
    set_indestructible(True);
    weapon()->set_weapon_type(Weapon_Type_Shard);
    weapon()->set_ablative(False);
    weapon()->set_discharge_skills(([
        Skill_Wyrding                           : 1.0,
    ]));
    weapon()->set_discharge_vector(Vector_Mote);
    weapon()->set_discharges(False);
    add_description(({
        #'katakacha_query_mote_description, "flicker across the surface of this", Description(Description_Type_Craftsmanship_Adjective),
        Description(([
            Description_Type                    : Description_Type_Element_Name,
            Description_Content                 : Description(Description_Type_Weapon_Type_Name),
            Description_Flags                   : Description_Flag_Ignore_Element_Suppression_Flags,
        ])),
        "with the rhythm of a heartbeat and a subtle vibration resonates along", ({ 'r', 0, "length" }),
    }));
    add_known_description(({
        "this", ({ "are", 0 }), 'a', "crystalline lifeform known as a katakacha",
    }));
    add_known_description(({
        "though sessile,", ({ 'p', 0 }), ({ "possess", 0 }), "sentience and the "
        "ability to transform itself into a variety of shapes",
    }));
    add_known_description(([
        Description_Type                        : Description_Type_Viewer_Condition,
        Description_Index                       : ([
            Condition_Type_Code                 : Condition_Type_Total,
            Condition_Value                     : 200,
            Condition_Info                      : ({
                ([
                    Condition_Type_Code         : Condition_Type_Skill,
                    Condition_Info              : Skill_Theology,
                ]),
                ([
                    Condition_Type_Code         : Condition_Type_Skill,
                    Condition_Info              : Skill_History,
                ]),
                ([
                    Condition_Type_Code         : Condition_Type_Skill,
                    Condition_Info              : Skill_Legend_Lore,
                ]),
                ([
                    Condition_Type_Code         : Condition_Type_Skill,
                    Condition_Info              : Skill_Mineral_Lore,
                    Condition_Value             : 0.1,
                ]),
            }),
        ]),
        Description_Content                     : ({
            "Ages ago, the katakacha were nearly driven to extinction by the kazar, and fled from them across the planes.  "
            "In recent times, the kazar found them once again, and would have eradicated them entirely if Amon Tyrice had not "
            "pled for Ganesha's intervention on their behalf.  They aid Ganesha and the Travelers out of gratitude for this "
            "act of kindness, one that had a great cost in making a formidable enemy of the kazar.",
        }),
    ]));
    add_proportion(([
        Element_Type                            : Material_Kacha,
        Element_Proportion                      : 1.0,
        Element_Flags                           : Element_Flag_Untransmutable | Element_Flag_Suppress_Material_Known_Adjectives | Element_Flag_Provide_Default_Color,
    ]));
    add_proportion(([
        Element_Type                            : Material_Quartz,
        Element_Proportion                      : 1.0,
        Element_Flags                           : Element_Flag_Untransmutable | Element_Flag_Secondary,
    ]));
    set_light(0.05);
    set_transactional_container(False);
    set_configuration_freeze(Configuration_Container_Status, True);
    add_hook(Do_Equip_Item, #'katakacha_do_equip);
    add_hook(Do_Unequip_Item, #'katakacha_do_unequip);
}

void process_post_restore(object who, record save, int restore_flags) {
    ::process_post_restore(who, save, restore_flags);
    set_transactional_container(False);
    set_configuration_freeze(Configuration_Container_Status, True);
}

status drop() {
    return True;
}

status query_auto_keep(object who) {
    return who->query_affiliation(project_control()) || ::query_auto_keep(who);
}

// hack to ignite webbing when it's got a hot damage type.  ideally this would be modelled by making the motes
// into an element dxr or dxrs that updated whenever the motes changed based on the damage effect.  but at present,
// damage effects are not defined as having elements associated with them.

varargs float disabled_query_property_proportion(mixed prop, int flags) {
    float out = ::query_property_proportion(&prop, flags);
    if(out < 1.0 && enhancement && sizeof(enhancement & ({ "fire", "heat", "light", "magma" })) && Is_Property(prop)) {
        if(!intp(prop))
            prop = Property(prop)->query_property_code();
        if(prop == Prop_Aflame)
            out = 1.0;
    }
    return out;
}
