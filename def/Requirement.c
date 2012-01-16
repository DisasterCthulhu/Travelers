#include <Travelers.h>
#include <daemon.h>

private mapping requirement;
private closure requirement_attach;
private closure requirement_detach;
private closure requirement_overcome;
private closure requirement_initialize;

inherit Travelers_Definition("Challenge_Component");

void preinit() {
	::preinit();
	requirement ||= ([]);
}

void configure() {
	::configure();
	set_broker(Travelers_Daemon("dharma"));
}

void set_requirement_name(string name) {
	requirement["name"] = name;
}

string query_requirement_name() {
	return requirement["name"];
}

string query_challenge_component_name() {
    return requirement["name"];
}

void set_requirement_rarity(int rarity) {
	requirement["rarity"] = rarity;
}

int query_requirement_rarity() {
	return requirement["rarity"];
}

void set_requirement_value(int value) {
	requirement["value"] = value;
}

int query_requirement_value() {
	return requirement["value"];
}

void set_requirement_initialize_description(string description) {
	requirement["initialize_description"] = description;
}

string query_requirement_initialize_description() {
	return requirement["initialize_description"];
}

void set_requirement_overcome_description(string description) {
	requirement["overcome_description"] = description;
}

string query_requirement_overcome_description() {
	return requirement["overcome_description"];
}

void set_requirement_initialize_display(mixed display) {
	requirement["initialize_display"] = Message(display);
}

descriptor query_requirement_initialize_display() {
	if(requirement["initialize_display"])
		return requirement["initialize_display"];
	else if(requirement["initialize_description"])
		return Message(([
			Message_Content         : ({
				0, ({ "sense", 0 }), "that this is an obstacle, a challenge provided by Ganesha,",
				"that can be overcome by", requirement["initialize_description"],
			}),
			Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
			Message_Color           : "status: risk",
		]));
}

void set_requirement_overcome_display(mixed display) {
	requirement["overcome_display"] = Message(display);
}

descriptor query_requirement_overcome_display() {
	if(requirement["overcome_display"])
		return requirement["overcome_display"];
	else if(requirement["overcome_description"])
		return Message(([
			Message_Content         : ({
				0, ({ "sense", 0 }), "that", ({ 'p', 0 }), ({ "have", 0 }), requirement["overcome_description"],
				"to overcome Ganesha's obstacle",
			}),
			Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
			Message_Color           : "status: safety",
		]));
}

void set_requirement_attach_process(closure cl) {
	requirement_attach = cl;
}

closure query_requirement_attach_process() {
	return requirement_attach;
}

void set_requirement_detach_process(closure cl) {
	requirement_detach = cl;
}

closure query_requirement_detach_process() {
	return requirement_detach;
}

void set_requirement_overcome_process(closure cl) {
	requirement_overcome = cl;
}

closure query_requirement_overcome_process() {
	return requirement_overcome;
}

void set_requirement_initialize_process(closure cl) {
	requirement_initialize = cl;
}

closure query_requirement_initialize_process() {
	return requirement_initialize;
}

string query_requirement_modifier_tag() {
	return "Ganesha_Requirement_" + replace(capitalize_words(query_requirement_name()), " ", "_") + "_Mod";
}

descriptor set_requirement_modifier_tag(descriptor dxr) {
	Modifier_Set_Info(dxr, query_requirement_modifier_tag(), True);
}

status has_requirement_modifier_tag(descriptor dxr) {
	return Modifier_Query_Info(dxr, query_requirement_modifier_tag());
}

void add_requirement_hook(int type, closure cl) {
	requirement["hooks"] ||= ([]);
	requirement["hooks"][type] ||= ({});
	requirement["hooks"][type] += ({ cl });
}

mapping query_requirement_hooks() {
	return requirement["hooks"] || ([]);
}

void engage_requirement_hooks(object who) {
	foreach(int type, mixed array cls : query_requirement_hooks())
		foreach(closure cl : cls)
			Call_Hook_On(who, type, cl, Call_Flag_Nonpersistent);
}

void disengage_requirement_hooks(object who) {
	foreach(int type, mixed array cls : query_requirement_hooks())
		foreach(closure cl : cls)
			Call_Hook_Off(who, type, cl, Call_Flag_Nonpersistent);
}

void requirement_attach(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	if(requirement_attach)
		funcall(requirement_attach, obj);
	engage_requirement_hooks(who);
}

void requirement_detach(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	if(requirement_detach)
		funcall(requirement_detach, obj);
	disengage_requirement_hooks(who);
}

void requirement_initialize(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	if(requirement_initialize)
		funcall(requirement_initialize, obj);
	descriptor display = query_requirement_initialize_display();
	if(display)
		who->display(display);
}

void requirement_overcome(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	if(requirement_overcome)
		funcall(requirement_overcome, obj);
	descriptor display = query_requirement_overcome_display();
	if(display)
		who->display(display);
}

void requirement_yield(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	if(requirement_overcome)
		funcall(requirement_overcome, obj);
}

void requirement_fail(object obj) {
}

void set_requirement_eligibility_condition(mixed cond) {
	descriptor use;
	if(Is_Condition(cond))
		use = cond;
	else
		use = Condition(cond);
	requirement["eligibility_condition"] = use;
}

descriptor query_requirement_eligibility_condition() {
	return requirement["eligibility_condition"];
}

status query_requirement_eligibility(object who) {
	descriptor cond = query_requirement_eligibility_condition();
	unless(cond)
		return True;
	return Condition_Apply(cond, who, 0);
}

void requirement_check_notification(object challenge, mixed current_value, string low_value, string high_value) {
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
				'j', "{{status: safety}",
				0, ({ "sense", 0 }), "that you have completed " + printable(current) + "% of", 'j', ({ 'r', 0, "challenge" }),
				"}"
			}),
			Message_Senses          : Message_Sense_Cognitive,
		]));
	}
}

void taboo_violation(object who) {
    object challenge = Travelers_Find_Challenge(who);
    challenge->ganesha_challenge_fail();
}
