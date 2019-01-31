#include <Travelers.h>
#include <colors.h>
#include <die.h>
#include <explore.h>
#include <interval.h>
#include <item.h>
#include <locations.h>
#include <realtime.h>

inherit "/std/app/personal_enchantment";

#define Brahmanda_Prakasa_Maximum_Shekhinah_Proportion  9.0
#define Brahmanda_Prakasa_Base_Resistances              ({ "air", "earth", "fire", "water", "entropy", "extropy", })

nosave private descriptor array base_resistances;
nosave private descriptor array chakra_resistances;
nosave private descriptor energy_cost_message;
nosave private descriptor life_support;
nosave private descriptor light_adjustment;
nosave private mixed array element_color_result;
nosave private object chakra;
nosave private object owner;
nosave private object preferred_chakra;
nosave private status life_saving_subsequent;
private float charge = 0.0;
private float power = 0.0;
private int duration;
private status goetic_protection;
private status will_expire;

#define Brahmanda_Prakasa_Chakra_Resistances ([\
    "ajna"                                      : ([\
        "air"                                   : 0.9,\
        "extropy"                               : 0.7,\
        "water"                                 : 0.4,\
    ]),\
    "anahata"                                   : ([\
        "water"                                 : 0.9,\
        "earth"                                 : 0.7,\
        "extropy"                               : 0.4,\
    ]),\
    "manipura"                                  : ([\
        "water"                                 : 0.9,\
        "fire"                                  : 0.7,\
        "entropy"                               : 0.4,\
    ]),\
    "muladhara"                                 : ([\
        "earth"                                 : 0.9,\
        "poison"                                : 0.7,\
        "disease"                               : 0.4,\
    ]),\
    "svadhisthana"                              : ([\
        "fire"                                  : 0.9,\
        "force"                                 : 0.7,\
        "extropy"                               : 0.4,\
    ]),\
    "sahasrara"                                 : ([\
        "extropy"                               : 0.9,\
        "air"                                   : 0.7,\
        "earth"                                 : 0.4,\
    ]),\
    "vishuddha"                                 : ([\
        "goetia"                                : 0.9,\
        "magick"                                : 0.7,\
        "psionic"                               : 0.4,\
    ]),\
    "padme"                                     : ([\
        "chaos"                                 : 0.7,\
        "entropy"                               : 0.6,\
        "extropy"                               : 0.5,\
        "order"                                 : 0.4,\
    ]),\
    "kalila"                                    : ([\
        "order"                                 : 0.7,\
        "extropy"                               : 0.6,\
        "entropy"                               : 0.5,\
        "chaos"                                 : 0.4,\
    ]),\
    "ka"                                        : ([\
        "time"                                  : 0.7,\
        "cutting"                               : 0.5,\
        "piercing"                              : 0.5,\
        "crushing"                              : 0.5,\
    ]),\
])

#define Brahmanda_Prakasa_Chakra_Colors ([\
    "ajna"                                      : "star-filled " + LS_Chakra("ajna")->query_chakra_color(),\
    "anahata"                                   : "star-filled " + LS_Chakra("anahata")->query_chakra_color(),\
    "manipura"                                  : "star-filled " + LS_Chakra("manipura")->query_chakra_color(),\
    "muladhara"                                 : "star-filled " + LS_Chakra("muladhara")->query_chakra_color(),\
    "svadhisthana"                              : "star-filled " + LS_Chakra("svadhisthana")->query_chakra_color(),\
    "sahasrara"                                 : "star-filled " + LS_Chakra("sahasrara")->query_chakra_color(),\
    "vishuddha"                                 : "star-filled " + LS_Chakra("vishuddha")->query_chakra_color(),\
    "padme"                                     : "star-filled ocean blue",\
    "kalila"                                    : "star-filled sky blue",\
    "ka"                                        : "star-filled emerald",\
])

status is_brahmanda_prakasa() {
    return True;
}

void process_post_restore(object who, record save, int restore_flags) {
    ::process_post_restore(who, save, restore_flags);
    if(find_element(Material_Dweomer))
        transmute_element(Material_Dweomer, Material_Goetia);
    if(sizeof(armour()->query_absorption_modifiers()))
        foreach(mixed modifier : armour()->query_absorption_modifiers())
            armour()->remove_absorption_modifier(modifier);
    personal_enchantment_set_unbound_armour(True);
    armour()->set_field_effect(True);
}

descriptor brahmanda_prakasa_query_owner_covering_element() {
    object who = personal_enchantment_query_owner();
    if(!who)
        return 0;
    foreach(descriptor dxr : who->query_elements())
        if(Element_Flag_Check(dxr, Element_Flag_Character_Appear | Element_Flag_Provide_Character_Color_Adjective | Element_Flag_Provide_Character_Color_Alternate))
            return dxr;
    descriptor bulk = who->find_element(Element_Part, Part_Bulk);
    if(bulk)
        return bulk;
    return who->query_elements()[0];
}

string brahmanda_prakasa_query_owner_covering_element_name() {
    return Element_Name(brahmanda_prakasa_query_owner_covering_element());
}

descriptor brahmanda_prakasa_query_goetia_element() {
    return find_element(Material_Goetia);
}

string brahmanda_prakasa_query_goetia_element_color() {
    descriptor dxr = brahmanda_prakasa_query_goetia_element();
    return dxr ? Element_Query(dxr, Element_Color) : "starry";
}

varargs string brahmanda_prakasa_color_with_goetia(string pre, string post) {
    return self_color(brahmanda_prakasa_query_goetia_element_color(), pre, post);
}

string brahmanda_prakasa_goetia_colored_bubbles() {
    return brahmanda_prakasa_color_with_goetia(0, "bubbles");
}

int brahmanda_prakasa_set_duration(int val) {
    will_expire = True;
    duration = val;
    return duration;
}

int brahmanda_prakasa_query_duration() {
    return duration;
}

int brahmanda_prakasa_add_duration(int val) {
    int left = brahmanda_prakasa_set_duration(brahmanda_prakasa_query_duration() + val);
    if(left < 1)
        personal_enchantment_expire();
    return left;
}

void brahmanda_prakasa_set_charge(float val) {
    charge = max(val, 0.0);
}

void brahmanda_prakasa_add_charge(float val) {
    charge = max(charge + val, 0.0);
}

float brahmanda_prakasa_query_charge() {
    return charge;
}

object brahmanda_prakasa_find_chakra() {
    foreach(object obj : personal_enchantment_query_owner()->query_child_objects())
        if(obj->is_chakra() && obj->query_info("Brahmanda_Prakasa_Chakra"))
            return obj;
    return 0;
}

object brahmanda_prakasa_query_chakra() {
    return chakra ||= brahmanda_prakasa_find_chakra();
}

object brahmanda_prakasa_find_preferred_chakra() {
    foreach(object obj : personal_enchantment_query_owner()->query_child_objects())
        if(obj->is_chakra() && obj->query_info("Brahmanda_Prakasa_Preferred_Chakra"))
            return obj;
    return 0;
}

object brahmanda_prakasa_query_preferred_chakra() {
    return preferred_chakra ||= brahmanda_prakasa_find_preferred_chakra();
}

private mixed brahmanda_prakasa_poll_element_color(mixed event) {
    descriptor element = mappingp(event) ? event["element"] : event[Poll_Element_Color_Event_Element];
    if(element == brahmanda_prakasa_query_owner_covering_element())
        return element_color_result ||= ({ brahmanda_prakasa_query_goetia_element_color(), 5 });
    return 0;
}

varargs void brahmanda_prakasa_activate_passing_effect(object who) {
    who ||= personal_enchantment_query_owner();
    if(!who) {
        personal_enchantment_expire();
        return;
    }
    mapping rec = Element_Record(who);
    string color = brahmanda_prakasa_query_goetia_element_color();
    who->add_hook(Poll_Element_Color, #'brahmanda_prakasa_poll_element_color);
    if(rec)
        Element_Record_Message(who, rec, ({
            "{{" + color + "}luminous bubbles} swirl tightly around", who, ", and",
        }), ({
            "as", ({ 'p', who }), ({ "merge", who }), "with the energy"
        }), ([
            Message_Senses      : Message_Sense_Visual | Message_Sense_Astral,
        ]));
}

varargs void brahmanda_prakasa_deactivate_passing_effect(object who) {
    who ||= personal_enchantment_query_owner();
    if(!who) {
        personal_enchantment_expire();
        return;
    }
    mapping rec = Element_Record(who);
    string color = brahmanda_prakasa_query_goetia_element_color();
    who->remove_hook(Poll_Element_Color, #'brahmanda_prakasa_poll_element_color);
    if(rec)
        Element_Record_Message(who, rec, 0, ({
            "as a seeming {{" + color + "}infinity of luminous bubbles} pour outward from", ({ 'r', who, "body" }),
        }), ([
            Message_Senses      : Message_Sense_Visual | Message_Sense_Astral,
        ]));
}

void brahmanda_prakasa_update_passing_effect(object old_chakra, object new_chakra, string color) {
    object who = personal_enchantment_query_owner();
    mapping rec = Element_Record(who);
    alter_element(find_element(Material_Goetia), Element_Color, color);
    element_color_result = 0;
    mixed array pre_message;
    if(old_chakra && new_chakra)
        pre_message = ({
            "bubbles of energy centered around", ({ 's', who, old_chakra }), "swirl and regather around",
            ({ 'r', who, new_chakra }), ", and",
        });
    else if(!old_chakra && !new_chakra)
        pre_message = ({
            "pockets of energy bubbles clinging to", who, "disperse evenly around", ({ 'o', who }), ", and",
        });
    else if(!old_chakra && new_chakra)
        pre_message = ({
            "bubbles of energy form around", who, "and swirl and gather around", ({ 'r', who, new_chakra }),
            ", and",
        });
    else if(old_chakra && !new_chakra)
        pre_message = ({
            "bubbles of energy centered around", ({ 's', who, old_chakra }), "disperse, and",
        });
    if(rec)
        unless(old_chakra == new_chakra && (old_chakra || new_chakra))
            Element_Record_Message(who, rec, pre_message, 0, ([
                Message_Senses      : Message_Sense_Visual | Message_Sense_Astral,
            ]));
}

void brahmanda_prakasa_synchronize_with_chakra() {
    object who = personal_enchantment_query_owner();
    object new_chakra;
    object old_chakra = brahmanda_prakasa_query_chakra();
    object pref_chakra = brahmanda_prakasa_query_preferred_chakra();
    if(pref_chakra)
        new_chakra = pref_chakra;
    else
        foreach(object obj : all_inventory(who))
            if(obj->is_chakra())
                if(!new_chakra || obj->query_chakra_energy() > new_chakra->query_chakra_energy())
                    new_chakra = obj;
    string color;
    if(!new_chakra)
        color = "starry";
    else
        color = Brahmanda_Prakasa_Chakra_Colors[new_chakra->query_chakra_name()];
    if(chakra_resistances)
        foreach(descriptor mod : chakra_resistances)
            if(mod)
                who->remove_resistance_modifier(mod);
    if(old_chakra) {
        old_chakra->remove_info("Brahmanda_Prakasa_Chakra");
        chakra = 0;
    }
    chakra_resistances = 0;
    if(new_chakra) {
        mapping resistances = Brahmanda_Prakasa_Chakra_Resistances[new_chakra->query_chakra_name()];
        chakra_resistances = allocate(sizeof(resistances));
        int base = personal_enchantment_effectiveness_scale(25, 100);
        int ix = 0;
        foreach(string type, float factor : resistances)
            chakra_resistances[ix++] = who->add_resistance_modifier(([
                Modifier_Index              : type,
                Modifier_Amount             : round(base * factor),
            ]));
        new_chakra->set_info("Brahmanda_Prakasa_Chakra", True);
        chakra = new_chakra;
    }
    brahmanda_prakasa_update_passing_effect(old_chakra, new_chakra, color);
}

mixed personal_enchantment_determine_effectiveness(object who, object target) {
    unless(who)
        return 0;
    return who->affiliation_links_reduce((: max($1, $2->query_brahmanda_prakasa_performance_rating()) :));
}

nomask float query_brahmanda_prakasa_power() {
    return power;
}

nomask float calculate_brahmanda_prakasa_raw_power() {
    object who = personal_enchantment_query_owner();
    unless(who)
        return 0;
    float effective_karma = to_float(Ganesha_Query_Karma_Phala(who));
    int points = who->query_exploration_points();
    float universe_exploration_proportion = to_float(points) / Exploration_Point_Total;
    float universe_exploration_proportion_factor = scale_conversion(universe_exploration_proportion, 0.0, 1.0, 0.2, 1.0);
    effective_karma *= universe_exploration_proportion_factor;
    effective_karma *= personal_enchantment_query_effectiveness();
    return effective_karma;
}

nomask float calculate_brahmanda_prakasa_power() {
    float raw_power = calculate_brahmanda_prakasa_raw_power();
    return diminishing_cap(raw_power * 0.0001, Brahmanda_Prakasa_Maximum_Shekhinah_Proportion);
}

void brahmanda_prakasa_add_default_elements() {
    add_proportion(([
        Element_Type                            : Material_Goetia,
        Element_Proportion                      : 1.0,
        Element_Color                           : "starry",
        Element_Flags                           : Element_Flag_Suppress_Material_Blend | Element_Flag_Suppress_Material_Name | Element_Flag_Provide_Default_Color,
    ]));
}

varargs void brahmanda_prakasa_update(status force) {
    object who = personal_enchantment_query_owner();
    unless(who)
        return;
    if(force) {
        personal_enchantment_refresh_effectiveness();
        power = calculate_brahmanda_prakasa_power();
    } else {
        float old_eff = personal_enchantment_query_effectiveness();
        personal_enchantment_refresh_effectiveness();
        float cur_power = calculate_brahmanda_prakasa_power();
        if(old_eff == personal_enchantment_query_effectiveness() && cur_power == power)
            return;
        power = cur_power;
    }
    set_will_update_configuration(True);
    set_elements(0);
    /*
    proportionalize_elements();
    descriptor empowerment = find_proportion(Element_Key, "Brahmanda_Prakasa_Empowerment");
    if(empowerment)
        remove_proportion(empowerment);
    */
    brahmanda_prakasa_add_default_elements();
    if(power > 0) {
        float total_proportion = 0;
        foreach(descriptor dxr : query_proportions())
            total_proportion += Element_Query(dxr, Element_Proportion);
        if(total_proportion > 0)
            add_proportion(([
                Element_Type        : Material_Shekhinah,
                Element_Proportion  : total_proportion * power,
            ]));
    }
    update_configuration(True);
    brahmanda_prakasa_synchronize_with_chakra();
}

void brahmanda_prakasa_switch_goetic_protection() {
    if(!goetic_protection) {
        goetic_protection = True;
        user_message(([
            Message_Content                             : ({
                0, ({ "begin", 0 }), "to effervesce rhythmically",
            }),
            Message_Senses                              : Message_Sense_Visual | Message_Sense_Astral,
        ]));
    } else {
        goetic_protection = False;
        user_message(([
            Message_Content                             : ({
                0, ({ "cease", 0 }), "to effervesce and", ({ "stabilize", 0 }),
            }),
            Message_Senses                              : Message_Sense_Visual | Message_Sense_Astral,
        ]));
    }
}

void brahmanda_prakasa_switch_goetic_protection_refraction() {
}

void brahmanda_prakasa_owner_do_contact(mapping args) {
    object who = personal_enchantment_query_owner();
    unless(args["contacting"] == who)
        return;
    mixed obj = args["contacted"];
    if(!objectp(obj))
        return;
    string verb = args["verb"];
    if(verb == "tap") {
        unless(obj->is_chakra())
            return;
        unless(obj->query_chakra_subject() == who)
            return;
        object old_pref = brahmanda_prakasa_query_preferred_chakra();
        if(old_pref) {
            old_pref->remove_info("Brahmanda_Prakasa_Preferred_Chakra");
            preferred_chakra = 0;
        }
        obj->set_info("Brahmanda_Prakasa_Preferred_Chakra", True);
        preferred_chakra = obj;
        brahmanda_prakasa_synchronize_with_chakra();
    } else if(verb == "touch") {
        unless(obj == who)
            return;
        int limb = args["contacted_limb"];
        if(limb == Null)
            return;
        unless(who->query_limb_type(limb) == Limb_Type_Head)
            return;
        if(find_call_out("brahmanda_prakasa_switch_goetic_protection_refraction") != Null)
            return;
        brahmanda_prakasa_switch_goetic_protection();
        call_out("brahmanda_prakasa_switch_goetic_protection_refraction", 2);
    }
}

void brahmanda_prakasa_life_saving_done() {
    life_saving_subsequent = False;
}

private mixed brahmanda_prakasa_owner_can_die(mapping args, descriptor ctx) {
    int flags = args["flags"];
    if(flags & Die_Flag_Suppress_Life_Saving)
        return True;
    if(!query_any_realm("NM") && !query_any_realm("NL") && semirandom(charge) > 100)
        return False;
    return True;
}

private void brahmanda_prakasa_owner_fail_die(mapping args, descriptor ctx) {
    if(Hook_Context_Query(ctx, Hook_Context_Failure_From) != #'brahmanda_prakasa_owner_can_die)
        return;
    object who = args["who"];
    if(life_saving_subsequent) {
        who->restore_limbs();
        who->set_hit_points();
        who->set_endurance(0.0);
        who->set_spell_points(0.0);
        return;
    }
    call_out("brahmanda_prakasa_life_saving_done", 2);
    user_message(([
        Message_Content                         : ({
            0, ({ "pulse", 0 }), "violently",
        }),
        Message_Senses                          : Message_Sense_Visual | Message_Sense_Astral,
        Message_Color                           : brahmanda_prakasa_query_goetia_element_color(),
    ]), who);
    who->message(([
        Message_Content                         : ({
            "all of", ({ 's', 0, "wounds" }), "instantly heal",
        }),
        Message_Senses                          : Message_Sense_Visual | Message_Sense_Tactile_For_Participants,
    ]));
    who->restore_limbs();
    who->set_hit_points();
    who->remove_causts();
    foreach(object obj : all_inventory(who)) {
        if(!obj)
            continue;
        if(obj->query_indestructible())
            continue;
        else if(obj->is_curse())
            obj->remove();
        else if(obj->is_disease() && semirandom(charge, 10) > obj->query_disease_virulence())
            obj->remove();
    }
    brahmanda_prakasa_set_charge(0.0);
    who->display(([
        Message_Content                         : ({
            0, ({ "feel", 0 }), "utterly exhausted",
        }),
        Message_Senses                          : Message_Sense_Tactile,
        Message_Color                           : "status: danger",
    ]));
    who->set_endurance(0.0);
    who->set_spell_points(0.0);
    object env = environment(who);
    unless(query_any_realm("NT", who)) {
        object room = Random_Location_Safe;
        string swirl = "in " + a(brahmanda_prakasa_color_with_goetia("swirl", "bubbles"));
        if(environment(who) != room)
            who->move_living(room, swirl, swirl);
    }
    log_file("Life_Saving", "[" + ctime() + "] subject: " + printable(who) + "; mechanism: " + printable(this_object()) + "; attacker: " + printable(args["attacker"]) + "; environment: " + printable(env) + "; environment after: " + printable(environment(who)));
    life_saving_subsequent = True;
}

mixed brahmanda_prakasa_assist() {
    object who = personal_enchantment_query_owner();
    if(!who)
        return Interval_Terminate;
    if(query_any_realm("NM"))
        return Interval_Terminate;
    status any = False;
    if(who->query_stunned() && semirandom(charge) >= 10) {
        user_message(([
            Message_Content                     : ({
                0, ({ "pulse", 0 }), "rhythmically",
            }),
            Message_Senses                      : Message_Sense_Visual | Message_Sense_Astral,
        ]), who);
        who->remove_stun();
        brahmanda_prakasa_add_charge(-5.0);
        any = True;
    }
    if(who->query_blindness() && semirandom(charge) >= 20) {
        who->remove_blindness();
        if(!who->query_blindness()) {
            user_message(([
                Message_Content                 : ({
                    0, ({ "swirl", 0 }), "hypnotically",
                }),
                Message_Senses                  : Message_Sense_Visual | Message_Sense_Astral,
            ]), who);
            brahmanda_prakasa_add_charge(-10.0);
            any = True;
        }
    }
    if(who->query_trait(Trait_Deafness) && semirandom(charge) >= 10) {
        who->remove_deafness();
        if(!who->query_trait(Trait_Deafness)) {
            user_message(([
                Message_Content                 : ({
                    0, ({ "twist", 0 }), "in spiraling shapes",
                }),
                Message_Senses                  : Message_Sense_Visual | Message_Sense_Astral,
            ]), who);
            brahmanda_prakasa_add_charge(-5.0);
            any = True;
        }
    }
    if(who->query_trait(Trait_Numbness) && semirandom(charge) >= 10) {
        who->remove_numbness();
        if(!who->query_trait(Trait_Numbness)) {
            user_message(([
                Message_Content                 : ({
                    0, ({ "pulse", 0 }), "irregularly",
                }),
                Message_Senses                  : Message_Sense_Visual | Message_Sense_Astral,
            ]), who);
            brahmanda_prakasa_add_charge(-5.0);
            any = True;
        }
    }
    if(who->query_poisoning() && semirandom(charge) >= 15) {
        user_message(([
            Message_Content                     : ({
                0, ({ "whirl", 0 }), "rapidly",
            }),
            Message_Senses                      : Message_Sense_Visual | Message_Sense_Astral,
        ]), who);
        who->remove_poisoning();
        brahmanda_prakasa_add_charge(-7.5);
        any = True;
    }
    if(sizeof(who->query_causts()) && semirandom(charge) >= 30) {
        user_message(([
            Message_Content                     : ({
                0, ({ "expand", 0 }), "and", ({ "contract", 0 }), "suddenly",
            }),
            Message_Senses                      : Message_Sense_Visual | Message_Sense_Astral,
        ]), who);
        who->remove_causts();
        brahmanda_prakasa_add_charge(-15.0);
        any = True;
    }
    return any ? 15 : Interval_Terminate;
}

descriptor query_brahmanda_prakasa_energy_cost_message() {
    return energy_cost_message ||=
        clonep(this_object())
        ? (blueprint() || load_name())->query_brahmanda_prakasa_energy_cost_message()
        :
            Message(([
                Message_Content                 : ({
                    0, ({ "feel", 0 }), ({ 'r', 0, this_object() }), "drawing spiritual energy from", ({ 'o', 0 }),
                    "as", ({ 'p', this_object() }), ({ "charge", this_object() }), ({ 'l', this_object() }),
                    "from the energies being directed at", ({ 'o', 0 }),
                }),
                Message_Senses                  : Message_Sense_Spiritual,
                Message_Flags                   : Message_Flag_Source_Only | Message_Flag_Filter_To_Alternate,
                Message_Difficulty              : Message_Difficulty_Hard,
                Message_Color                   : "status: risk",
                Message_Categories              : Message_Category_Energy_Maintenance,
                Message_Alternate               : ([
                    Message_Content             : ({
                        Energy(Energy_Spiritual)->query_energy_colored_appearance(), "flows from", 0, "to",
                        ({ 'r', 0, this_object() }),
                    }),
                    Message_Senses              : Message_Sense_Astral,
                    Message_Difficulty          : Message_Difficulty_Hard,
                    Message_Flags               : Message_Flag_Exclude_Source,
                ]),
            ]))
    ;
}

varargs status brahmanda_prakasa_check_goetic_energy(status silent) {
    object who = personal_enchantment_query_owner();
    if(who)
        if(who->query_energy_amount(Energy_Goetic) > 0)
            return True;
    unless(silent)
        who->display(([
            Message_Content                     : ({
                0, ({ "sense", 0 }), this_object(), "attempting to draw upon", ({ 'r', 0, "exhausted reserves of goetic energy" }),
            }),
            Message_Senses                      : Message_Sense_Spiritual,
            Message_Color                       : "alert: high",
        ]));
    brahmanda_prakasa_switch_goetic_protection();
    return False;
}

void brahmanda_prakasa_at_damage_absorbed(mapping args) {
    if(!goetic_protection)
        return;
    descriptor attack = args["attack"];
    if(Attack_Flag_Check(attack, Attack_Flag_Hypothetical))
        return;
    float total = Attack_Total_Damage(attack);
    object who = Attack_Query(attack, Attack_Target);
    unless(who && who == personal_enchantment_query_owner())
        return;
    if(total <= 10.0)
        return;
    unless(brahmanda_prakasa_check_goetic_energy())
        return;
    int limbs = sizeof(who->query_specified_limbs_indices(Attack_Query(attack, Attack_Limb)));
    float goetic_energy = who->query_energy_amount(Energy_Goetic);
    float efficiency = diminishing_returns(calculate_brahmanda_prakasa_raw_power(), 0.01);
    float absorb_cost = diminishing_returns(total / efficiency, 20);
    who->display(([
        Message_Content                     : ({
            0, ({ "sense", 0 }), this_object(), "drawing upon", ({ 'r', 0, "reserves of goetic energy" }),
            "to absorb the attack",
        }),
        Message_Senses                      : Message_Sense_Spiritual,
        Message_Color                       : "status: risk",
    ]));
    user_message(([
        Message_Content                     : ({
            0, ({ "effervesce", 0 }), "steadily",
        }),
        Message_Senses                      : Message_Sense_Astral | Message_Sense_Visual,
    ]));
    who->add_energy_amount(Energy_Goetic, -absorb_cost);
    mixed damage = Attack_Query(attack, Attack_Damage);
    if(arrayp(damage))
        Attack_Set(attack, Attack_Damage, allocate(sizeof(damage), 0.0));
    else
        Attack_Set(attack, Attack_Damage, 0.0);
}

int brahmanda_prakasa_mod_absorb_damage(mapping args) {
    object who = args["target"];
    descriptor attack = args["attack"];
    mixed type = Attack_Query(attack, Attack_Damage_Type);
    float charge_proportion = 0;
    if(Damage_Type_Find(type, Damage_Type_Temporal))
        charge_proportion += 1.0;
    if(Damage_Type_Find(type, "light"))
        charge_proportion += 0.70 * sizeof(Damage_Model("light")->query_damage_model_types());
    else if(Damage_Type_Find(type, "heat"))
        charge_proportion += 0.90 * sizeof(Damage_Model("heat")->query_damage_model_types());
    if(Damage_Type_Find(type, "eskara"))
        charge_proportion += 0.80 * sizeof(Damage_Model("eskara")->query_damage_model_types());
    if(Damage_Type_Find(type, Damage_Type_Force))
        charge_proportion += 0.40;
    if(Damage_Type_Find(type, Damage_Type_Divine))
        charge_proportion += 0.10;
    if(Damage_Type_Find(type, Damage_Type_Primal))
        charge_proportion += 0.05;
    charge_proportion /= sizeof(type);
    if(charge_proportion > 1)
        charge_proportion = 1.0;
    float usable = Attack_Total_Damage(attack) * charge_proportion;
    if(usable > 0) {
        float amount = diminishing_returns(usable, 0.1);
        if(who->query_real_spell_points() >= amount) {
            descriptor msg = query_brahmanda_prakasa_energy_cost_message();
            int diff = max(Message_Difficulty_Hard - round(amount * Message_Difficulty_Adjust_Medium), 0);
            Message_Set(msg, Message_Difficulty, diff);
            Message_Set(Message_Query(msg, Message_Alternate), Message_Difficulty, diff);
            who->message(msg);
            who->add_spell_points(-amount);
            brahmanda_prakasa_add_charge(amount);
        }
    }
    Interval_Require(#'brahmanda_prakasa_assist, 4);
    return 0;
}

void personal_enchantment_maintenance(object owner, object creator, object maintainer) {
    brahmanda_prakasa_update();
    if(will_expire)
        brahmanda_prakasa_add_duration(-personal_enchantment_query_maintenance_interval());
}

void personal_enchantment_attach(object who) {
    ::personal_enchantment_attach(who);
    if(who->is_autonomon() && !who->query_incarnos_control())
        brahmanda_prakasa_add_charge(fuzz(personal_enchantment_query_effectiveness() * 300.0));
    brahmanda_prakasa_activate_passing_effect(who);
    brahmanda_prakasa_update(True);
    life_support = who->add_trait_modifier(([
        Modifier_Index                          : Trait_Respiration,
        Modifier_Amount                         : 5,
        Modifier_Specification                  : Respiration_Internal,
    ]));
    string array base_types = Brahmanda_Prakasa_Base_Resistances;
    base_resistances = allocate(sizeof(base_types));
    int ix = 0;
    foreach(string type : base_types)
        base_resistances[ix++] = who->add_resistance_modifier(([
            Modifier_Index                      : type,
            Modifier_Amount                     : 10,
        ]));
    light_adjustment = who->add_trait_modifier(([
        Modifier_Index                          : Trait_Light_Sensitivity,
        Modifier_Amount                         : -2,
    ]));
    who->add_hook(At_Damage_Absorbed, #'brahmanda_prakasa_at_damage_absorbed);
    goetic_protection = True;
}

void personal_enchantment_detach(object who) {
    who->remove_hook(At_Damage_Absorbed, #'brahmanda_prakasa_at_damage_absorbed);
    brahmanda_prakasa_deactivate_passing_effect();
    who->remove_trait_modifier(&life_support);
    who->remove_trait_modifier(&light_adjustment);
    if(base_resistances)
        foreach(descriptor mod : base_resistances)
            if(mod)
                who->remove_resistance_modifier(mod);
    base_resistances = 0;
    if(chakra_resistances)
        foreach(descriptor mod : chakra_resistances)
            who->remove_resistance_modifier(mod);
    ::personal_enchantment_detach(who);
}

mixed personal_enchantment_validate(object who, object target) {
    return !target->query_child_object("is_brahmanda_prakasa");
}

varargs void personal_enchantment_setup(status temporary) {
    if(temporary)
        brahmanda_prakasa_set_duration(personal_enchantment_effectiveness_scale(Time_Hour, Time_Day));
}

void configure() {
    ::configure();
    set_identity(([
        Identity_Type           : Identity_Type_Custom,
        Identity_Name           : "brahmanda-prakasa",
        Identity_Special_Names  : ({ "BRAHMANDA-PRAKASA", "PROTECTION_AURA" }),
        Identity_Nouns          : ({ "aura" }),
        Identity_Adjectives     : ({ "cosmic" }),
    ]));
    set_identify_skills(([
        Skill_Arcane_Lore       : 100,
        Skill_Cosmology         : 60,
        Skill_Legend_Lore       : 55,
        Skill_Theology          : 75,
    ]));
    set_short(0);
    add_description(({
        "This is", 'a',
        Description(([
            Description_Type    : Description_Type_Element_Color,
            Description_Content : ({ "luminous", "aura" }),
        ])),
        ".",
    }));
    add_known_description(({
        'v', ({ "recognize", 'v' }), ({ 'o', 0 }), "as a brahmanda-prakasa, a field of energy that mystically reflects the origins of cosmoi -- "
        "this cosmos and innumerable others -- enveloping the possessor of the aura and serving to protect and empower hir, especially "
        "with respect to traveling through the many divergent spaces of the universe.",
    }));
    set_craft(Craft_Unearthly);
    armour()->set_armour_type(Armour_Type_Aura);
    armour()->set_ablative(False);
    armour()->set_field_effect(True);
    armour()->set_race("human");
    personal_enchantment_set_maintenance_interval(Time_Minute);
    personal_enchantment_set_unbound_armour(True);
    personal_enchantment_set_mod_description(([
        Description_Type                        : Description_Type_Simple,
        Description_Content                     : ({
            ({ 'a', "%t" }), ({ "bubble", "%t" }), "and", ({ "swirl", "%t" }), "tightly around", ({ 'r', "%o", "body" }),
            ", constantly merging and emerging from", ({ 'r', "%o", #'brahmanda_prakasa_query_owner_covering_element_name }),
        }),
    ]));
    personal_enchantment_set_add_display(([
        Message_Content                         : ({
            "%o", ({ "feel", "%o" }), "Ganesha's majesty hatching a miniature cosmos around", ({ 'o', "%o" }),
        }),
        Message_Senses                          : Message_Sense_Cognitive | Message_Sense_Spiritual,
        Message_Color                           : "status: safety",
    ]));
    personal_enchantment_set_add_message(([
        Message_Content                         : ({
            #'brahmanda_prakasa_goetia_colored_bubbles, "swirl around", "%o", "for a moment and then",
            "coalesce into", ({ 'a', "%t" })
        }),
        Message_Senses                          : Message_Sense_Astral | Message_Sense_Visual,
    ]));
    personal_enchantment_set_remove_display(([
        Message_Content                         : ({
            "%o", ({ "feel", "%o" }), ({ 's', "%o", "%t" }), "blending with the extant cosmos",
            "to the point of dissipation",
        }),
        Message_Senses                          : Message_Sense_Cognitive | Message_Sense_Spiritual,
        Message_Color                           : "status: risk",
    ]));
    personal_enchantment_set_remove_message(([
        Message_Content                         : ({
            #'brahmanda_prakasa_goetia_colored_bubbles, "flow outward from", ({ 's', "%o", "%t" }), "as", ({ 'p', "%t" }),
            ({ "dissipate", "%t" }), "into the surrounding", Description(Description_Type_Ambient_Medium_Colored_Name)
        }),
        Message_Senses                          : Message_Sense_Astral | Message_Sense_Visual,
    ]));
    personal_enchantment_set_hooks(({
        ({
            Fail_Die,
            #'brahmanda_prakasa_owner_fail_die,
        }),
        ({
            Can_Die,
            #'brahmanda_prakasa_owner_can_die,
        }),
        ({
            Do_Contact,
            #'brahmanda_prakasa_owner_do_contact,
        }),
    }));
    add_hook(Mod_Absorb_Damage, #'brahmanda_prakasa_mod_absorb_damage);
    brahmanda_prakasa_add_default_elements();    
    set_light(0.5);
}
