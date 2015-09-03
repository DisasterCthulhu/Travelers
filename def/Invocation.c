#include <alignment.h>
#include <daemon.h>
#include <services_ooc_information.h>
#include <speech.h>
#include <Travelers.h>

nosave private mapping relevant_skills;
nosave private mixed relevant_skills_total_weight;
private closure invocation_begin_can_call;
private closure invocation_continue_can_call;
private closure invocation_execute_do_call;
private descriptor ability_determinants;
private descriptor array cost;
private descriptor cost_bypass_condition;
private descriptor use_condition;
private int delay;
private int maximum_ability;
private int minimum_ability;
private int rarity;
private mixed help;
private status argument_required;
private string argument;
private string argument_description;
private string bestowal;
private string description;
private string key;
private string lower_case_key;
private string name;
private string summary;

inherit "/std/definition";

void configure() {
	::configure();
	set_broker(Travelers_Daemon("dharma"));
}

void set_invocation_name(string val) {
	name = val;
}

string query_invocation_name() {
	return name;
}

void set_invocation_summary(string val) {
	summary = val;
}

string query_invocation_summary() {
	return summary;
}

void set_invocation_bestowal(string val) {
	bestowal = val;
}

string query_invocation_bestowal() {
	return bestowal;
}

void set_invocation_help(mixed val) {
	help = val;
}

mixed query_invocation_help(object who) {
	if(stringp(help))
		return help;
	else if(closurep(help)) {
		unless(who)
			return help;
		return funcall(help, who);
	} else
		return "no help given";
}

void set_invocation_description(string desc) {
	description = desc;
}

string query_invocation_description() {
	return description || "(no summary given)";
}

void set_invocation_rarity(int val) {
	rarity = val;
}

int query_invocation_rarity() {
	return rarity || Rarity_Rare;
}

void set_invocation_ability_maximum(int max) {
	maximum_ability = max;
}

int query_invocation_ability_maximum() {
	return maximum_ability;
}

void set_invocation_ability_minimum(int min) {
	minimum_ability = min;
}

int query_invocation_ability_minimum() {
	return minimum_ability;
}

void set_invocation_cost(mixed val) {
	descriptor array energy_cost = ({});
	foreach(mixed array energy : val) {
		descriptor new_cost = Energy_Cost(energy);
		Energy_Cost_Set(new_cost, Energy_Cost_Purpose, ({
			"perform the invocation", capitalize_words(query_invocation_name(), True)
		}));
		energy_cost += ({ new_cost });
	}
	cost = energy_cost;
}

descriptor array query_invocation_cost() {
	return cost;
}

void set_invocation_cost_bypass_condition(mixed cond) {
	cost_bypass_condition = Condition(cond);
}

descriptor query_invocation_cost_bypass_condition() {
	return cost_bypass_condition;
}

void set_invocation_ability_determinants(mixed cond) {
	descriptor dxr = Condition(cond);
	ability_determinants = dxr;
}

descriptor query_invocation_ability_determinants() {
	return ability_determinants || Condition(([
		Condition_Skill_Composite           : True,
		Skill_Theurgy                         : 0.50,
		Skill_Invocation                    : 0.50,
		([
			Condition_Type_Code             : Condition_Type_Attribute,
			Condition_Info                  : Attr_Cha,
			Condition_Value                 : 0.75,
		])                                  : True,
		([
			Condition_Type_Code             : Condition_Type_Attribute,
			Condition_Info                  : Attr_Int,
			Condition_Value                 : 0.25,
		])                                  : True,
	]));
}


mapping query_invocation_relevant_skills() {
	return relevant_skills ||= Condition_Relevant_Skills(query_invocation_ability_determinants()) || ([]);
}

private mixed invocation_determine_relevant_skills_total_weight() {
	mixed out = 0;
	foreach(int skill, float weight : query_invocation_relevant_skills())
		out += weight;
	return out;
}

mixed query_invocation_relevant_skills_total_weight() {
	return relevant_skills_total_weight ||= invocation_determine_relevant_skills_total_weight();
}

float query_invocation_ability(object who) {
	float out = Condition_Evaluate(query_invocation_ability_determinants(), who, 0);
	if(query_invocation_ability_maximum())
		out = to_float(min(query_invocation_ability_maximum(), out));
	return out;
}

int query_invocation_sonority(object who) {
	int max = query_invocation_ability_maximum();
	int min = query_invocation_ability_minimum();
	if(!max)
		return 100;
	return scale_conversion(round(query_invocation_ability(who)), min, max, 0, 100);
}

void set_invocation_argument(string arg) {
	argument = arg;
}

string query_invocation_argument() {
	return argument;
}

void set_invocation_argument_description(string arg) {
	argument_description = arg;
}

string query_invocation_argument_description() {
	return argument_description;
}

void set_invocation_argument_required(status req) {
	argument_required = req;
}

status query_invocation_argument_required() {
	return argument_required;
}

void set_invocation_key(string val) {
	key = val;
	lower_case_key = lower_case(val);
}

string query_invocation_key() {
	return key;
}

string query_invocation_lower_case_key() {
	return lower_case_key;
}

void set_invocation_delay(int val) {
	delay = val;
}

int query_invocation_delay() {
	return delay;
}

void set_invocation_use_condition(mixed cond) {
	use_condition = Condition(cond);
}

descriptor query_invocation_use_condition() {
	return use_condition;
}

status query_invocation_known(object who) {
	string bestowal = query_invocation_bestowal();
	if(bestowal) {
		object link = who->query_affiliation_link(project_control());
		if(member(link->travelers_query_bestowals(), bestowal) == Null)
			return False;
	}
	return True;
}

void set_invocation_begin_can_call(closure cl) {
	invocation_begin_can_call = cl;
}

closure query_invocation_begin_can_call() {
	return invocation_begin_can_call;
}

void set_invocation_continue_can_call(closure cl) {
	invocation_continue_can_call = cl;
}

closure query_invocation_continue_can_call() {
	return invocation_continue_can_call;
}

void set_invocation_execute_do_call(closure cl) {
	invocation_execute_do_call = cl;
}

closure query_invocation_execute_do_call() {
	return invocation_execute_do_call;
}

void add_invocation_log_datum(descriptor dxr, mixed arg) {
	string data = Process_Query_Info(dxr, "Travelers_Invocation_Log_Data");
	if(!data) {
		data = printable(arg);
	} else {
		data += ", ";
		data += printable(arg);
	}
	Process_Set_Info(dxr, "Travelers_Invocation_Log_Data", data);     
}

status invocation_complete_do_call(descriptor dxr) {
	object who = Process_Query(dxr, Process_Actor);
	float total_cost = 0.0;
	foreach(descriptor ec : query_invocation_cost())
		total_cost += Energy_Cost_Query(ec, Energy_Cost_Amount) * Energy(Energy_Cost_Query(ec, Energy_Cost_Type))->query_energy_potency();
	mapping skills = query_invocation_relevant_skills();
	float total_prop = query_invocation_relevant_skills_total_weight();
	foreach(int skill, float prop : skills)
		who->add_skill_exp(skill, total_cost * prop / total_prop);
	string log_string = "[" + ctime() + "] " + printable(who) + " performed invocation " + query_invocation_name() + " with " + printable(query_invocation_sonority(who)) + "% sonority";
	string extra = Process_Query_Info(dxr, "Travelers_Invocation_Log_Data");
	if(extra)
		log_string += " with the arguments " + extra;
	log_file("dev/twilight.travelers_powers", log_string);
	return True;
}

status invocation_begin_do_call(descriptor dxr) {
	object who = Process_Query(dxr, Process_Actor);
	descriptor bypass_cond = query_invocation_cost_bypass_condition();
	unless(bypass_cond && Condition_Apply(bypass_cond, who, 0))
		Energy_Costs_Assert(query_invocation_cost(), who);
	return True;
}

status invocation_continue_can_call(descriptor dxr) {
	object who = Process_Query(dxr, Process_Actor);
	if(query_any_realm("NM", environment(who))) {
		Process_Set_Info(dxr, "reason", ({ "the currents of magick have stopped flowing through", ({ 'o', who }) }));
		return False;
	}
	mixed res;
	if(invocation_continue_can_call) {
		res = funcall(invocation_continue_can_call, dxr);
		if(res != True) {
			Process_Set_Info(dxr, "reason", res);
			return False;
		}
	}
	return True;
}

void invocation_continue_fail_call(descriptor dxr) {
	object who = Process_Query(dxr, Process_Actor);
	mixed array reason = Process_Query_Info(dxr, "reason") || ({ "" });
	who->interrupt_current_process(reason, Process_Interrupt_Failure);
}

status invocation_begin_can_call(descriptor dxr) {
	object who = Process_Query(dxr, Process_Actor);
	descriptor use_cond = query_invocation_use_condition();
	if((use_cond && !Condition_Apply(use_cond, who, 0))) {
		return Error(({
			({ 's', who, "circumstances" }), "do not permit", ({ 'p', 0 }), "to attempt this invocation"
		}));
	}
	if(who->query_alignment(Alignment_Order_Chaos) < Travelers_Minimum_Orderliness) {
		return Error(({
			who, ({ "sense", who }), "that", ({ 'r', who, "ability to entreat Ganesha" }), "has been compromised by",
			"ignoring dharma",
		}));
	}
	if(who->query_alignment(Alignment_Good_Evil) < Travelers_Minimum_Goodness) {
		return Error(({
			who, ({ "sense", who }), "that", ({ 'r', who, "ability to entreat Ganesha" }), "has been compromised by",
			"ignoring karma",
		}));
	}
	if(query_any_realm("NM", environment(who))) {
		return Error(({
			"something is preventing", ({ 's', who, "invocation" }), "from reaching Ganesha"
		}));
	}
	if(invocation_begin_can_call) {
		mixed res = funcall(invocation_begin_can_call, dxr);
		if(Result_Failure(res))
			return res;
	}
	descriptor bypass_cond = query_invocation_cost_bypass_condition();
	unless(bypass_cond && Condition_Apply(bypass_cond, who, 0)) {
		mixed res = Energy_Cost_Check_List(query_invocation_cost(), who);
		if(Result_Failure(res))
			return Error(({ res }));
	}
	return True;
}

void invocation_continue_do_call(descriptor dxr) {
	object who = Process_Query(dxr, Process_Actor);
	int count = Process_Query(dxr, Process_Continue_Count);
	if(count == query_invocation_delay() / 2) {
		funcall(invocation_execute_do_call, dxr);
		who->complete_process(dxr);
	} else {
		if(environment(who)) {
			who->message(([
				Message_Content             : ({
					0, ({ "concentrate", 0 }),
				}),
				Message_Flags               : Message_Flag_Exclude_Source,
				Message_Senses              : Message_Sense_Visual,
			]));
		}
		who->display(([
			Message_Content                 : ({
				0, ({ "concentrate", 0 }), "and", ({ "let", 0 }), "Ganesha's influence continue to build within", ({ 'o', 0 }),
			}),
			Message_Senses                  : Message_Sense_Spiritual | Message_Sense_Cognitive,
		]));
	}
}

status perform_invocation(mapping args) {
	object who = args["who"];
	descriptor speech = args["speech"];
	descriptor array manipulators = Speech_Query(speech, Speech_Manipulators);
	string array manipulator_names = map(manipulators, (: Manipulator_Query($1, Manipulator_Name) :));
	string verb = Speech_Query(speech, Speech_Verb);
	mixed res = who->begin_process(([
		Process_Description                 : ({ "performing the invocation " + query_invocation_name() }),
		Process_Begin_Message               : Message(([
			Message_Content                 : ({
				([
					Message_Content         : ({
						self_color("iridescent", "tiny", "motes of light"), "surround", ({ 's', who, list_array(manipulator_names) }),
						"as", ({ 'p', manipulator_names }), ({ "give", manipulator_names}), "form to the words",
					}),
					Message_Senses          : Message_Sense_Astral,
				]),
				([
					Message_Content         : ({
						";",
					}),
					Message_Senses          : Message_Sense_Astral,
					Message_Flags           : Message_Flag_Source_Only,
				]),
				([
					Message_Content         : ({
						who, ({ "feel", who }), "Ganesha's influence begin to flow through", ({ 'o', who }),
					}),
					Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
					Message_Flags           : Message_Flag_Source_Only,
				]),
			}),
		])),
		Process_Interrupt_Message           : Message(([
			Message_Content                 : ({
				([
					Message_Content         : ({
						"the", self_color("iridescent", "twinkles of", "light"), "dancing on", ({ 's', who, list_array(manipulator_names) }),
						"vanish abruptly"
					}),
					Message_Senses          : Message_Sense_Astral,
				]),
				([
					Message_Content         : ({
						"as",
					}),
					Message_Senses          : Message_Sense_Astral,
					Message_Flags           : Message_Flag_Source_Only,
				]),
				([
					Message_Content         : ({
						who, ({ "lose", who }), "concentration and Ganesha's influence withdraws from", ({ 'o', who }),
					}),
					Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
					Message_Flags           : Message_Flag_Source_Only,
				]),
			}),
		])),
		Process_Priority                    : Process_Priority_High,
		Process_Begin_Can_Call              : #'invocation_begin_can_call,
		Process_Begin_Do_Call               : #'invocation_begin_do_call,
		Process_Continue_Can_Call           : #'invocation_continue_can_call,
		Process_Continue_Do_Call            : #'invocation_continue_do_call,
		Process_Continue_Fail_Call          : #'invocation_continue_fail_call,
		Process_Complete_Do_Call            : #'invocation_complete_do_call,
		Process_Interrupts                  : Process_Interrupts_All_Except(Process_Interrupt_Blindness),
		Process_Interrupts_Resistable       : Process_Interrupt_Combat | Process_Resistable_Interrupts_Sensations | Process_Resistable_Interrupts_Emotions,
		Process_Info                        : ([
			"Ganesha_Invocation_Process"    : True,
			"ability"                       : query_invocation_ability(who),
			"sonority"                      : query_invocation_sonority(who),
			"argument"                      : args["argument"],
			"language"                      : Speech_Query(speech, Speech_Language),
			"comprehension"                 : Speech_Query(speech, Speech_Speaker_Comprehension),
		]),
	]));
	if(Hook_Failure(res)) {
		if(Hook_Display(res))
			return notify_fail(res);
		else
			return notify_fail(({
				who, ({ "cannot", who }), "do that",
			}));
	}
	return True;
}

string get_invocation_help(object who, string what) {
	if(who && !query_invocation_known(who))
		return 0;
	string out = "";
	out += "{{layout: title}the invocation {{yellow}" + capitalize_words(name, True) + "}}\n";
	if(query_invocation_use_condition())
		out += "\n{{layout: label}Requirements              :} {{indent +4!}" + Condition_Describe(query_invocation_use_condition(), who, Condition_Describe_Flag_Infinitive) + "}";
    int max = query_invocation_ability_maximum();
    if(max) {
	    out += "\n{{layout: label}Ability Determinants      :} {{indent +4!}" + Condition_Describe(query_invocation_ability_determinants(), who, Condition_Describe_Flag_Evaluate | Condition_Describe_Flag_Infinitive) + "}";
	    int min = query_invocation_ability_minimum();
	    if(min)
	        out += "\n{{layout: label}Minimum Effective Ability :} {{indent +4!}" + printable(min) + "}";
	    out += "\n{{layout: label}Maximum Effective Ability :} {{indent +4!}" + printable(max) + "}";
	    out += "\n{{layout: label}Your Ability              :} {{indent +4!}" + printable(query_invocation_ability(who)) + "}";
	    out += "\n{{layout: label}Sonority                  :} {{indent +4!}" + printable(query_invocation_sonority(who)) + "}";
	}
	string array energy_list = ({});
	out += "\n{{layout: label}Energy Costs              :} {{indent +4!}";
	foreach(descriptor energy_cost : query_invocation_cost())
		energy_list += ({
			things("point", Energy_Cost_Query(energy_cost, Energy_Cost_Amount)) + " of " + Energy(Energy_Cost_Query(energy_cost, Energy_Cost_Type))->query_energy_designation()
		});
	out += list_array(energy_list) + "}";
	string key = query_invocation_key();
	string arg = query_invocation_argument();
	if(arg) {
		if(!query_invocation_argument_description()) {
			arg = replace(arg, "%s", "<target>");
			arg = replace(arg, "%d", "<amount>");
		} else {
			arg = query_invocation_argument_description();
		}
		arg = regreplace(arg, "\<[^<>]*\>", "{{layout: deemphasis}&}", 1);
		if(query_invocation_argument_required())
			out += "\n{{layout: label}Invocation Mantra         :} {{indent +4!}" + key + " " + arg + "}";
		else
			out += "\n{{layout: label}Invocation Mantras        :}\n{{indent +4!}" + key + "}\n{{indent +8!}" + key + " " + arg + "}";
	} else {
		out += "\n{{layout: label}Invocation Mantra         :} {{indent +4!}" + key + "}";
	}
	out += "\n";
	out += "\f" + query_invocation_help(who);
	out += "\n";
	out += "\n{{interface: contrast}Development Information   :} {{indent +4!}" + implode(map(Daemon_Ooc_Information->query_general_ooc_information(this_object(), who), func(who, "message_text")), "  ") + "}";
	return out;
}

string query_custom_info_type() {
	return "invocation";
}
