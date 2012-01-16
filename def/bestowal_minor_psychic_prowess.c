#include <daemon.h>
#include <specialty_access.h>
#include <Travelers.h>

inherit Travelers_Definition("Bestowal");

void configure() {
	::configure();
	set_bestowal_name("minor psychic prowess");
	set_bestowal_type(Travelers_Bestowal_Type_Specialty_Access);
	set_bestowal_rarity(Rarity_Unusual);
	set_bestowal_universality(False);
	set_bestowal_value(Travelers_Bestowal_Value_Low);
	set_bestowal_description("marginally increased psychic potential");
	set_bestowal_eligibility_condition(([
		Condition_Type_Code                     : Condition_Type_Or,
		Condition_Info                          : ({
			([
				Condition_Type_Code             : Condition_Type_Affiliation,
				Condition_Info                  : "the Wild Talents",
			]),
			([
				Condition_Type_Code             : Condition_Type_Affiliation,
				Condition_Info                  : "the Void Paladins",
			]),
		}),
	]));
	set_bestowal_specialty_access(([
		Specialty_Skills                        : Skill_Class(Skill_Class_Psionic)->query_skill_class_contents()->query_skill_code(),
		Specialty_Degree                        : 1,
		Specialty_Flags                         : Specialty_Flag_Accumulative_Only,
	]));
}
