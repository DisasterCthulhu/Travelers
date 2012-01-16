#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("wealth");
	set_bestowal_type(Travelers_Bestowal_Type_Recurring);
	set_bestowal_rarity(Rarity_Rare);
	set_bestowal_universality(True);
	set_bestowal_value(Travelers_Bestowal_Value_Very_Low);
	set_bestowal_description("wealth");
	set_bestowal_reward_process((:
		object who = $1;
		who->add_wealth(semirandom(50000));
		return;
	:));
}
