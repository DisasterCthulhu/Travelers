#include <Travelers.h>
#include <daemon.h>
#include <extant.h>

inherit "/std/daemon";
inherit "/mod/daemon/broker";

internal mapping creator_benefits;
internal mapping obstacle_lookup = ([]);
internal mapping obstacle_rarities = ([]);
internal mapping requirement_lookup = ([]);
internal mapping requirement_rarities = ([]);
internal mapping bestowal_lookup = ([]);
internal mapping bestowal_rarities = ([]);
internal mapping taboo_lookup = ([]);
internal mapping taboo_rarities = ([]);
internal mapping forfeit_lookup = ([]);
internal mapping forfeit_rarities = ([]);
internal mapping invocation_lookup = ([]);
internal mapping invocation_rarities = ([]);
internal mapping help_lookup = ([]);
internal mapping challenge_component_selection_adjustment_index = ([]);
internal object array valid_obstacles = ({});
internal string array obstacle_names = ({});
internal string array taboo_names = ({});
internal object array valid_taboos = ({});
internal string array forfeit_names = ({});
internal object array valid_forfeits = ({});
internal object array valid_requirements = ({});
internal string array requirement_names = ({});
internal object array valid_bestowals = ({});
internal string array bestowal_names = ({});
internal object array valid_invocations = ({});
internal string array invocation_names = ({});

void register_obstacle(object def);
void unregister_obstacle(object def);
void register_requirement(object def);
void unregister_requirement(object def);
void register_bestowal(object def);
void unregister_bestowal(object def);
void register_taboo(object def);
void unregister_taboo(object def);
void register_forfeit(object def);
void unregister_forfeit(object def);
void register_invocation(object def);
void unregister_invocation(object def);
string obstacle_select();
string requirement_select();
string bestowal_select();
string invocation_select();
string taboo_select();
string forfeit_select();

void preinit() {
	::preinit();
	creator_benefits = Persistence(Persistence_Tag("creator_benefits"), ([]));
}

void configure() {
	::configure();
	set_definition_registration(([
		Travelers_Definition("requirement_*")   : #'register_requirement,
		Travelers_Definition("obstacle_*")      : #'register_obstacle,
		Travelers_Definition("bestowal_*")      : #'register_bestowal,
		Travelers_Definition("invocation_*")    : #'register_invocation,
		Travelers_Definition("taboo_*")         : #'register_taboo,
		Travelers_Definition("forfeit_*")       : #'register_forfeit,
	]));
	set_definition_priority(([
		Travelers_Definition("requirement_*")   : 1,
		Travelers_Definition("obstacle_*")      : 2,
		Travelers_Definition("bestowal_*")      : 3,
		Travelers_Definition("taboo_*")         : 4,
		Travelers_Definition("forfeit_*")       : 5,
		Travelers_Definition("invocation_*")    : 6,
	]));
	set_definition_unregistration(([
		Travelers_Definition("requirement_*")   : #'unregister_requirement,
		Travelers_Definition("obstacle_*")      : #'unregister_obstacle,
		Travelers_Definition("bestowal_*")      : #'unregister_bestowal,
		Travelers_Definition("invocation_*")    : #'unregister_invocation,
		Travelers_Definition("taboo_*")         : #'unregister_taboo,
		Travelers_Definition("forfeit_*")       : #'unregister_forfeit,
	]));
}

void set_challenge_component_selection_adjustment(string index, string value, float adjust) {
    challenge_component_selection_adjustment_index[index] ||= ([]);
    if(challenge_component_selection_adjustment_index[index][value])
        warn("tried to overwrite component selection index " + index + ", value " + value);
    else
        challenge_component_selection_adjustment_index[index][value] = adjust;
}

void register_challenge_component_selection_adjustments(object component) {
    mapping component_adjustments = component->query_challenge_component_selection_adjustments();
    if(!component_adjustments)
        return;
    string component_name = component->query_challenge_component_name();
    foreach(string what, float adjust : component_adjustments) {
        set_challenge_component_selection_adjustment(what, component_name, adjust);
        set_challenge_component_selection_adjustment(component_name, what, adjust);
    }
}

void unregister_challenge_component_selection_adjustments(object component) {
    string component_name = component->query_challenge_component_name();
    m_delete(challenge_component_selection_adjustment_index, component_name);
    foreach(string index, mapping adjustments : challenge_component_selection_adjustment_index)
        m_delete(adjustments, component_name);
}

mapping query_challenge_component_selection_adjustment_index() {
    return challenge_component_selection_adjustment_index;
}

void register_obstacle(object def) {
	string name = def->query_obstacle_name();
	obstacle_rarities[name] = def->query_obstacle_rarity();
	obstacle_lookup[name] = def;
	array_specify(&valid_obstacles, def);
	array_specify(&obstacle_names, name);
	valid_obstacles -= ({ 0 });
	register_challenge_component_selection_adjustments(def);
}

void unregister_obstacle(object def) {
    unregister_challenge_component_selection_adjustments(def);
	string name = def->query_obstacle_name();
	m_delete(obstacle_rarities, name);
	m_delete(obstacle_lookup, name);
	array_remove(&valid_obstacles, name);
	array_remove(&obstacle_names, name);
	valid_obstacles -= ({ 0 });
}

void register_requirement(object def) {
	string name = def->query_requirement_name();
	requirement_rarities[name] = def->query_requirement_rarity();
	requirement_lookup[name] = def;
	array_specify(&valid_requirements, def);
	array_specify(&requirement_names, name);
	valid_requirements -= ({ 0 });
	register_challenge_component_selection_adjustments(def);
}

void unregister_requirement(object def) {
    unregister_challenge_component_selection_adjustments(def);
	string name = def->query_requirement_name();
	m_delete(requirement_rarities, name);
	m_delete(requirement_lookup, name);
	array_remove(&valid_requirements, def);
	array_remove(&requirement_names, name);
	valid_requirements -= ({ 0 });
}

void register_taboo(object def) {
	string name = def->query_taboo_name();
	taboo_rarities[name] = def->query_taboo_rarity();
	taboo_lookup[name] = def;
	array_specify(&valid_taboos, def);
	array_specify(&taboo_names, name);
	valid_taboos -= ({ 0 });
	register_challenge_component_selection_adjustments(def);
}

void unregister_taboo(object def) {
    unregister_challenge_component_selection_adjustments(def);
	string name = def->query_taboo_name();
	m_delete(taboo_rarities, name);
	m_delete(taboo_lookup, name);
	array_remove(&valid_taboos, def);
	array_remove(&taboo_names, name);
	valid_taboos -= ({ 0 });
}

void register_forfeit(object def) {
	string name = def->query_forfeit_name();
	forfeit_rarities[name] = def->query_forfeit_rarity();
	forfeit_lookup[name] = def;
	array_specify(&valid_forfeits, def);
	array_specify(&forfeit_names, name);
	valid_forfeits -= ({ 0 });
	register_challenge_component_selection_adjustments(def);
}

void unregister_forfeit(object def) {
    unregister_challenge_component_selection_adjustments(def);
	string name = def->query_forfeit_name();
	m_delete(forfeit_rarities, name);
	m_delete(forfeit_lookup, name);
	array_remove(&valid_forfeits, def);
	array_remove(&forfeit_names, name);
	valid_forfeits -= ({ 0 });
}

void register_bestowal(object def) {
	string name = def->query_bestowal_name();
	bestowal_rarities[name] = def->query_bestowal_rarity();
	bestowal_lookup[name] = def;
	array_specify(&valid_bestowals, def);
	array_specify(&bestowal_names, name);
	valid_bestowals -= ({ 0 });
	register_challenge_component_selection_adjustments(def);
}

void unregister_bestowal(object def) {
    unregister_challenge_component_selection_adjustments(def);
	string name = def->query_bestowal_name();
	m_delete(bestowal_rarities, name);
	m_delete(bestowal_lookup, name);
	array_remove(&valid_bestowals, def);
	array_remove(&bestowal_names, name);
	valid_bestowals -= ({ 0 });
}

string help_invocation(object who, string what) {
	what = replace(what, "_", " ");
	return Travelers_Invocation(what)->get_invocation_help(who);
}

void register_invocation(object def) {
	string name = def->query_invocation_name();
	invocation_rarities[name] = def->query_invocation_rarity();
	invocation_lookup[name] = def;
	help_lookup[name] = #'help_invocation;
	array_specify(&valid_invocations, def);
	array_specify(&invocation_names, name);
	valid_invocations -= ({ 0 });
}

void unregister_invocation(object def) {
	string name = def->query_invocation_name();
	m_delete(invocation_rarities, name);
	m_delete(invocation_lookup, name);
	array_remove(&valid_invocations, def);
	array_remove(&invocation_names, name);
	valid_invocations -= ({ 0 });
}

string array query_available_obstacles() {
	return obstacle_names;
}

object array query_valid_obstacles() {
	return valid_obstacles;
}

mapping query_obstacle_rarities() {
	return obstacle_rarities;
}

string obstacle_select(mapping adjust) {
	return Rarity_Select(obstacle_rarities, adjust);
}

string array query_available_requirements() {
	return requirement_names;
}

object array query_valid_requirements() {
	return valid_requirements;
}

mapping query_requirement_rarities() {
	return requirement_rarities;
}

string requirement_select(mapping adjust) {
	return Rarity_Select(requirement_rarities, adjust);
}

string array query_available_taboos() {
	return taboo_names;
}

object array query_valid_taboos() {
	return valid_taboos;
}

mapping query_taboo_rarities() {
	return taboo_rarities;
}

string taboo_select(mapping adjust) {
	return Rarity_Select(taboo_rarities, adjust);
}

string array query_available_forfeits() {
	return forfeit_names;
}

object array query_valid_forfeits() {
	return valid_forfeits;
}

mapping query_forfeit_rarities() {
	return forfeit_rarities;
}

string forfeit_select(mapping adjust) {
	return Rarity_Select(forfeit_rarities, adjust);
}

string array query_available_bestowals() {
	return bestowal_names;
}

object array query_valid_bestowals() {
	return valid_bestowals;
}

mapping query_bestowal_rarities() {
	return bestowal_rarities;
}

string bestowal_select(mapping adjust) {
	return Rarity_Select(bestowal_rarities, adjust);
}

string array query_available_invocations() {
	return invocation_names;
}

object array query_valid_invocations() {
	return valid_invocations;
}

mapping query_invocation_rarities() {
	return invocation_rarities;
}

string invocation_select(mapping adjust) {
	return Rarity_Select(invocation_rarities, adjust);
}

int query_obstacle_rarity(string obstacle) {
	return obstacle_rarities[obstacle];
}

int query_requirement_rarity(string requirement) {
	return requirement_rarities[requirement];
}

int query_bestowal_rarity(string bestowal) {
	return bestowal_rarities[bestowal];
}

int query_invocation_rarity(string invocation) {
	return invocation_rarities[invocation];
}

status is_obstacle(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return member(obstacle_lookup, lower_case(what));
	case T_OBJECT :
		return member(valid_obstacles, what) != Null;
	default       :
		return False;
	}
}

object obstacle(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		string str = lower_case(what);
		object obj = Travelers_Definition("obstacle_" + replace(str, " ", "_"))->load();
		return obstacle_lookup[lower_case(what)] || obj || error("Invalid obstacle '" + what + "'");
	case T_OBJECT :
		return what;
	default       :
		error("Invalid obstacle specification, " + printable(what));
	}
}

status is_requirement(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return member(requirement_lookup, lower_case(what));
	case T_OBJECT :
		return member(valid_requirements, what) != Null;
	default       :
		return False;
	}
}

object requirement(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return requirement_lookup[lower_case(what)] || error("Invalid requirement '" + what + "'");
	case T_OBJECT :
		return what;
	default       :
		error("Invalid requirement specification, " + printable(what));
	}
}

status is_taboo(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return member(taboo_lookup, lower_case(what));
	case T_OBJECT :
		return member(valid_taboos, what) != Null;
	default       :
		return False;
	}
}

object taboo(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return taboo_lookup[lower_case(what)] || error("Invalid taboo '" + what + "'");
	case T_OBJECT :
		return what;
	default       :
		error("Invalid taboo specification, " + printable(what));
	}
}

status is_forfeit(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return member(forfeit_lookup, lower_case(what));
	case T_OBJECT :
		return member(valid_forfeits, what) != Null;
	default       :
		return False;
	}
}

object forfeit(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return forfeit_lookup[lower_case(what)] || error("Invalid forfeit '" + what + "'");
	case T_OBJECT :
		return what;
	default       :
		error("Invalid forfeit specification, " + printable(what));
	}
}

status is_bestowal(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return member(bestowal_lookup, lower_case(what));
	case T_OBJECT :
		return member(valid_bestowals, what) != Null;
	default       :
		return False;
	}
}

object bestowal(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return bestowal_lookup[lower_case(what)] || error("Invalid bestowal '" + what + "'");
	case T_OBJECT :
		return what;
	default       :
		error("Invalid bestowal specification, " + printable(what));
	}
}

status is_invocation(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return member(invocation_lookup, lower_case(what));
	case T_OBJECT :
		return member(valid_invocations, what) != Null;
	default       :
		return False;
	}
}

object invocation(mixed what) {
	switch(typeof(what)) {
	case T_STRING :
		return invocation_lookup[lower_case(what)] || error("Invalid invocation '" + what + "'");
	case T_OBJECT :
		return what;
	default       :
		error("Invalid invocation specification, " + printable(what));
	}
}

int ganesha_query_karma_phala(object who) {
    return who->query_info("Ganesha_Karma_Phala");
}

int ganesha_set_karma_phala(object who, int amount) {
    who->set_info("Ganesha_Karma_Phala", amount);
    return amount;
}

int ganesha_add_karma_phala(object who, int amount) {
    if(amount > 0)
        who->display(([
            Message_Content                     : ({
                0, ({ "sense", 0 }), "Ganesha looking upon", ({ 'o', 0 }), "favorably",
            }),
            Message_Senses                      : Message_Sense_Spiritual,
            Message_Color                       : "status: enhancement",
        ]));
    else if(amount < 0)
        who->display(([
            Message_Content                     : ({
                0, ({ "sense", 0 }), "Ganesha looking upon", ({ 'o', 0 }), "unfavorably",
            }),
            Message_Senses                      : Message_Sense_Spiritual,
            Message_Color                       : "status: loss",
        ]));
    return ganesha_set_karma_phala(who, ganesha_query_karma_phala(who) + amount);
}

object find_challenge(object who) {
	object array challenges = filter(all_inventory(who), (: $1->is_ganesha_challenge() :));
	if(sizeof(challenges) > 1)
		error("multiple challenges found on " + printable(who));
	if(!sizeof(challenges))
		return 0;
	return challenges[0];
}

object find_interdiction_field(object who) {
	object array fields = filter(all_inventory(who), (: $1->is_ganesha_interdiction_field() :));
	if(sizeof(fields) > 1)
		error("multiple interdiction fields found on " + printable(who));
	if(!sizeof(fields))
		return 0;
	return fields[0];
}

mapping query_challenge_selection_adjustments(object challenge) {
    mapping out = ([]);
    string requirement = challenge->ganesha_challenge_query_requirement();
    string obstacle = challenge->ganesha_challenge_query_obstacle();
    string taboo = challenge->ganesha_challenge_query_taboo();
    string forfeit = challenge->ganesha_challenge_query_forfeit();
    string array components = ({ requirement, obstacle, taboo, forfeit });
    components -= ({ 0 });
    foreach(string component : components) {
        if(challenge_component_selection_adjustment_index[component]) {
            foreach(string adjustment, float adjust : challenge_component_selection_adjustment_index[component]) {
                if(out[adjustment]) {
                    out[adjustment] *= adjust;
                } else {
                    out[adjustment] = adjust;
                }
            }
        }
    }
    return out;
}

string generate_random_obstacle(object challenge) {
	mapping adjust = query_challenge_selection_adjustments(challenge);
	foreach(object valid_obstacle : query_valid_obstacles())
		if(!valid_obstacle->query_obstacle_eligibility(challenge->ganesha_challenge_query_owner()))
			adjust[valid_obstacle->query_obstacle_name()] = 0.0;   
	return obstacle_select(adjust);
}

string generate_random_requirement(object challenge) {
	mapping adjust = query_challenge_selection_adjustments(challenge);
	foreach(object valid_requirement : query_valid_requirements())
		if(!valid_requirement->query_requirement_eligibility(challenge->ganesha_challenge_query_owner()))
			adjust[valid_requirement->query_requirement_name()] = 0.0;   
	return requirement_select(adjust);
}

string generate_random_taboo(object challenge) {
	mapping adjust = query_challenge_selection_adjustments(challenge);
	foreach(object valid_taboo : query_valid_taboos())
		if(!valid_taboo->query_taboo_eligibility(challenge->ganesha_challenge_query_owner()))
			adjust[valid_taboo->query_taboo_name()] = 0.0;   
	return taboo_select(adjust);
}

string generate_random_forfeit(object challenge) {
	mapping adjust = query_challenge_selection_adjustments(challenge);
	foreach(object valid_forfeit : query_valid_forfeits())
		if(!valid_forfeit->query_forfeit_eligibility(challenge->ganesha_challenge_query_owner()))
			adjust[valid_forfeit->query_forfeit_name()] = 0.0;   
	return forfeit_select(adjust);
}

void check_for_challenge_creator_benefits(object who, object link) {
	string extant = who->require_extant();
	if(creator_benefits && creator_benefits[extant]) {
		link->travelers_benefit_challenge_creator(creator_benefits[extant]);
		m_delete(creator_benefits, extant);
	}
}

void benefit_challenge_creator(object challenge) {
	mixed creator = challenge->ganesha_challenge_query_creator();
	int value = challenge->ganesha_challenge_query_value();
	if(objectp(creator)) {
		object link = creator->query_affiliation_link(project_control());
		if(link)
			link->travelers_benefit_challenge_creator(value);
	} else if(stringp(creator)) {
	    creator_benefits[creator] ||= ({});
		creator_benefits[creator] += ({ value });
	}
}

mixed query_creator_benefits() {
    return creator_benefits;
}

string array challenge_overcome(object challenge) {
    string array out = ({});
	object who = challenge->ganesha_challenge_query_owner();
	mixed creator = challenge->ganesha_challenge_query_creator();
	int value = challenge->ganesha_challenge_query_value();
	Ganesha_Add_Karma_Phala(who, value);
	if(creator && creator != who)
		benefit_challenge_creator(challenge);
	int value_remaining = value;
	mapping adjust = ([]);
	foreach(object valid_bestowal : query_valid_bestowals()) {
		string bestowal_name = valid_bestowal->query_bestowal_name();
		if(!valid_bestowal->query_bestowal_eligibility(challenge->ganesha_challenge_query_owner()))
			adjust[bestowal_name] = 0.0;
		int bestowal_value = valid_bestowal->query_bestowal_value();
		if(bestowal_value > value_remaining)
			adjust[bestowal_name] = 0.0;
	}
	while(value_remaining) {
		foreach(object valid_bestowal : query_valid_bestowals()) {
			string bestowal_name = valid_bestowal->query_bestowal_name();
			int bestowal_value = valid_bestowal->query_bestowal_value();
			if(bestowal_value > value_remaining)
				adjust[bestowal_name] = 0.0;
		}
		string bestowal_name = bestowal_select(adjust);
		if(!bestowal_name)
			break;
		object bestowal = Travelers_Bestowal(bestowal_name);
		int bestowal_value = bestowal->query_bestowal_value();
		bestowal->bestowal_bestow(who);
		out += ({ bestowal_name });
		value_remaining -= bestowal_value;
		adjust[bestowal_name] = 0.0;
	}
	return out;
}

mixed retrieve_help(string text, object who) {
	return funcall(help_lookup[text], who, text);
}

