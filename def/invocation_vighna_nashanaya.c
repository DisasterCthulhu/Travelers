#include <Travelers.h>
#include <daemon.h>
#include <conceptual_navigation.h>

inherit Travelers_Definition("Invocation");

void configure() {
    ::configure();
    set_invocation_name("vighna nashanaya");
    set_invocation_summary("list conceptual navigation points");
    set_invocation_key(
        "o Ganesha, lord of hosts, who dwells at the base of my spine, who tastes the kapitta and the jamboophala and remembers "
        "all things, reveal the paths you have opened to me"
    );
    set_invocation_help(
        "This invocation requests of Ganesha that he reminds you of the locations that you have designated as landmarks on your "
        "journey."
    );
    set_invocation_cost(({
        ({ Energy_Spiritual, 10 })
    }));
    set_invocation_bestowal("conceptual navigation");
    set_invocation_delay(Time_Second * 2);
    set_invocation_ability_determinants(([
        Condition_Skill_Composite           : True,
        Skill_Invocation                    : 1.00,
        ([
            Condition_Type_Code             : Condition_Type_Attribute,
            Condition_Info                  : Attr_Ego,
            Condition_Value                 : 1.00,
        ])                                  : True,
    ]));
    set_invocation_execute_do_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        who->display(([
            Message_Content         : ({
                0, ({ "sense", 0 }), "knowledge and memories of", ({ 'r', 0, "travels" }), ", past and future, flow from Ganesha into",
                ({ 'r', 0, "mind" }),
            }),
            Message_Senses          : Message_Sense_Cognitive,
        ]));
        object array points = who->query_affiliation_link(project_control())->travelers_query_conceptual_navigation_point_rooms();
        int count = 1;
        string array ordinals = ({});
        string array room_names = ({});
        string array area_names = ({});
        foreach(object point : points) {
            ordinals += ({ "{{blue}" + numerical(count, Numerical_Flag_Ordinal) + "}" });
            if(!point) {
                room_names += ({ "{{black}not yet specified}" });
                area_names += ({ "{{black}N/A}" });
            } else {
                room_names += ({ "{{green}" + point->query_short() + "}" });
                string area_name = point->project_control()->query_area();
                if(area_name)
                    area_names += ({ "{{orange}" + area_name + "}" });
                else
                    area_names += ({ "{{black}N/A}" });
            }
            count++;
        }
        who->display(Table_Render(([
            Table_Columns           : ({
                ([
                    Column_Items    : ordinals,
                    Column_Head     : "{{violet}point}",
                ]),
                ([
                    Column_Items    : room_names,
                    Column_Head     : "{{violet}specific place}",
                ]),
                ([
                    Column_Items    : area_names,
                    Column_Head     : "{{violet}greater area}",
                ]),
            }),
            Table_Title          : "{{yellow}Traveler Conceptual Navigation Points}",
            Table_Flags          : Table_Flag_No_Expansion,
        ]), who));
        return;
    :));
}
