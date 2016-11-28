#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("kazar blade destruction");
    set_bestowal_type(Travelers_Bestowal_Type_Invocation);
    set_bestowal_rarity(Rarity_Common);
    set_bestowal_universality(False);
    set_bestowal_value(Travelers_Bestowal_Value_High);
    set_bestowal_eligibility_condition(([
        Condition_Type_Code             : Condition_Type_Metric,
        Condition_Info                  : "karma phala",
        Condition_Value                 : 750,
    ]));
    set_bestowal_required_bestowals(({
        "katakacha",
    }));
    set_bestowal_description("the ability to entreat Ganesha to destroy kazar blades");
}
