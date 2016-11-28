#include <Travelers.h>
#include <compare.h>
#include <config.h>
#include <daemon.h>
#include <tables.h>

inherit "/std/def/showable";

private string show_user_karma_phala(object who) {
    object array list = filter(active_incarnoi(), (: $1->query_real_name() && !is_developer($1) :));
    list =  sort_array(list, (: compare($1->query_level(), $2->query_level()) || compare($1->query_real_name(), $2->query_real_name()) :));
    string array col_1 = ({});
    string array col_2 = ({});
    string array col_3 = ({});
    string array col_4 = ({});
    foreach(object user : list) {
        col_1 += ({ user->query_full_name(who) });
        col_2 += ({ is_developer(user) ? query_staff_title(user)[0..2] : user->query_level()});
        col_3 += ({ user->query_affiliation("the Travelers") ? "{{white}yes}" : "{{black}no}" });
        int kp = Metric_Retrieve("karma phala", user);
        col_4 += ({ (kp > 0 ? "{{white}" : "{{black}") + printable(kp) + "}" });
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
                Column_Head  : "KP",
                Column_Align : Column_Align_Right,
            ]),
        }),
        Table_Title          : "User Karma Phala Summary",
        Table_Flags          : Table_Flag_No_Expansion,
    ]), who);
}

void configure() {
    ::configure();
    set_interaction_name("user karma phala");
    set_interaction_group(Interaction_Group_Informational);
    set_interaction_ooc(True);
    set_interaction_developer(True);
    set_interaction_suppress_project_adaptation(True);
    set_interaction_help(
        "Displays a report on player karma phala."
    );
    set_interaction_layout((:
        return show_user_karma_phala($1);
    :));
}
