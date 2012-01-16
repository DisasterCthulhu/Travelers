#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("advanced heat enhancements");
	set_bestowal_type(Travelers_Bestowal_Type_Katakacha_Enhancement);
	set_bestowal_rarity(Rarity_Very_Unusual);
	set_bestowal_universality(False);
	set_bestowal_value(Travelers_Bestowal_Value_Low);
	set_bestowal_required_bestowals(({
		"katakacha",
		"temperature enhancements",
	}));
	set_bestowal_description("the ability to enhance your katakacha with esoteric principles of heat");
	set_bestowal_katakacha_damage_types(({
		({ "order", "heat" }),
		({ "fire", "heat", "lightning", "heat" }),
		({ "order", "fire", "heat", "lightning", }),
	}));
}
