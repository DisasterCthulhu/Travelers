#include <Travelers.h>
#include <daemon.h>

private mapping obstacle;
private closure obstacle_attach;
private closure obstacle_detach;
private closure obstacle_overcome;
private closure obstacle_initialize;

inherit Travelers_Definition("Challenge_Component");

void preinit() {
	::preinit();
	obstacle ||= ([]);
}

void configure() {
	::configure();
	set_broker(Travelers_Daemon("dharma"));
}

void set_obstacle_name(string name) {
	obstacle["name"] = name;
}

string query_obstacle_name() {
	return obstacle["name"];
}

string query_challenge_component_name() {
    return obstacle["name"];
}

void set_obstacle_prepend_indefinite_article(status article) {
	obstacle["article"] = article;
}

status query_obstacle_prepend_indefinite_article() {
	return obstacle["article"];
}

void set_obstacle_rarity(int rarity) {
	obstacle["rarity"] = rarity;
}

int query_obstacle_rarity() {
	return obstacle["rarity"];
}

void set_obstacle_value(int value) {
	obstacle["value"] = value;
}

int query_obstacle_value() {
	return obstacle["value"];
}

void set_obstacle_initialize_display(mixed display) {
	obstacle["initialize_display"] = Message(display);
}

descriptor query_obstacle_initialize_display() {
	return obstacle["initialize_display"];
}

void set_obstacle_overcome_display(mixed display) {
	obstacle["overcome_display"] = Message(display);
}

descriptor query_obstacle_overcome_display() {
	return obstacle["overcome_display"];
}

void set_obstacle_attach_process(closure cl) {
	obstacle_attach = cl;
}

closure query_obstacle_attach_process() {
	return obstacle_attach;
}

void set_obstacle_detach_process(closure cl) {
	obstacle_detach = cl;
}

closure query_obstacle_detach_process() {
	return obstacle_detach;
}

void set_obstacle_overcome_process(closure cl) {
	obstacle_overcome = cl;
}

closure query_obstacle_overcome_process() {
	return obstacle_overcome;
}

void set_obstacle_initialize_process(closure cl) {
	obstacle_initialize = cl;
}

closure query_obstacle_initialize_process() {
	return obstacle_initialize;
}

void add_obstacle_hook(int type, closure cl) {
	obstacle["hooks"] ||= ([]);
	obstacle["hooks"][type] ||= ({});
	obstacle["hooks"][type] += ({ cl });
}

mapping query_obstacle_hooks() {
	return obstacle["hooks"] || ([]);
}

void engage_obstacle_hooks(object who) {
	foreach(int type, mixed array cls : query_obstacle_hooks())
		foreach(closure cl : cls)
			Call_Hook_On(who, type, cl, Call_Flag_Nonpersistent);
}

void disengage_obstacle_hooks(object who) {
	foreach(int type, mixed array cls : query_obstacle_hooks())
		foreach(closure cl : cls)
			Call_Hook_Off(who, type, cl, Call_Flag_Nonpersistent);
}

string query_obstacle_modifier_tag() {
	return "Ganesha_Obstacle_" + replace(capitalize_words(query_obstacle_name()), " ", "_") + "_Mod";
}

descriptor set_obstacle_modifier_tag(descriptor dxr) {
	Modifier_Set_Info(dxr, query_obstacle_modifier_tag(), True);
}

status has_obstacle_modifier_tag(descriptor dxr) {
	return Modifier_Query_Info(dxr, query_obstacle_modifier_tag());
}

void add_obstacle_trait_modifier(mixed modifier) {
    if(mappingp(modifier))
        modifier[Modifier_Type] = Modifier_Type_Trait;
	descriptor dxr = Modifier(modifier);
	Modifier_Set(dxr, Modifier_Type, Modifier_Type_Trait);
	Modifier_Set(dxr, Modifier_Bound, Modifier_Query(dxr, Modifier_Amount) * 3);
	Modifier_Flag_On(dxr, Modifier_Flag_Unremovable);
	//Modifier_Flag_On(dxr, Modifier_Flag_Persistent);
	set_obstacle_modifier_tag(dxr);
	obstacle["trait_modifiers"] ||= ({});
	obstacle["trait_modifiers"] += ({ dxr });
}

descriptor array query_obstacle_trait_modifiers() {
	return obstacle["trait_modifiers"] || ({});
}

void engage_obstacle_trait_modifiers(object who) {
	foreach(descriptor dxr : query_obstacle_trait_modifiers())
		who->add_trait_modifier(dxr);
}

descriptor array find_obstacle_trait_modifiers(object who) {
	return filter(who->query_trait_modifiers(), (:
		has_obstacle_modifier_tag($1)
	:));
}

void disengage_obstacle_trait_modifiers(object who) {
	foreach(descriptor trait_mod : find_obstacle_trait_modifiers(who))
		who->remove_trait_modifier(trait_mod);
}

void add_obstacle_skill_modifier(mixed modifier) {
	descriptor dxr = Modifier(modifier);
	Modifier_Set(dxr, Modifier_Type, Modifier_Type_Skill);
	Modifier_Set(dxr, Modifier_Bound, Modifier_Query(dxr, Modifier_Amount) * 3);
	Modifier_Flag_On(dxr, Modifier_Flag_Unremovable);
	//Modifier_Flag_On(dxr, Modifier_Flag_Persistent);
	set_obstacle_modifier_tag(dxr);
	obstacle["skill_modifiers"] ||= ({});
	obstacle["skill_modifiers"] += ({ dxr });
}

descriptor array query_obstacle_skill_modifiers() {
	return obstacle["skill_modifiers"] || ({});
}

void add_obstacle_scaled_skill_modifier(mixed modifier) {
	obstacle["scaled_skill_modifiers"] ||= ({});
	obstacle["scaled_skill_modifiers"] += ({ modifier });
}

mixed array query_obstacle_scaled_skill_modifiers() {
	return obstacle["scaled_skill_modifiers"] || ({});
}

void engage_obstacle_skill_modifiers(object who) {
	foreach(descriptor dxr : query_obstacle_skill_modifiers())
		who->add_skill_modifier(dxr);
	foreach(mapping modifier : query_obstacle_scaled_skill_modifiers()) {
		if(!arrayp(modifier[Modifier_Index]))
			modifier[Modifier_Index] = ({ modifier[Modifier_Index] });
		foreach(int skill : modifier[Modifier_Index]) {
			int amount = round(modifier[Modifier_Amount] * who->query_skill(skill));
			if(amount > 0) {
				descriptor dxr = Modifier(([
					Modifier_Type               : Modifier_Type_Skill,
					Modifier_Index              : skill,
					Modifier_Amount             : -amount,
					Modifier_Bound              : amount * -3,
					Modifier_Flags              : Modifier_Flag_Unremovable | Modifier_Flag_Persistent,
				]));
				set_obstacle_modifier_tag(dxr);
				who->add_skill_modifier(dxr);
			}
		}
	}
}

descriptor array find_obstacle_skill_modifiers(object who) {
	return filter(who->query_skill_modifiers(), (:
		has_obstacle_modifier_tag($1)
	:));
}

void disengage_obstacle_skill_modifiers(object who) {
	foreach(descriptor skill_mod : find_obstacle_skill_modifiers(who))
		who->remove_skill_modifier(skill_mod);
}

void recalculate_obstacle_scaled_skill_modifiers(object who) {
	mapping array list = query_obstacle_scaled_skill_modifiers();
	if(!sizeof(list))
		return 0;
	descriptor array mods = find_obstacle_skill_modifiers(who);
	foreach(mapping modifier : list) {
		if(!arrayp(modifier[Modifier_Index]))
			modifier[Modifier_Index] = ({ modifier[Modifier_Index] });
		foreach(int skill : modifier[Modifier_Index]) {
			if(!Is_Skill(skill))
				error("Invalid skill " + printable(skill));
			foreach(descriptor mod : mods) {
				int array index = Modifier_Query(mod, Modifier_Index);
				if(sizeof(index) == 1 && index[0] == skill && Modifier_Query(mod, Modifier_Bound) == Modifier_Query(mod, Modifier_Amount) * 3) {
					Modifier_Flag_On(mod, Modifier_Flag_Suspend_Remove_Calc);
					who->remove_skill_modifier(mod);
					break;
				}
			}
			int amount = round(modifier[Modifier_Amount] * who->query_skill(skill));
			if(amount > 0) {
				descriptor dxr = Modifier(([
					Modifier_Type               : Modifier_Type_Skill,
					Modifier_Index              : skill,
					Modifier_Amount             : -amount,
					Modifier_Bound              : amount * -3,
					Modifier_Flags              : Modifier_Flag_Unremovable | Modifier_Flag_Persistent,
				]));
				set_obstacle_modifier_tag(dxr);
				who->add_skill_modifier(dxr);
			}
		}
	}
}

void add_obstacle_attribute_modifier(mixed modifier) {
	descriptor dxr = Modifier(modifier);
	Modifier_Set(dxr, Modifier_Type, Modifier_Type_Attribute);
	Modifier_Set(dxr, Modifier_Bound, Modifier_Query(dxr, Modifier_Amount) * 3);
	Modifier_Flag_On(dxr, Modifier_Flag_Unremovable);
	//Modifier_Flag_On(dxr, Modifier_Flag_Persistent);
	set_obstacle_modifier_tag(dxr);
	obstacle["attribute_modifiers"] ||= ({});
	obstacle["attribute_modifiers"] += ({ dxr });
}

descriptor array query_obstacle_attribute_modifiers() {
	return obstacle["attribute_modifiers"] || ({});
}

void add_obstacle_scaled_attribute_modifier(mixed modifier) {
	obstacle["scaled_attribute_modifiers"] ||= ({});
	obstacle["scaled_attribute_modifiers"] += ({ modifier });
}

mixed array query_obstacle_scaled_attribute_modifiers() {
	return obstacle["scaled_attribute_modifiers"] || ({});
}

void engage_obstacle_attribute_modifiers(object who) {
	foreach(descriptor dxr : query_obstacle_attribute_modifiers())
		who->add_attribute_modifier(dxr);
	foreach(mapping modifier : query_obstacle_scaled_attribute_modifiers()) {
		if(!arrayp(modifier[Modifier_Index]))
			modifier[Modifier_Index] = ({ modifier[Modifier_Index] });
		foreach(int attribute : modifier[Modifier_Index]) {
			int amount = round(modifier[Modifier_Amount] * who->query_attribute(attribute));
			if(amount > 0) {
				descriptor dxr = Modifier(([
					Modifier_Type               : Modifier_Type_Attribute,
					Modifier_Index              : attribute,
					Modifier_Amount             : -amount,
					Modifier_Bound              : amount * -3,
					Modifier_Flags              : Modifier_Flag_Unremovable | Modifier_Flag_Persistent,
				]));
				set_obstacle_modifier_tag(dxr);
				who->add_attribute_modifier(dxr);
			}
		}
	}
}

descriptor array find_obstacle_attribute_modifiers(object who) {
	return filter(who->query_attribute_modifiers(), (:
		has_obstacle_modifier_tag($1)
	:));
}

void disengage_obstacle_attribute_modifiers(object who) {
	foreach(descriptor attribute_mod : find_obstacle_attribute_modifiers(who))
		who->remove_attribute_modifier(attribute_mod);
}

void recalculate_obstacle_scaled_attribute_modifiers(object who) {
	mapping array list = query_obstacle_scaled_attribute_modifiers();
	if(!sizeof(list))
		return;
	descriptor array mods = find_obstacle_attribute_modifiers(who);
	foreach(mapping modifier : list) {
		if(!arrayp(modifier[Modifier_Index]))
			modifier[Modifier_Index] = ({ modifier[Modifier_Index] });
		foreach(int attribute : modifier[Modifier_Index]) {
			foreach(descriptor mod : mods) {
				int array index = Modifier_Query(mod, Modifier_Index);
				if(sizeof(index) == 1 && index[0] == attribute && Modifier_Query(mod, Modifier_Bound) == Modifier_Query(mod, Modifier_Amount) * 3) {
					Modifier_Flag_On(mod, Modifier_Flag_Suspend_Remove_Calc);
					who->remove_attribute_modifier(mod);
					break;
				}
			}
			int amount = round(modifier[Modifier_Amount] * who->query_attribute(attribute));
			if(amount > 0) {
				descriptor dxr = Modifier(([
					Modifier_Type               : Modifier_Type_Attribute,
					Modifier_Index              : attribute,
					Modifier_Amount             : -amount,
					Modifier_Bound              : amount * -3,
					Modifier_Flags              : Modifier_Flag_Unremovable | Modifier_Flag_Persistent,
				]));
				set_obstacle_modifier_tag(dxr);
				who->add_attribute_modifier(dxr);
			}
		}
	}
}

void add_obstacle_speed_modifier(mixed modifier) {
	descriptor dxr = Modifier(modifier);
	Modifier_Set(dxr, Modifier_Type, Modifier_Type_Speed);
	Modifier_Set(dxr, Modifier_Bound, Modifier_Query(dxr, Modifier_Amount) * 3);
	Modifier_Flag_On(dxr, Modifier_Flag_Unremovable);
	//Modifier_Flag_On(dxr, Modifier_Flag_Persistent);
	set_obstacle_modifier_tag(dxr);
	obstacle["speed_modifiers"] ||= ({});
	obstacle["speed_modifiers"] += ({ dxr });
}

descriptor array query_obstacle_speed_modifiers() {
	return obstacle["speed_modifiers"] || ({});
}

void engage_obstacle_speed_modifiers(object who) {
	foreach(descriptor dxr : query_obstacle_speed_modifiers())
		who->add_speed_modifier(dxr);
}

descriptor array find_obstacle_speed_modifiers(object who) {
	return filter(who->query_speed_modifiers(), (:
		has_obstacle_modifier_tag($1)
	:));
}

void disengage_obstacle_speed_modifiers(object who) {
	foreach(descriptor speed_mod : find_obstacle_speed_modifiers(who))
		who->remove_speed_modifier(speed_mod);
}

void add_obstacle_resistance_modifier(mixed modifier) {
	descriptor dxr = Modifier(modifier);
	Modifier_Set(dxr, Modifier_Type, Modifier_Type_Resistance);
	Modifier_Set(dxr, Modifier_Bound, Modifier_Query(dxr, Modifier_Amount) * 3);
	Modifier_Flag_On(dxr, Modifier_Flag_Unremovable);
	//Modifier_Flag_On(dxr, Modifier_Flag_Persistent);
	set_obstacle_modifier_tag(dxr);
	obstacle["resistance_modifiers"] ||= ({});
	obstacle["resistance_modifiers"] += ({ dxr });
}

descriptor array query_obstacle_resistance_modifiers() {
	return obstacle["resistance_modifiers"] || ({});
}

void engage_obstacle_resistance_modifiers(object who) {
	foreach(descriptor dxr : query_obstacle_resistance_modifiers())
		who->add_resistance_modifier(dxr);
}

descriptor array find_obstacle_resistance_modifiers(object who) {
	return filter(who->query_resistance_modifiers(), (:
		has_obstacle_modifier_tag($1)
	:));
}

void disengage_obstacle_resistance_modifiers(object who) {
	foreach(descriptor resistance_mod : find_obstacle_resistance_modifiers(who))
		who->remove_resistance_modifier(resistance_mod);
}

void obstacle_attach(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	engage_obstacle_hooks(who);
	engage_obstacle_trait_modifiers(who);
	engage_obstacle_skill_modifiers(who);
	engage_obstacle_attribute_modifiers(who);
	engage_obstacle_speed_modifiers(who);
	if(obstacle_attach)
		funcall(obstacle_attach, obj);
}

void obstacle_detach(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	disengage_obstacle_hooks(who);
	disengage_obstacle_trait_modifiers(who);
	disengage_obstacle_skill_modifiers(who);
	disengage_obstacle_attribute_modifiers(who);
	disengage_obstacle_speed_modifiers(who);
	if(obstacle_detach)
		funcall(obstacle_detach, obj);
}

void obstacle_initialize(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	if(obstacle_initialize)
		funcall(obstacle_initialize, obj);
	descriptor display = query_obstacle_initialize_display();
	if(display)
		who->display(display);
}

void obstacle_overcome(object obj) {
	object who = obj->ganesha_challenge_query_owner();
	if(obstacle_overcome)
		funcall(obstacle_overcome, obj);
	descriptor display = query_obstacle_overcome_display();
	if(display)
		who->display(display);
}

void obstacle_yield(object obj) {
	obstacle_overcome(obj);
}

void obstacle_fail(object obj) {
	obstacle_overcome(obj);
}

void set_challenge_component_selection_adjustments(mapping adjust) {
	obstacle["requirement_rarity_adjustments"] = adjust;
}

mapping query_challenge_component_selection_adjustments() {
	return obstacle["requirement_rarity_adjustments"] || ([]);
}

void set_obstacle_eligibility_condition(mixed cond) {
	descriptor use;
	if(Is_Condition(cond))
		use = cond;
	else
		use = Condition(cond);
	obstacle["eligibility_condition"] = use;
}

descriptor query_obstacle_eligibility_condition() {
	return obstacle["eligibility_condition"];
}

status query_obstacle_eligibility(object who) {
	descriptor cond = query_obstacle_eligibility_condition();
	unless(cond)
		return True;
	return Condition_Apply(cond, who, 0);
}
