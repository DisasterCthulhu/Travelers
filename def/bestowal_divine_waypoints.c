#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("divine waypoints");
	set_bestowal_type(Travelers_Bestowal_Type_Invocation);
	set_bestowal_rarity(Rarity_Unusual);
	set_bestowal_universality(False);
	set_bestowal_value(Travelers_Bestowal_Value_High);
	set_bestowal_eligibility_condition(([
	    Condition_Type_Code             : Condition_Type_And,
	    Condition_Info                  : ({
	        ([
	            Condition_Type_Code     : Condition_Type_Metric,
	            Condition_Info          : "unused conceptual navigation points",
	            Condition_Value         : 20,
	        ]),
	        ([
	            Condition_Type_Code     : Condition_Type_Metric,
	            Condition_Info          : "conceptual navigation points",
	            Condition_Value         : 60,
	        ]),
	    }),
	]));
	set_bestowal_description("the ability to entreat Ganesha for travel via divine intervention");
	set_bestowal_reward_process((:
        $1->query_affiliation_link(project_control())->travelers_spend_conceptual_navigation_points(20);
		$1->query_affiliation_link(project_control())->travelers_add_divine_waypoints(2);
		return;
	:));
}
