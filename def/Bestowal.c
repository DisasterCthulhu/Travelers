#include <daemon.h>
#include <services_master.h>
#include <Travelers.h>

private closure reward_process;
private descriptor array specialty_access;
private descriptor display;
private descriptor eligibility_condition;
private int array attribute_adjustment;
private int rarity;
private int type;
private int value;
private mixed array katakacha_damage_types;
private string array kabatha_resistance_types;
private status universality;
private string array required_bestowals;
private string description;
private string name;

inherit "/std/definition";

void configure() {
    ::configure();
    set_broker(Travelers_Daemon("dharma"));
}

void set_bestowal_name(string val) {
    name = val;
}

string query_bestowal_name() {
    return name;
}

void set_bestowal_type(int val) {
    type = val;
}

int query_bestowal_type() {
    return type;
}

void set_bestowal_rarity(int val) {
    rarity = val;
}

int query_bestowal_rarity() {
    return rarity;
}

void set_bestowal_value(int val) {
    value = val;
}

int query_bestowal_value() {
    return value;
}

void set_bestowal_universality(status val) {
    universality = val;
}

status query_bestowal_universality() {
    return universality;
}

void set_bestowal_required_bestowals(string array val) {
    required_bestowals = val;
}

string array query_bestowal_required_bestowals() {
    return required_bestowals;
}

void set_bestowal_eligibility_condition(mixed val) {
    eligibility_condition = val && Condition(val);
}

descriptor query_bestowal_eligibility_condition() {
    return eligibility_condition;
}

status query_bestowal_eligibility(object who) {
    if(!query_bestowal_universality()) {
        object link = who->query_affiliation_link(project_control());
        if(!link)
            return False;
        if(query_bestowal_type() != Travelers_Bestowal_Type_Recurring)
            if(member(link->travelers_query_bestowals(), query_bestowal_name()) != Null)
                return False;
        string array bestowals = query_bestowal_required_bestowals();
        if(bestowals)
            if(sizeof(bestowals&link->travelers_query_bestowals()) != sizeof(bestowals))
                return False;
    } else if(query_bestowal_type() != Travelers_Bestowal_Type_Recurring) {
        mapping aff = who->query_affiliation_info(project_control());
        aff["bestowals"] ||= ({});
        if(member(aff["bestowals"], name) != Null)
            return False;
    }
    descriptor cond = query_bestowal_eligibility_condition();
    return !cond || Condition_Apply(cond, who, 0);
}

void set_bestowal_reward_process(closure cl) {
    reward_process = cl;
}

closure query_bestowal_reward_process() {
    return reward_process;
}

void set_bestowal_description(string desc) {
    description = desc;
}

string query_bestowal_description() {
    return description;
}

void set_bestowal_display(mixed display) {
    display = Message(display);
}

descriptor query_bestowal_display() {
    if(display)
        return display;
    if(description)
        return display = Message(([
            Message_Content         : ({
                'j', "{{status: enhancement}",
                0, ({ "sense", 0 }), ({ 'l', 0 }), "being bestowed with", description,
                "}"
            }),
            Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
        ]));
    return 0;
}

void set_bestowal_attribute_adjustment(int attr, int amount) {
    attribute_adjustment = ({ attr, amount });
}

int array query_bestowal_attribute_adjustment() {
    return attribute_adjustment;
}

void set_bestowal_specialty_access(mixed spec) {
    descriptor array use;
    if(mappingp(spec))
        use = ({ Specialty(spec) });
    else if(arrayp(spec))
        use = map(spec, (: (Is_Specialty($1) && $1) || Specialty($1) :));
    specialty_access = use;
}

descriptor array query_bestowal_specialty_access() {
    return specialty_access;
}

void set_bestowal_katakacha_damage_types(mixed array types) {
    katakacha_damage_types = types;
}

mixed array query_bestowal_katakacha_damage_types() {
    return katakacha_damage_types;
}

string array query_bestowal_katakacha_damage_types_and_combos() {
    unless(katakacha_damage_types)
        return 0;
    string array out = ({});
    foreach(mixed array type : katakacha_damage_types) {
        mixed array effect = Damage_Type_Effect(type);
        out += ({ effect[Damage_Effect_Style] });
    }
    return sort_array(out, #'>);
}

void set_bestowal_kabatha_resistances(string array types) {
    kabatha_resistance_types = types;
}

mixed array query_bestowal_kabatha_resistances() {
    return kabatha_resistance_types;
}

void bestowal_bestow(object who) {
    who->display(query_bestowal_display());
    funcall(reward_process, who);
    object link = who->query_affiliation_link(project_control());
    if(link) {
        link->travelers_bestowal_bestowed(this_object());
    } else if(type != Travelers_Bestowal_Type_Recurring) {
        mapping aff = who->query_affiliation_info(project_control());
        aff["bestowals"] ||= ({});
        if(member(aff["bestowals"], name) != Null)
            error("attempted to add duplicate bestowal '" + name + "'");
        aff["bestowals"] += ({ name });
    }
}

void bestowal_deliver(object who, mixed what) {
    switch(typeof(what)) {
    case T_OBJECT   :
        break;
    case T_STRING   :
        what = what->query_unique() ? load_object(what) : new(what);
        break;
    default         :
        check_argument(1, what);
    }
    if(what->is_armour())
        what->armour()->perform_adapt(who);
    if(what->move(who) == Move_Succeed) {
        who->message(([
            Message_Content     : ({
                ({ 'a', what }), ({ "materialize", what }), "in", ({ 's', 0, "inventory" }), "in", 'a', self_color(random_element(Travelers_Rainbow), 0, "haze"),
            }),
            Message_Senses      : Message_Sense_Visual,
        ]));
    } else if(environment(who) && what->move(environment(who), Move_Flag_Failure_Destructs) == Move_Succeed) {
        what->message(([
            Message_Content : ({
                ({ 'a', 0 }), ({ "materialize", 0 }), "here in", 'a', self_color(random_element(Travelers_Rainbow), 0, "haze"),
            }),
            Message_Senses  : Message_Sense_Visual,
        ]));
    } else {
        safe_destruct(what);
    }
}

varargs mixed bestowal_find_safe_items(int effect_class, int categories, status include_rarities) {
    mapping out = ([]);
    foreach(descriptor dxr : Daemon_Items->query_items()) {
        if(Item_Query(dxr, Item_Dangers) & ~Item_Danger_Severe_Effect)
            continue;
        if(categories && (Item_Query(dxr, Item_Categories) & categories) != categories)
            continue;
        if(Item_Query(dxr, Item_Effect_Class) != effect_class)
            continue;
        int weight = Rarity(Item_Query(dxr, Item_Rarity))->query_rarity_weight();
        mixed what = Item_Query(dxr, Item_File);
        switch(typeof(what)) {
        case T_STRING   :
            out[what] = weight;
            break;
        case T_POINTER  :
            {
                int sub_weight = 0;
                foreach(string item : what)
                    out[item] = sub_weight ||= max(round(to_float(weight) / sizeof(what)), 1);
            }
            break;
        case T_MAPPING  :
            {
                int total = 0;
                foreach(string item, int sub_weight : what) {
                    total ||= accumulate(values(what));
                    out[item] = max(round(to_float(weight) * sub_weight / total), 1);
                }
            }
        }
    }
    if(include_rarities)
        return out;
    else
        return m_indices(out);
}

float bestowal_query_total_skill_experience(object who) {
    float total_skill_exp = 0.0;
    int array skills = who->query_skills();
    foreach(int skill : skills)
        total_skill_exp += who->query_skill_exp(skill);
    return total_skill_exp;
}
