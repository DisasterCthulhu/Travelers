#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("kabatha");
	set_bestowal_type(Travelers_Bestowal_Type_Invocation);
	set_bestowal_rarity(Rarity_Unusual);
	set_bestowal_universality(False);
	set_bestowal_value(Travelers_Bestowal_Value_High);
	set_bestowal_required_bestowals(({
		"amphiptere",
	}));
	set_bestowal_description("the ability to entreat Ganesha for a kabatha");
}