#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("conceptual navigation point");
    set_bestowal_type(Travelers_Bestowal_Type_Recurring);
    set_bestowal_rarity(Rarity_Rare);
    set_bestowal_universality(False);
    set_bestowal_value(Travelers_Bestowal_Value_Very_Low);
    set_bestowal_description("an additional conceptual navigation point");
    set_bestowal_required_bestowals(({
        "conceptual navigation"
    }));
    set_bestowal_reward_process((:
        $1->query_affiliation_link(project_control())->travelers_add_conceptual_navigation_points(1);
        return;
    :));
}
