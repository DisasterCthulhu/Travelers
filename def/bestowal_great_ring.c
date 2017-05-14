#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

nosave private mixed rings;

mixed bestowal_rings() {
    return bestowal_find_safe_items(Effect_Class_Great, Item_Category_Armour | Item_Category_Hand_Accessory | Item_Category_Magical);
}

void configure() {
    ::configure();
    set_bestowal_name("great ring");
    set_bestowal_type(Travelers_Bestowal_Type_Recurring);
    set_bestowal_rarity(Rarity_Very_Rare);
    set_bestowal_universality(True);
    set_bestowal_value(Travelers_Bestowal_Value_Very_High);
    set_bestowal_description("a ring of great power");
    set_bestowal_reward_process((:
        object who = $1;
        string ring_blueprint = random_element(rings ||= bestowal_rings());
        if(!ring_blueprint) {
            warn("failed to select ring blueprint");
            return;
        }
        bestowal_deliver(who, ring_blueprint);
        return;
    :));
}
