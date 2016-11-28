#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("advanced challenges");
    set_bestowal_type(Travelers_Bestowal_Type_Invocation);
    set_bestowal_rarity(Rarity_Very_Common);
    set_bestowal_universality(False);
    set_bestowal_value(Travelers_Bestowal_Value_High);
    set_bestowal_eligibility_condition(([
        Condition_Type_Code             : Condition_Type_Metric,
        Condition_Info                  : "karma phala",
        Condition_Value                 : 1500,
    ]));
    set_bestowal_description("the ability to entreat Ganesha to divest an advanced challenge");
}
