#include <Travelers.h>
#include <daemon.h>

private mapping taboo;
private closure taboo_attach;
private closure taboo_detach;

inherit Travelers_Definition("Challenge_Component");

void preinit() {
	::preinit();
	taboo ||= ([]);
}

void configure() {
	::configure();
	set_broker(Travelers_Daemon("dharma"));
}

void set_taboo_name(string name) {
	taboo["name"] = name;
}

string query_taboo_name() {
	return taboo["name"];
}

string query_challenge_component_name() {
    return taboo["name"];
}

void set_taboo_rarity(int rarity) {
	taboo["rarity"] = rarity;
}

int query_taboo_rarity() {
	return taboo["rarity"];
}

void set_taboo_value(int value) {
	taboo["value"] = value;
}

int query_taboo_value() {
	return taboo["value"];
}

void set_taboo_initialize_description(string description) {
	taboo["initialize_description"] = description;
}

string query_taboo_initialize_description() {
	return taboo["initialize_description"];
}

void set_taboo_overcome_description(string description) {
	taboo["overcome_description"] = description;
}

string query_taboo_overcome_description() {
	return taboo["overcome_description"];
}

void set_taboo_initialize_display(mixed display) {
	taboo["initialize_display"] = Message(display);
}

descriptor query_taboo_initialize_display() {
	if(taboo["initialize_display"])
		return taboo["initialize_display"];
	else if(taboo["initialize_description"])
		return Message(([
			Message_Content         : ({
				0, ({ "sense", 0 }), "that this obstacle has a taboo against",
				taboo["initialize_description"]
			}),
			Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
			Message_Color           : "status: risk",
		]));
}

void set_taboo_overcome_display(mixed display) {
	taboo["overcome_display"] = Message(display);
}

descriptor query_taboo_overcome_display() {
	if(taboo["overcome_display"])
		return taboo["overcome_display"];
	else if(taboo["overcome_description"])
		return Message(([
			Message_Content         : ({
				0, ({ "sense", 0 }), "the taboo against", taboo["name"], "being lifted from", ({ 'o', 0 }),
			}),
			Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
			Message_Color           : "status: safety",
		]));
}

void set_taboo_attach_process(closure cl) {
	taboo_attach = cl;
}

closure query_taboo_attach_process() {
	return taboo_attach;
}

void set_taboo_detach_process(closure cl) {
	taboo_detach = cl;
}

closure query_taboo_detach_process() {
	return taboo_detach;
}

string query_taboo_modifier_tag() {
	return "Ganesha_Taboo_" + replace(capitalize_words(query_taboo_name()), " ", "_") + "_Mod";
}

descriptor set_taboo_modifier_tag(descriptor dxr) {
	Modifier_Set_Info(dxr, query_taboo_modifier_tag(), True);
}

status has_taboo_modifier_tag(descriptor dxr) {
	return Modifier_Query_Info(dxr, query_taboo_modifier_tag());
}

void add_taboo_hook(int type, closure cl) {
	taboo["hooks"] ||= ([]);
	taboo["hooks"][type] ||= ({});
	taboo["hooks"][type] += ({ cl });
}

mapping query_taboo_hooks() {
	return taboo["hooks"] || ([]);
}

void engage_taboo_hooks(object who) {
	foreach(int type, mixed array cls : query_taboo_hooks())
		foreach(closure cl : cls)
			Call_Hook_On(who, type, cl, Call_Flag_Nonpersistent);
}

void disengage_taboo_hooks(object who) {
	foreach(int type, mixed array cls : query_taboo_hooks())
		foreach(closure cl : cls)
			Call_Hook_Off(who, type, cl, Call_Flag_Nonpersistent);
}

void taboo_attach(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	if(taboo_attach)
		funcall(taboo_attach, obj);
	engage_taboo_hooks(who);
}

void taboo_detach(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	if(taboo_detach)
		funcall(taboo_detach, obj);
	disengage_taboo_hooks(who);
}

void taboo_initialize(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	descriptor display = query_taboo_initialize_display();
	if(display)
		who->display(display);
}

void taboo_overcome(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	descriptor display = query_taboo_overcome_display();
	if(display)
		who->display(display);
}

void taboo_yield(object obj) {
	taboo_overcome(obj);
}

void taboo_fail(object obj) {
}

void set_taboo_eligibility_condition(mixed cond) {
	descriptor use;
	if(Is_Condition(cond))
		use = cond;
	else
		use = Condition(cond);
	taboo["eligibility_condition"] = use;
}

descriptor query_taboo_eligibility_condition() {
	return taboo["eligibility_condition"];
}

status query_taboo_eligibility(object who) {
	descriptor cond = query_taboo_eligibility_condition();
	unless(cond)
		return True;
	return Condition_Apply(cond, who, 0);
}

void taboo_violation(object obj) {
    object who = obj->ganesha_challenge_query_owner();
    who->display(([
        Message_Content                         : ({
            0, ({ "sense", 0 }), "that", ({ 'p', 0 }), ({ "have", 0 }), "violated the taboo against",
            taboo["name"], "placed on", ({ 'o', 0 }), "by Ganesha and", ({ "have", 0 }), "perforce failed his challenge",
        }),
        Message_Color                           : "alert: high",
        Message_Senses                          : Message_Sense_Spiritual | Message_Sense_Cognitive,
    ]));
    obj->ganesha_challenge_fail();
}
