#include <grant_channel.h>
#include <item.h>
#include <specialty_access.h>
#include <speech.h>
#include <Travelers.h>
#include <Empathic_Bonds.h>
#include <bond.h>

inherit "/std/affiliation_link";

void travelers_convert_old_info() {
    if(info["kharma phala"]) {
        Ganesha_Set_Karma_Phala(owner, info["kharma phala"]);
        m_delete(info, "kharma phala");
    }
    if(info["karma phala"]) {
        Ganesha_Set_Karma_Phala(owner, info["karma phala"]);
        m_delete(info, "karma phala");
    }
	unless(info["logos"])
		return;
	owner->display(({
		"{{bright red}Updating old guild info; you are being grandfathered into the rewrite with core powers and some karma phala.}"
	}));
	int old_logos = info["logos"];
	foreach(mixed key, mixed value : info)
		m_delete(info, key);
	Travelers_Bestowal("katakacha")->bestowal_bestow(owner);
	Travelers_Bestowal("brahmanda-prakasa")->bestowal_bestow(owner);
	Travelers_Bestowal("conceptual navigation")->bestowal_bestow(owner);
	Ganesha_Set_Karma_Phala(owner, old_logos);
}

string array travelers_query_conceptual_navigation_points() {
	return info["conceptual navigation points"] ||= ({});
}

int travelers_query_unused_conceptual_navigation_point_count() {
	int out = 0;
	foreach(string point : travelers_query_conceptual_navigation_points())
		if(!point)
			out++;
	return out;
}

int travelers_query_conceptual_navigation_points_count() {
	return sizeof(travelers_query_conceptual_navigation_points());
}

string travelers_query_conceptual_navigation_point(int count) {
	return travelers_query_conceptual_navigation_points()[count - 1];
}

object array travelers_query_conceptual_navigation_point_rooms() {
	string array points = travelers_query_conceptual_navigation_points();
	return map(points, (:
		unless($1)
			return $1;
		if(catch(resolve_project_path($1)->load())) {
			int idx = member($2, $1);
			info["conceptual navigation points"][idx] = 0;
			return 0;
		}
		return resolve_project_path($1)->load();
	:), points);
}

object travelers_query_conceptual_navigation_point_room(int count) {
	return travelers_query_conceptual_navigation_point_rooms()[count - 1];
}

void travelers_add_conceptual_navigation_points(int how_many) {
	info["conceptual navigation points"] ||= ({});
	while(how_many) {
		info["conceptual navigation points"] += ({ 0 });
		how_many--;
	}
}

void travelers_set_conceptual_navigation_points(string array points) {
	info["conceptual navigation points"] = points;
}

void travelers_set_conceptual_navigation_point(int count, mixed room) {
	if(!room) {
		info["conceptual navigation points"][count - 1] = 0;
		return;
	}
	if(objectp(room) || (stringp(room) && !begins_with(room, "~"))) {
		room = room->load();
		room = project_file(object_name(room));
	}
	info["conceptual navigation points"][count - 1] = room;
}

void travelers_spend_conceptual_navigation_points(int how_many) {
    while(how_many) {
        travelers_set_conceptual_navigation_points(array_remove(info["conceptual navigation points"], 0));
        how_many--;
    }
}

string array travelers_query_divine_waypoints() {
	info["divine waypoints"] ||= ({});
	return info["divine waypoints"];
}

int travelers_query_divine_waypoints_count() {
	return sizeof(travelers_query_divine_waypoints());
}

string travelers_query_divine_waypoint(int count) {
	return travelers_query_divine_waypoints()[count - 1];
}

object array travelers_query_divine_waypoint_rooms() {
	string array waypoints = travelers_query_divine_waypoints();
	return map(waypoints, (:
		unless($1)
			return $1;
		if(catch(resolve_project_path($1)->load())) {
			int idx = member($2, $1);
			info["divine waypoints"][idx] = 0;
			return 0;
		}
		return resolve_project_path($1)->load();
	:), waypoints);
}

object travelers_query_divine_waypoint_room(int count) {
	return travelers_query_divine_waypoint_rooms()[count - 1];
}

void travelers_add_divine_waypoints(int how_many) {
	info["divine waypoints"] ||= ({});
	while(how_many) {
		info["divine waypoints"] += ({ 0 });
		how_many--;
	}
}

void travelers_set_divine_waypoints(string array points) {
	info["divine waypoints"] = points;
}

void travelers_set_divine_waypoint(int count, mixed room) {
	if(!room) {
		info["divine waypoints"][count - 1] = 0;
		return;
	}
	if(objectp(room) || (stringp(room) && !begins_with(room, "~"))) {
		room = room->load();
		room = project_file(object_name(room));
	}
	info["divine waypoints"][count - 1] = room;
}

void travelers_add_bestowal(string bestowal) {
	info["bestowals"] ||= ({});
	if(member(info["bestowals"], bestowal) != Null)
		error("attempted to add duplicate bestowal '" + bestowal + "'");
	info["bestowals"] += ({ bestowal });
}

void travelers_set_bestowals(string array bestowals) {
	info["bestowals"] = bestowals;
}

string array travelers_query_bestowals() {
	return info["bestowals"] || ({});
}

object array travelers_query_bestowal_definitions() {
	return map(travelers_query_bestowals(), (: Travelers_Bestowal($1) :));
}

descriptor array travelers_query_bestowal_specialty_access() {
	object array specialty_bestowals = filter(travelers_query_bestowal_definitions(), (:
		$1->query_bestowal_type() == Travelers_Bestowal_Type_Specialty_Access
	:));
	descriptor array out = ({});
	foreach(object specialty_bestowal : specialty_bestowals)
		out += specialty_bestowal->query_bestowal_specialty_access();
	return out;
}

void travelers_bestowal_bestowed(object bestowal) {
	int value = bestowal->query_bestowal_value();
	int type = bestowal->query_bestowal_type();
	string name = bestowal->query_bestowal_name();
	if(type != Travelers_Bestowal_Type_Recurring)
		travelers_add_bestowal(name);
	if(type == Travelers_Bestowal_Type_Specialty_Access)
		owner->update_specialty_access();
	if(type == Travelers_Bestowal_Type_Attribute_Adjustment) {
		int array adjust = bestowal->query_bestowal_attribute_adjustment();
		owner->add_permanent_attribute_adjustment(adjust[0], adjust[1]);
	}
	if(type == Travelers_Bestowal_Type_Kabatha_Resistance) {
		descriptor bond = Empathic_Bonds_Find_Affiliation_Bond(owner, project_control());
		if(bond) {
			object familiar = Bond_Query(bond, Bond_Familiar);
			object kabatha = familiar && familiar->query_child_object("is_kabatha");
			if(kabatha)
				kabatha->kabatha_update_resistances(familiar, bestowal->query_bestowal_kabatha_resistances());
		}
	}
}

void travelers_benefit_challenge_creator(mixed value) {
    if(intp(value)) {
	    Ganesha_Add_Karma_Phala(owner, value / 2);
	    owner->display(([
		    Message_Content         : ({
			    0, ({ "sense", 0 }), "that a challenge that", ({ 'p', 0 }), "provided has been successfully completed"
		    }),
		    Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
		    Message_Color           : "status: enhancement",
	    ]));
	} else if(arrayp(value)) {
	    Ganesha_Add_Karma_Phala(owner, accumulate(value) / 2);
	    owner->display(([
		    Message_Content         : ({
			    0, ({ "sense", 0 }), "that", things("challenge", sizeof(value)),"that", ({ 'p', 0 }), "provided", ({ "have", value }), "been successfully completed"
		    }),
		    Message_Senses          : Message_Sense_Spiritual | Message_Sense_Cognitive,
		    Message_Color           : "status: enhancement",
	    ]));
	}
}

void travelers_check_for_challenge_creator_benefits() {
	Travelers_Daemon("dharma")->check_for_challenge_creator_benefits(owner, this_object());
}

void travelers_do_hear(descriptor dxr) {
	if(Speech_Query(dxr, Speech_Speaker) != owner || Speech_Query(dxr, Speech_Language) != Skill_Archaen)
		return;
	if(Speech_Flag_Check(dxr, Speech_Flag_Distance || Speech_Flag_OOC))
		return;
	if(Speech_Flag_Check(dxr, Speech_Flag_Targeted))
		if(member(Speech_Query(dxr, Speech_Recipients), owner) == Null)
			return;
	string str = lower_case(Speech_Query(dxr, Speech_Modified_Message));
	object array invocations = filter(Travelers_Daemon("dharma")->query_valid_invocations(), (: $1->query_invocation_known($2) :), owner);
	foreach(object invocation : invocations) {
		string key = invocation->query_invocation_lower_case_key();
		int ix = strstr(str, key);
		if(ix == 0) {
			mapping args = ([
				"who"       : owner,
				"speech"    : dxr,
			]);
			if(strlen(str) - ix > strlen(key)) {
				string rest = str[ix + strlen(key)..];
				while(rest[0] == ' ')
					rest = rest[1..];
				rest = regreplace(rest, " *,.*", "", 0);
				string how = invocation->query_invocation_argument();
				string invocation_args;
				if(how)
					sscanf(rest, how, invocation_args);
				if(invocation_args)
					args["argument"] = invocation_args;
			}
			mixed res = invocation->perform_invocation(args);
			if(Result_Failure(res))
				if(Result_Display(res))
					owner->display(res);
				else
					owner->display(query_notify_fail());
		}
	}
}


void configure() {
	::configure();
	set_id(({ "TRAVELERS", "GUILDOBJ" }));
	set_score_prepend("You are a Traveler.");
	set_score_append("Type 'help Travelers' for information.");
}

void attach_affiliation(object who, mixed conf) {
	::attach_affiliation(who, conf);
	call_out("travelers_check_for_challenge_creator_benefits", 6);
	grant_channel(who, "traveler");
	owner->add_hook(Do_Hear, #'travelers_do_hear);
	call_out("travelers_convert_old_info", 5);
}

void detach_affiliation(object who) {
	revoke_channel(who, "traveler");
	owner->remove_hook(Do_Hear, #'travelers_do_hear);
	::detach_affiliation(who);
}

string stat() {
	string out = "\n";
	out += "{{orange}{{bright yellow}Travelers Status} ------------------------------------------------------------}\n";
	out += "{{red}Karma Phala             :} {{orange}" + Ganesha_Query_Karma_Phala(owner) + "}\n";
	string array bestowals = travelers_query_bestowals();
	out += "{{red}Permanent Bestowals     :} ";
	if(!sizeof(bestowals))
		out += "{{black}none}\n";
	else
		out += "{{orange}" + sizeof(bestowals) + "}\n    {{yellow}" + implode(sort_array(bestowals, #'>), "\n    ") + "}\n";
	string array points = map(travelers_query_conceptual_navigation_point_rooms(), (:
		if(!$1)
			return "{{dark yellow}[unused point]}";
		else
			return "{{yellow}" + project_file($1) + "}";
	:));
	out += "{{red}Conceptual Nav Points   :} ";
	if(!sizeof(points))
		out += "{{black}none}\n";
	else
		out += "{{orange}" + sizeof(points) + "}\n    " + implode(points, "\n    ") + "\n";
	string array waypoints = map(travelers_query_divine_waypoint_rooms(), (:
		if(!$1)
			return "{{dark yellow}[unused point]}";
		else
			return "{{yellow}" + project_file($1) + "}";
	:));
	out += "{{red}Divine Wayoints         :} ";
	if(!sizeof(waypoints))
		out += "{{black}none}\n";
	else
		out += "{{orange}" + sizeof(waypoints) + "}\n    " + implode(waypoints, "\n    ") + "\n";
	out += "{{orange}-----------------------------------------------------------------------------}\n";
	return out;
}
