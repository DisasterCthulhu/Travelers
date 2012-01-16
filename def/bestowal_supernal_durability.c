#include <daemon.h>
#include <specialty_access.h>
#include <Travelers.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("supernal durability");
	set_bestowal_type(Travelers_Bestowal_Type_Specialty_Access);
	set_bestowal_rarity(Rarity_Exotic);
	set_bestowal_universality(False);
	set_bestowal_value(Travelers_Bestowal_Value_High);
	set_bestowal_description("supernal durability");
	set_bestowal_specialty_access(([
		Specialty_Skills                        : Skill_Supernal_Durability,
		Specialty_Degree                        : 2,
		Specialty_Bonus                         : 2,
		Specialty_Flags                         : Specialty_Flag_Reveal,
	]));
}
