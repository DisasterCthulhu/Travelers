#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("energize chakras");
    set_bestowal_type(Travelers_Bestowal_Type_Recurring);
    set_bestowal_rarity(Rarity_Common);
    set_bestowal_universality(True);
    set_bestowal_value(Travelers_Bestowal_Value_Very_Low);
    set_bestowal_description("chakra energy");
    set_bestowal_reward_process((:
        object who = $1;
        string array chakras = get_dir(LS_Chakra("*.c"), GETDIR_PATH);
        foreach(string chakra : chakras)
            chakra->energize_chakra(who, 1000);
        return;
    :));
}
