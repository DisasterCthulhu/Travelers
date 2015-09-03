#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("fire protection");
	set_bestowal_type(Travelers_Bestowal_Type_Kabatha_Resistance);
	set_bestowal_rarity(Rarity_Very_Unusual);
	set_bestowal_universality(False);
	set_bestowal_value(Travelers_Bestowal_Value_Low);
	set_bestowal_required_bestowals(({
		"kabatha",
	}));
	set_bestowal_description("an enchantment to your vahana's kabatha, granting it with " + query_bestowal_name());
	set_bestowal_kabatha_resistances(({ "fire" }));
}