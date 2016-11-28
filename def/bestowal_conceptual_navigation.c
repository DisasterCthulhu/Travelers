#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("conceptual navigation");
    set_bestowal_type(Travelers_Bestowal_Type_Invocation);
    set_bestowal_rarity(Rarity_Common);
    set_bestowal_universality(False);
    set_bestowal_value(Travelers_Bestowal_Value_High);
    set_bestowal_description("the ability to entreat Ganesha for travel via conceptual navigation");
    set_bestowal_reward_process((:
        $1->query_affiliation_link(project_control())->travelers_add_conceptual_navigation_points(2);
        return;
    :));
}
