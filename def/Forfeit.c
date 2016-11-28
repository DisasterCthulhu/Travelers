#include <Travelers.h>
#include <daemon.h>

private mapping forfeit;
private closure forfeit_process;

inherit Travelers_Definition("Challenge_Component");

void preinit() {
    ::preinit();
    forfeit ||= ([]);
}

void configure() {
    ::configure();
    set_broker(Travelers_Daemon("dharma"));
}

void set_forfeit_name(string name) {
    forfeit["name"] = name;
}

string query_forfeit_name() {
    return forfeit["name"];
}

string query_challenge_component_name() {
    return forfeit["name"];
}

void set_forfeit_process(closure cl) {
    forfeit_process = cl;
}

closure query_forfeit_process() {
    return forfeit_process;
}

void set_forfeit_rarity(int rarity) {
    forfeit["rarity"] = rarity;
}

int query_forfeit_rarity() {
    return forfeit["rarity"];
}

void set_forfeit_value(int value) {
    forfeit["value"] = value;
}

int query_forfeit_value() {
    return forfeit["value"];
}

void set_forfeit_message(mixed message) {
    forfeit["message"] = Message(message);
}

descriptor query_forfeit_message() {
    return forfeit["message"];
}

void set_forfeit_display(mixed display) {
    forfeit["display"] = Message(display);
}

descriptor query_forfeit_display() {
    return forfeit["display"];
}

void set_forfeit_initialize_description(string description) {
    forfeit["initialize_description"] = description;
}

string query_forfeit_initialize_description() {
    return forfeit["initialize_description"];
}

void set_forfeit_overcome_description(string description) {
    forfeit["overcome_description"] = description;
}

string query_forfeit_overcome_description() {
    return forfeit["overcome_description"];
}

void set_forfeit_initialize_display(mixed display) {
    forfeit["initialize_display"] = Message(display);
}

descriptor query_forfeit_initialize_display() {
    if(forfeit["initialize_display"])
        return forfeit["initialize_display"];
    else if(forfeit["initialize_description"])
        return Message(([
            Message_Content         : ({
                0, ({ "sense", 0 }), "that if", ({ 'p', 0 }), ({ "violate", 0 }), "this taboo,", ({ 'r', 0, "forfeit" }), "will be",
                forfeit["initialize_description"]
            }),
            Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
            Message_Color           : "status: risk",
        ]));
}

void forfeit_initialize(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    descriptor display = query_forfeit_initialize_display();
    if(display)
        who->display(display);
}

void forfeit_fail(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    who->display(([
        Message_Content                         : ({
            0, ({ "sense", 0 }), "the forfeit of", forfeit["name"], "being met upon", ({ 'o', 0 }),
        }),
        Message_Color                           : "alert: high",
        Message_Senses                          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
    if(forfeit["message"])
        who->message(forfeit["message"]);
    if(forfeit["display"])
        who->display(forfeit["display"]);
    funcall(forfeit_process, obj);
}

void set_forfeit_eligibility_condition(mixed cond) {
    descriptor use;
    if(Is_Condition(cond))
        use = cond;
    else
        use = Condition(cond);
    forfeit["eligibility_condition"] = use;
}

descriptor query_forfeit_eligibility_condition() {
    return forfeit["eligibility_condition"];
}

status query_forfeit_eligibility(object who) {
    descriptor cond = query_forfeit_eligibility_condition();
    unless(cond)
        return True;
    return Condition_Apply(cond, who, 0);
}
