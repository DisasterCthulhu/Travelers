#include <daemon.h>
#include <services_master.h>
#include <Travelers.h>

inherit Travelers_Definition("Bestowal");

internal mixed rings;

mixed bestowal_rings() {
	return bestowal_find_safe_master_items(LS_Ring("major_"), Item_Category_Armour | Item_Category_Hand_Accessory | Item_Category_Magical);
}

void configure() {
	::configure();
	set_bestowal_name("major ring");
	set_bestowal_type(Travelers_Bestowal_Type_Recurring);
	set_bestowal_rarity(Rarity_Very_Unusual);
	set_bestowal_universality(True);
	set_bestowal_value(Travelers_Bestowal_Value_Moderate);
	set_bestowal_description("a ring of major power");
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
