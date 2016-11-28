#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("major intelligence");
    set_bestowal_type(Travelers_Bestowal_Type_Attribute_Adjustment);
    set_bestowal_rarity(Rarity_Rare);
    set_bestowal_universality(False);
    set_bestowal_value(Travelers_Bestowal_Value_Moderate);
    set_bestowal_description("a permanent increase in intelligence");
    set_bestowal_attribute_adjustment(Attr_Int, 5);
}
