#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("goetic energy");
    set_bestowal_type(Travelers_Bestowal_Type_Recurring);
    set_bestowal_rarity(Rarity_Common);
    set_bestowal_value(Travelers_Bestowal_Value_Very_Low);
    set_bestowal_description("goetic energy");
    set_bestowal_required_bestowals(({
        "brahmanda-prakasa",
    }));
    set_bestowal_reward_process((:
        object who = $1;
        who->add_energy_amount(Energy_Goetic, who->query_energy_maximum(Energy_Goetic));
        return;
    :));
}
