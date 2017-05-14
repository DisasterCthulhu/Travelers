#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
    ::configure();
    set_bestowal_name("minor vitality");
    set_bestowal_aliases(({ "minor constitution" }));
    set_bestowal_type(Travelers_Bestowal_Type_Attribute_Adjustment);
    set_bestowal_rarity(Rarity_Unusual);
    set_bestowal_universality(False);
    set_bestowal_value(Travelers_Bestowal_Value_Very_Low);
    set_bestowal_description("a slight, permanent increase in vitality");
    set_bestowal_attribute_adjustment(Attr_Vit, 1);
}
