#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("divine waypoint");
	set_bestowal_type(Travelers_Bestowal_Type_Recurring);
	set_bestowal_rarity(Rarity_Unusual);
	set_bestowal_universality(False);
	set_bestowal_value(Travelers_Bestowal_Value_Moderate);
	set_bestowal_eligibility_condition(([
	    Condition_Type_Code             : Condition_Type_And,
	    Condition_Info                  : ({
	        ([
	            Condition_Type_Code     : Condition_Type_Metric,
	            Condition_Info          : "unused conceptual navigation points",
	            Condition_Value         : 10,
	        ]),
	        ([
	            Condition_Type_Code     : Condition_Type_Metric,
	            Condition_Info          : "conceptual navigation points",
	            Condition_Value         : 50,
	        ]),
	    }),
	]));
    set_bestowal_required_bestowals(({
        "divine waypoints"
    }));
	set_bestowal_description("an additional divine waypoint");
	set_bestowal_reward_process((:
	    $1->query_affiliation_link(project_control())->travelers_spend_conceptual_navigation_points(10);
		$1->query_affiliation_link(project_control())->travelers_add_divine_waypoints(1);
		return;
	:));
}
