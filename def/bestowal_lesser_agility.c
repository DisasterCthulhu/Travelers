#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("lesser agility");
    set_bestowal_aliases(({ "lesser dexterity" }));
    set_bestowal_type(Travelers_Bestowal_Type_Attribute_Adjustment);
    set_bestowal_rarity(Rarity_Very_Unusual);
    set_bestowal_universality(False);
    set_bestowal_value(Travelers_Bestowal_Value_Low);
    set_bestowal_description("a small, permanent increase in agility");
    set_bestowal_attribute_adjustment(Attr_Agi, 3);
}
