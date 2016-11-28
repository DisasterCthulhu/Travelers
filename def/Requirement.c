#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Challenge_Component");

nosave private string modifier_tag;
private closure attach_process;
private closure detach_process;
private closure initialize_process;
private closure overcome_process;
private descriptor eligibility_condition;
private descriptor initialize_display;
private descriptor overcome_display;
private int rarity;
private int value;
private mapping hooks;
private status suspendable;
private string initialize_description;
private string name;
private string overcome_description;
private string array bonus_bestowals;

void configure() {
    ::configure();
    set_broker(Travelers_Daemon("dharma"));
}

protected nomask void set_requirement_name(string val) {
    name = val;
}

nomask string query_requirement_name() {
    return name;
}

nomask string query_challenge_component_name() {
    return name;
}

protected nomask void set_requirement_rarity(int val) {
    rarity = val;
}

nomask int query_requirement_rarity() {
    return rarity;
}

protected nomask void set_requirement_value(int val) {
    value = val;
}

nomask int query_requirement_value() {
    return value;
}

protected nomask void set_requirement_initialize_description(string val) {
    initialize_description = val;
}

nomask string query_requirement_initialize_description() {
    return initialize_description;
}

protected nomask void set_requirement_suspendable(status val) {
    suspendable = val;
}

nomask status query_requirement_suspendable() {
    return suspendable;
}

protected nomask void set_requirement_bonus_bestowals(string array val) {
    bonus_bestowals = val;
}

nomask string array query_requirement_bonus_bestowals() {
    return bonus_bestowals;
}

protected nomask void set_requirement_overcome_description(string val) {
    overcome_description = val;
}

nomask string query_requirement_overcome_description() {
    return overcome_description;
}

protected nomask void set_requirement_initialize_display(mixed val) {
    initialize_display = val && Message(val);
}

nomask descriptor query_requirement_initialize_display() {
    if(initialize_display)
        return initialize_display;
    if(initialize_description)
        return initialize_display = Message(([
            Message_Content         : ({
                0, ({ "sense", 0 }), "that this is an obstacle, a challenge provided by Ganesha,",
                "that can be overcome by", initialize_description,
            }),
            Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
            Message_Color           : "status: risk",
        ]));
    return 0;
}

protected nomask void set_requirement_overcome_display(mixed val) {
    overcome_display = val && Message(val);
}

nomask descriptor query_requirement_overcome_display() {
    if(overcome_display)
        return overcome_display;
    if(overcome_description)
        return overcome_display = Message(([
            Message_Content         : ({
                0, ({ "sense", 0 }), "that", ({ 'p', 0 }), ({ "have", 0 }), overcome_description,
                "to overcome Ganesha's obstacle",
            }),
            Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
            Message_Color           : "status: safety",
        ]));
    return 0;
}

protected nomask void set_requirement_attach_process(closure cl) {
    attach_process = cl;
}

nomask closure query_requirement_attach_process() {
    return attach_process;
}

protected nomask void set_requirement_detach_process(closure cl) {
    detach_process = cl;
}

nomask closure query_requirement_detach_process() {
    return detach_process;
}

protected nomask void set_requirement_overcome_process(closure cl) {
    overcome_process = cl;
}

nomask closure query_requirement_overcome_process() {
    return overcome_process;
}

protected nomask void set_requirement_initialize_process(closure cl) {
    initialize_process = cl;
}

nomask closure query_requirement_initialize_process() {
    return initialize_process;
}

nomask string query_requirement_modifier_tag() {
    return modifier_tag ||= "Ganesha_Requirement_" + replace(capitalize_words(query_requirement_name()), " ", "_") + "_Mod";
}

nomask void add_requirement_hook(int type, closure cl) {
    hooks ||= ([]);
    array_push(hooks[type], cl);
}

nomask mapping query_requirement_hooks() {
    return hooks;
}

nomask void requirement_engage_hooks(object who) {
    if(hooks)
        foreach(int type, mixed array cls : hooks)
            foreach(closure cl : cls)
                Call_Hook_On(who, type, cl, Call_Flag_Nonpersistent);
}

nomask void requirement_disengage_hooks(object who) {
    if(hooks)
        foreach(int type, mixed array cls : hooks)
            foreach(closure cl : cls)
                Call_Hook_Off(who, type, cl, Call_Flag_Nonpersistent);
}

nomask void requirement_attach(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    funcall(attach_process, obj);
    requirement_engage_hooks(who);
}

nomask void requirement_detach(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    funcall(detach_process, obj);
    requirement_disengage_hooks(who);
}

// temporary compat, if you're reading this delete this function
nomask void detach_process(object obj) {
    requirement_detach(obj);
}

nomask void requirement_initialize(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    funcall(initialize_process, obj);
    descriptor display = query_requirement_initialize_display();
    if(display)
        who->display(display);
}

nomask void requirement_overcome(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    funcall(overcome_process, obj);
    descriptor display = query_requirement_overcome_display();
    if(display)
        who->display(display);
}

nomask void requirement_yield(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    funcall(overcome_process, obj);
}

nomask void requirement_fail(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    funcall(overcome_process, obj);
}

protected nomask void set_requirement_eligibility_condition(mixed cond) {
    if(!Is_Condition(cond))
        cond = Condition(cond);
    eligibility_condition = cond;
}

nomask descriptor query_requirement_eligibility_condition() {
    return eligibility_condition;
}

nomask status query_requirement_eligibility(object who) {
    descriptor cond = query_requirement_eligibility_condition();
    return !cond || Condition_Apply(cond, who, 0);
}

nomask void requirement_check_notification(object challenge, mixed current_value, string low_value, string high_value) {
    object who = challenge->ganesha_challenge_query_owner();
    mixed low = low_value ? challenge->query_info(low_value) : 0;
    mixed high = challenge->query_info(high_value);
    int last = challenge->query_info("Last_Requirement_Notification");
    int current = round(scale_conversion(current_value, low, high, 0, 100));
    current = min(current, 99);
    if(current > last + 5) {
        challenge->set_info("Last_Requirement_Notification", current);
        who->display(([
            Message_Content         : ({
                0, ({ "sense", 0 }), "that you have completed " + printable(current) + "% of", ({ 'r', 0, "challenge" }),
            }),
            Message_Senses          : Message_Sense_Cognitive,
            Message_Color           : "status: safety",
        ]));
    }
}
