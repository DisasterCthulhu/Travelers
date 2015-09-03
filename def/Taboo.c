#include <Travelers.h>
#include <daemon.h>

nosave private string modifier_tag;
private closure attach_process;
private closure detach_process;
private descriptor eligibility_condition;
private descriptor initialize_display;
private descriptor overcome_display;
private int rarity;
private int value;
private mapping hooks;
private string initialize_description;
private string name;
private string overcome_description;

inherit Travelers_Definition("Challenge_Component");

void configure() {
	::configure();
	set_broker(Travelers_Daemon("dharma"));
}

protected nomask void set_taboo_name(string val) {
	name = val;
}

nomask string query_taboo_name() {
	return name;
}

nomask string query_challenge_component_name() {
    return name;
}

protected nomask void set_taboo_rarity(int val) {
	rarity = val;
}

nomask int query_taboo_rarity() {
	return rarity;
}

protected nomask void set_taboo_value(int val) {
	value = val;
}

nomask int query_taboo_value() {
	return value;
}

protected nomask void set_taboo_initialize_description(string val) {
	initialize_description = val;
}

nomask string query_taboo_initialize_description() {
	return initialize_description;
}

protected nomask void set_taboo_overcome_description(string val) {
	overcome_description = val;
}

nomask string query_taboo_overcome_description() {
	return overcome_description;
}

protected nomask void set_taboo_initialize_display(mixed val) {
	initialize_display = val && Message(val);
}

nomask descriptor query_taboo_initialize_display() {
	if(initialize_display)
		return initialize_display;
	if(initialize_description)
		return initialize_display = Message(([
			Message_Content         : ({
				0, ({ "sense", 0 }), "that this obstacle has a taboo against", initialize_description,
			}),
			Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
			Message_Color           : "status: risk",
		]));
	return 0;
}

protected nomask void set_taboo_overcome_display(mixed val) {
	overcome_display = val && Message(val);
}

nomask descriptor query_taboo_overcome_display() {
	if(overcome_display)
		return overcome_display;
	if(overcome_description)
		return overcome_display = Message(([
			Message_Content         : ({
				0, ({ "sense", 0 }), "the taboo against", overcome_description, "being lifted from", ({ 'o', 0 }),
			}),
			Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
			Message_Color           : "status: safety",
		]));
	return 0;
}

protected nomask void set_taboo_attach_process(closure cl) {
	attach_process = cl;
}

nomask closure query_taboo_attach_process() {
	return attach_process;
}

protected nomask void set_taboo_detach_process(closure cl) {
	detach_process = cl;
}

nomask closure query_taboo_detach_process() {
	return detach_process;
}

nomask string query_taboo_modifier_tag() {
	return modifier_tag ||= "Ganesha_Taboo_" + replace(capitalize_words(query_taboo_name()), " ", "_") + "_Mod";
}

protected nomask void set_taboo_eligibility_condition(mixed cond) {
	descriptor use;
	if(Is_Condition(cond))
		use = cond;
	else
		use = Condition(cond);
	eligibility_condition = use;
}

nomask descriptor query_taboo_eligibility_condition() {
	return eligibility_condition;
}

protected nomask void add_taboo_hook(int type, closure cl) {
	hooks ||= ([]);
	array_push(hooks[type], cl);
}

nomask mapping query_taboo_hooks() {
	return hooks;
}

protected nomask void taboo_engage_hooks(object who) {
	if(hooks)
		foreach(int type, mixed array cls : hooks)
			foreach(closure cl : cls)
				Call_Hook_On(who, type, cl, Call_Flag_Nonpersistent);
}

protected nomask void taboo_disengage_hooks(object who) {
	if(hooks)
		foreach(int type, mixed array cls : hooks)
			foreach(closure cl : cls)
				Call_Hook_Off(who, type, cl, Call_Flag_Nonpersistent);
}

nomask void taboo_attach(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	funcall(attach_process, obj);
	taboo_engage_hooks(who);
}

nomask void taboo_detach(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	funcall(detach_process, obj);
	taboo_disengage_hooks(who);
}

nomask void taboo_initialize(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	descriptor display = query_taboo_initialize_display();
	if(display)
		who->display(display);
}

nomask void taboo_overcome(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	descriptor display = query_taboo_overcome_display();
	if(display)
		who->display(display);
}

nomask void taboo_yield(object obj) {
	taboo_overcome(obj);
}

nomask void taboo_fail(object obj) {
}

nomask status query_taboo_eligibility(object who) {
	descriptor cond = query_taboo_eligibility_condition();
	unless(cond)
		return True;
	return Condition_Apply(cond, who, 0);
}

nomask void taboo_violation(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    who->display(([
        Message_Content                         : ({
            0, ({ "sense", 0 }), "that", ({ 'p', 0 }), ({ "have", 0 }), "violated the taboo against",
            overcome_description, "placed on", ({ 'o', 0 }), "by Ganesha and", ({ "have", 0 }), "perforce failed his challenge",
        }),
        Message_Color                           : "alert: high",
        Message_Senses                          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
    obj->ganesha_challenge_fail();
}
