#include <Travelers.h>
#include <daemon.h>
#include <tables.h>

inherit "/std/def/showable";

private string show_user_challenges(object who) {
	object array list = filter(active_incarnoi(), (: $1->query_real_name() && !is_developer($1) :));
	list =  sort_array(list, (: compare($1->query_real_name(), $2->query_real_name()) :));
	string array col_1 = ({});
	string array col_2 = ({});
	string array col_3 = ({});
	string array col_4 = ({});
	string array col_5 = ({});
	string array col_6 = ({});
	string array col_7 = ({});
	string array col_8 = ({});
	foreach(object user : list) {
		col_1 += ({ user->query_full_name(who) });
		col_2 += ({ is_developer(user) ? query_staff_title(user)[..2] : user->query_level()});
		col_3 += ({ user->query_affiliation("the Travelers") ? "{{white}yes}" : "{{black}no}" });
		object challenge = Travelers_Find_Challenge(user);
		if(challenge) {
			col_4 += ({ challenge->ganesha_challenge_query_requirement() });
			col_5 += ({ challenge->ganesha_challenge_query_obstacle() });
			col_6 += ({ challenge->ganesha_challenge_query_taboo() || "" });
			col_7 += ({ challenge->ganesha_challenge_query_forfeit() || "" });
			col_8 += ({ challenge->query_info("Last_Requirement_Notification") + "%" });
		} else {
			col_4 += ({ "" });
			col_5 += ({ "" });
			col_6 += ({ "" });
			col_7 += ({ "" });
			col_8 += ({ "" });
		}
	}
	return Table_Render(([
		Table_Columns        : ({
			([
				Column_Items : col_1,
				Column_Head  : "User",
			]),
			([
				Column_Items : col_2,
				Column_Head  : "Lvl",
				Column_Align : Column_Align_Right,
			]),
			([
				Column_Items : col_3,
				Column_Head  : "Trv",
				Column_Align : Column_Align_Right,
			]),
			([
				Column_Items : col_4,
				Column_Head  : "Requirement",
				Column_Align : Column_Align_Right,
			]),
			([
				Column_Items : col_5,
				Column_Head  : "Obstacle",
				Column_Align : Column_Align_Right,
			]),
			([
				Column_Items : col_6,
				Column_Head  : "Taboo",
				Column_Align : Column_Align_Right,
			]),
			([
				Column_Items : col_7,
				Column_Head  : "Forfeit",
				Column_Align : Column_Align_Right,
			]),
			([
				Column_Items : col_8,
				Column_Head  : "Done",
				Column_Align : Column_Align_Right,
			]),
		}),
		Table_Title          : "User Challenge Summary",
	]), who);
}

void configure() {
	::configure();
	set_interaction_name("user challenges");
	set_interaction_group(Interaction_Group_Informational);
	set_interaction_ooc(True);
	set_interaction_developer(True);
	set_interaction_suppress_project_adaptation(True);
	set_interaction_help(
		"Displays a report on player challenges."
	);
	set_interaction_layout((:
		return show_user_challenges($1);
	:));
}
