#include <Travelers.h>
#include <daemon.h>

nosave private string modifier_tag;
private closure attach_process;
private closure detach_process;
private closure initialize_process;
private closure overcome_process;
private descriptor array attribute_modifiers;
private descriptor array resistance_modifiers;
private descriptor array applied_skill_modifiers;
private descriptor array skill_modifiers;
private descriptor array speed_modifiers;
private descriptor array trait_modifiers;
private descriptor eligibility_condition;
private descriptor initialize_display;
private descriptor overcome_display;
private int rarity;
private int value;
private mapping array scaled_attribute_modifiers;
private mapping array scaled_skill_modifiers;
private mapping component_selection_adjustments;
private mapping hooks;
private status prepend_indefinite_article;
private string name;

inherit Travelers_Definition("Challenge_Component");

void configure() {
    ::configure();
    set_broker(Travelers_Daemon("dharma"));
}

protected nomask void set_obstacle_name(string val) {
    name = val;
}

nomask string query_obstacle_name() {
    return name;
}

nomask string query_challenge_component_name() {
    return name;
}

protected nomask void set_obstacle_prepend_indefinite_article(status val) {
    prepend_indefinite_article = val;
}

nomask status query_obstacle_prepend_indefinite_article() {
    return prepend_indefinite_article;
}

protected nomask void set_obstacle_rarity(int val) {
    rarity = val;
}

nomask int query_obstacle_rarity() {
    return rarity;
}

protected nomask void set_obstacle_value(int val) {
    value = val;
}

nomask int query_obstacle_value() {
    return value;
}

protected nomask void set_obstacle_initialize_display(mixed val) {
    initialize_display = val && Message(val);
}

nomask descriptor query_obstacle_initialize_display() {
    return initialize_display;
}

protected nomask void set_obstacle_overcome_display(mixed val) {
    overcome_display = val && Message(val);
}

nomask descriptor query_obstacle_overcome_display() {
    return overcome_display;
}

protected nomask void set_obstacle_attach_process(closure cl) {
    attach_process = cl;
}

nomask closure query_obstacle_attach_process() {
    return attach_process;
}

protected nomask void set_obstacle_detach_process(closure cl) {
    detach_process = cl;
}

nomask closure query_obstacle_detach_process() {
    return detach_process;
}

protected nomask void set_obstacle_overcome_process(closure cl) {
    overcome_process = cl;
}

nomask closure query_obstacle_overcome_process() {
    return overcome_process;
}

protected nomask void set_obstacle_initialize_process(closure cl) {
    initialize_process = cl;
}

nomask closure query_obstacle_initialize_process() {
    return initialize_process;
}

protected nomask void add_obstacle_hook(int type, closure cl) {
    hooks ||= ([]);
    array_push(hooks[type], cl);
}

nomask mapping query_obstacle_hooks() {
    return hooks;
}

protected nomask void obstacle_engage_hooks(object who) {
    if(hooks)
        foreach(int type, mixed array cls : hooks)
            foreach(closure cl : cls)
                Call_Hook_On(who, type, cl, Call_Flag_Nonpersistent);
}

protected nomask void obstacle_disengage_hooks(object who) {
    if(hooks)
        foreach(int type, mixed array cls : hooks)
            foreach(closure cl : cls)
                Call_Hook_Off(who, type, cl, Call_Flag_Nonpersistent);
}

nomask string query_obstacle_modifier_tag() {
    return modifier_tag ||= "Ganesha_Obstacle_" + replace(capitalize_words(query_obstacle_name()), " ", "_") + "_Mod";
}

// temp compat; if you're reading this, replace this function with the version below
nomask status is_obstacle_modifier(descriptor dxr) {
    string tag = query_obstacle_modifier_tag();
    string key = Modifier_Query(dxr, Modifier_Key);
    if(key)
        return tag == key;
    else
        return Modifier_Query_Info(dxr, tag);
}
/*
nomask status is_obstacle_modifier(descriptor dxr) {
    return Modifier_Query(dxr, Modifier_Key) == query_obstacle_modifier_tag();
}
*/

protected nomask void add_obstacle_trait_modifier(mixed modifier) {
    if(mappingp(modifier))
        modifier[Modifier_Type] = Modifier_Type_Trait;
    descriptor dxr = Modifier(modifier);
    Modifier_Set(dxr, Modifier_Type, Modifier_Type_Trait);
    Modifier_Set(dxr, Modifier_Bound, Modifier_Query(dxr, Modifier_Amount) * 3);
    Modifier_Set(dxr, Modifier_Key, query_obstacle_modifier_tag());
    Modifier_Flag_On(dxr, Modifier_Flag_Unremovable);
    //Modifier_Flag_On(dxr, Modifier_Flag_Persistent);
    array_push(trait_modifiers, dxr);
}

nomask descriptor array query_obstacle_trait_modifiers() {
    return trait_modifiers;
}

protected nomask void obstacle_engage_trait_modifiers(object who) {
    if(trait_modifiers)
        foreach(descriptor dxr : trait_modifiers)
            who->add_trait_modifier(dxr);
}

nomask descriptor array find_obstacle_trait_modifiers(object who) {
    return filter(who->query_trait_modifiers(), #'is_obstacle_modifier);
}

protected nomask void obstacle_disengage_trait_modifiers(object who) {
    foreach(descriptor trait_mod : find_obstacle_trait_modifiers(who))
        who->remove_trait_modifier(trait_mod);
}

protected nomask void add_obstacle_skill_modifier(mixed modifier) {
    descriptor dxr = Modifier(modifier);
    Modifier_Set(dxr, Modifier_Type, Modifier_Type_Skill);
    Modifier_Set(dxr, Modifier_Bound, Modifier_Query(dxr, Modifier_Amount) * 3);
    Modifier_Flag_On(dxr, Modifier_Flag_Unremovable);
    //Modifier_Flag_On(dxr, Modifier_Flag_Persistent);
    Modifier_Set(dxr, Modifier_Key, query_obstacle_modifier_tag());
    array_push(skill_modifiers, dxr);
}

nomask descriptor array query_obstacle_skill_modifiers() {
    return skill_modifiers;
}

protected nomask void add_obstacle_scaled_skill_modifier(mapping spec) {
    if(!arrayp(spec[Modifier_Index]))
        spec[Modifier_Index] = ({ spec[Modifier_Index] });
    array_push(scaled_skill_modifiers, spec);
}

nomask mapping array query_obstacle_scaled_skill_modifiers() {
    return scaled_skill_modifiers;
}

protected nomask void obstacle_engage_skill_modifiers(object who) {
    if(skill_modifiers)
        foreach(descriptor dxr : skill_modifiers)
            who->add_skill_modifier(dxr);
    if(scaled_skill_modifiers) {
        foreach(mapping spec : scaled_skill_modifiers) {
            foreach(int skill : spec[Modifier_Index]) {
                int amount = round(spec[Modifier_Amount] * who->query_skill(skill));
                if(amount > 0) {
                    descriptor dxr = Modifier(([
                        Modifier_Type               : Modifier_Type_Skill,
                        Modifier_Index              : skill,
                        Modifier_Amount             : -amount,
                        Modifier_Bound              : amount * -3,
                        Modifier_Key                : query_obstacle_modifier_tag(),
                        Modifier_Flags              : Modifier_Flag_Unremovable | Modifier_Flag_Persistent,
                    ]));
                    who->add_skill_modifier(dxr);
                }
            }
        }
    }
}

nomask descriptor array find_obstacle_skill_modifiers(object who) {
    return filter(who->query_skill_modifiers(), #'is_obstacle_modifier);
}

protected nomask void obstacle_disengage_skill_modifiers(object who) {
    foreach(descriptor skill_mod : find_obstacle_skill_modifiers(who))
        who->remove_skill_modifier(skill_mod);
}

// Must be public, this is called from the GANESHA_CHALLENGE object
public nomask void recalculate_obstacle_scaled_skill_modifiers(object who) {
    if(!scaled_skill_modifiers)
        return;
    descriptor array mods = find_obstacle_skill_modifiers(who);
    foreach(mapping spec : scaled_skill_modifiers) {
        foreach(int skill : spec[Modifier_Index]) {
            if(!Is_Skill(skill))
                error("Invalid skill " + printable(skill));
            foreach(descriptor mod : mods) {
                int array index = Modifier_Query(mod, Modifier_Index);
                if(sizeof(index) == 1 && index[0] == skill && Modifier_Query(mod, Modifier_Key) == query_obstacle_modifier_tag()) {
                    who->remove_skill_modifier(mod, Modifier_Event_Flag_Suspend_Calculations);
                    break;
                }
            }
            int amount = round(spec[Modifier_Amount] * who->query_skill(skill));
            if(amount > 0) {
                descriptor dxr = Modifier(([
                    Modifier_Type               : Modifier_Type_Skill,
                    Modifier_Index              : skill,
                    Modifier_Amount             : -amount,
                    Modifier_Bound              : amount * -3,
                    Modifier_Key                : query_obstacle_modifier_tag(),
                    Modifier_Flags              : Modifier_Flag_Unremovable | Modifier_Flag_Persistent,
                ]));
                who->add_skill_modifier(dxr);
            }
        }
    }
}

protected nomask void add_obstacle_attribute_modifier(mixed modifier) {
    descriptor dxr = Modifier(modifier);
    Modifier_Set(dxr, Modifier_Type, Modifier_Type_Attribute);
    Modifier_Set(dxr, Modifier_Bound, Modifier_Query(dxr, Modifier_Amount) * 3);
    Modifier_Set(dxr, Modifier_Key, query_obstacle_modifier_tag());
    Modifier_Flag_On(dxr, Modifier_Flag_Unremovable);
    //Modifier_Flag_On(dxr, Modifier_Flag_Persistent);
    array_push(attribute_modifiers, dxr);
}

nomask descriptor array query_obstacle_attribute_modifiers() {
    return attribute_modifiers;
}

protected nomask void add_obstacle_scaled_attribute_modifier(mixed spec) {
    if(!arrayp(spec[Modifier_Index]))
        spec[Modifier_Index] = ({ spec[Modifier_Index] });
    array_push(scaled_attribute_modifiers, spec);
}

nomask mixed array query_obstacle_scaled_attribute_modifiers() {
    return scaled_attribute_modifiers;
}

protected nomask void obstacle_engage_attribute_modifiers(object who) {
    if(attribute_modifiers)
        foreach(descriptor dxr : attribute_modifiers)
            who->add_attribute_modifier(dxr);
    if(scaled_attribute_modifiers) {
        foreach(mapping spec : scaled_attribute_modifiers) {
            foreach(int attribute : spec[Modifier_Index]) {
                int amount = round(spec[Modifier_Amount] * who->query_attribute(attribute));
                if(amount > 0) {
                    descriptor dxr = Modifier(([
                        Modifier_Type               : Modifier_Type_Attribute,
                        Modifier_Index              : attribute,
                        Modifier_Amount             : -amount,
                        Modifier_Bound              : amount * -3,
                        Modifier_Key                : query_obstacle_modifier_tag(),
                        Modifier_Flags              : Modifier_Flag_Unremovable | Modifier_Flag_Persistent,
                    ]));
                    who->add_attribute_modifier(dxr);
                }
            }
        }
    }
}

nomask descriptor array find_obstacle_attribute_modifiers(object who) {
    return filter(who->query_attribute_modifiers(), #'is_obstacle_modifier);
}

protected nomask void obstacle_disengage_attribute_modifiers(object who) {
    foreach(descriptor attribute_mod : find_obstacle_attribute_modifiers(who))
        who->remove_attribute_modifier(attribute_mod);
}

// Must be public, this is called from the GANESHA_CHALLENGE object
public nomask void recalculate_obstacle_scaled_attribute_modifiers(object who) {
    if(!scaled_attribute_modifiers)
        return;
    descriptor array mods = find_obstacle_attribute_modifiers(who);
    status any = False;
    foreach(mapping spec : scaled_attribute_modifiers) {
        foreach(int attribute : spec[Modifier_Index]) {
            foreach(descriptor mod : mods) {
                int array index = Modifier_Query(mod, Modifier_Index);
                if(sizeof(index) == 1 && index[0] == attribute && Modifier_Query(mod, Modifier_Key) == query_obstacle_modifier_tag()) {
                    who->remove_attribute_modifier(mod, Modifier_Event_Flag_Suspend_Calculations);
                    any = True;
                    break;
                }
            }
            int amount = round(spec[Modifier_Amount] * who->query_attribute(attribute));
            if(amount > 0) {
                descriptor dxr = Modifier(([
                    Modifier_Type               : Modifier_Type_Attribute,
                    Modifier_Index              : attribute,
                    Modifier_Amount             : -amount,
                    Modifier_Bound              : amount * -3,
                    Modifier_Key                : query_obstacle_modifier_tag(),
                    Modifier_Flags              : Modifier_Flag_Unremovable | Modifier_Flag_Persistent,
                ]));
                who->add_attribute_modifier(dxr, Modifier_Event_Flag_Suspend_Calculations);
                any = True;
            }
        }
    }
    if(any)
        who->update_attributes();
}

protected nomask void add_obstacle_speed_modifier(mixed modifier) {
    descriptor dxr = Modifier(modifier);
    Modifier_Set(dxr, Modifier_Type, Modifier_Type_Speed);
    Modifier_Set(dxr, Modifier_Bound, Modifier_Query(dxr, Modifier_Amount) * 3);
    Modifier_Set(dxr, Modifier_Key, query_obstacle_modifier_tag());
    Modifier_Flag_On(dxr, Modifier_Flag_Unremovable);
    //Modifier_Flag_On(dxr, Modifier_Flag_Persistent);
    array_push(speed_modifiers, dxr);
}

nomask descriptor array query_obstacle_speed_modifiers() {
    return speed_modifiers;
}

protected nomask void obstacle_engage_speed_modifiers(object who) {
    if(speed_modifiers)
        foreach(descriptor dxr : speed_modifiers)
            who->add_speed_modifier(dxr);
}

nomask descriptor array find_obstacle_speed_modifiers(object who) {
    return filter(who->query_speed_modifiers(), #'is_obstacle_modifier);
}

protected nomask void obstacle_disengage_speed_modifiers(object who) {
    foreach(descriptor speed_mod : find_obstacle_speed_modifiers(who))
        who->remove_speed_modifier(speed_mod);
}

protected nomask void add_obstacle_resistance_modifier(mixed modifier) {
    descriptor dxr = Modifier(modifier);
    Modifier_Set(dxr, Modifier_Type, Modifier_Type_Resistance);
    Modifier_Set(dxr, Modifier_Bound, Modifier_Query(dxr, Modifier_Amount) * 3);
    Modifier_Set(dxr, Modifier_Key, query_obstacle_modifier_tag());
    Modifier_Flag_On(dxr, Modifier_Flag_Unremovable);
    //Modifier_Flag_On(dxr, Modifier_Flag_Persistent);
    array_push(resistance_modifiers, dxr);
}

nomask descriptor array query_obstacle_resistance_modifiers() {
    return resistance_modifiers;
}

protected nomask void obstacle_engage_resistance_modifiers(object who) {
    if(resistance_modifiers)
        foreach(descriptor dxr : resistance_modifiers)
            who->add_resistance_modifier(dxr);
}

nomask descriptor array find_obstacle_resistance_modifiers(object who) {
    return filter(who->query_resistance_modifiers(), #'is_obstacle_modifier);
}

protected nomask void obstacle_disengage_resistance_modifiers(object who) {
    foreach(descriptor resistance_mod : find_obstacle_resistance_modifiers(who))
        who->remove_resistance_modifier(resistance_mod);
}

protected nomask void set_challenge_component_selection_adjustments(mapping val) {
    component_selection_adjustments = val;
}

mapping query_challenge_component_selection_adjustments() {
    return component_selection_adjustments;
}

protected nomask void set_obstacle_eligibility_condition(mixed cond) {
    descriptor use;
    if(Is_Condition(cond))
        use = cond;
    else
        use = Condition(cond);
    eligibility_condition = use;
}

nomask descriptor query_obstacle_eligibility_condition() {
    return eligibility_condition;
}

nomask status query_obstacle_eligibility(object who) {
    descriptor cond = query_obstacle_eligibility_condition();
    unless(cond)
        return True;
    return Condition_Apply(cond, who, 0);
}

nomask void obstacle_attach(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    obstacle_engage_hooks(who);
    obstacle_engage_trait_modifiers(who);
    obstacle_engage_skill_modifiers(who);
    obstacle_engage_attribute_modifiers(who);
    obstacle_engage_speed_modifiers(who);
    funcall(attach_process, obj);
}

nomask void obstacle_detach(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    obstacle_disengage_hooks(who);
    obstacle_disengage_trait_modifiers(who);
    obstacle_disengage_skill_modifiers(who);
    obstacle_disengage_attribute_modifiers(who);
    obstacle_disengage_speed_modifiers(who);
    funcall(detach_process, obj);
}

nomask void obstacle_initialize(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    funcall(initialize_process, obj);
    descriptor display = query_obstacle_initialize_display();
    if(display)
        who->display(display);
}

nomask void obstacle_overcome(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    funcall(overcome_process, obj);
    descriptor display = query_obstacle_overcome_display();
    if(display)
        who->display(display);
}

nomask void obstacle_yield(object obj) {
    obstacle_overcome(obj);
}

nomask void obstacle_fail(object obj) {
    obstacle_overcome(obj);
}
