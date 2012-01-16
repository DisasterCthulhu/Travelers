#include <daemon.h>
#include <specialty_access.h>
#include <Travelers.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("longevity");
	set_bestowal_type(Travelers_Bestowal_Type_Specialty_Access);
	set_bestowal_rarity(Rarity_Very_Rare);
	set_bestowal_universality(False);
	set_bestowal_value(Travelers_Bestowal_Value_Low);
	set_bestowal_description("longevity");
	set_bestowal_specialty_access(([
		Specialty_Skills                        : Skill_Aging_Retardation,
		Specialty_Degree                        : 15,
		Specialty_Bonus                         : 1,
		Specialty_Flags                         : Specialty_Flag_Reveal,
	]));
}
