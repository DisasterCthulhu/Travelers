#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <die.h>

inherit Travelers_Definition("Forfeit");

void configure() {
	::configure();
	set_creator("chaos");
	set_forfeit_name("dismemberment");
	set_forfeit_rarity(Rarity_Unusual);
	set_forfeit_value(Travelers_Forfeit_Value_Low);
	set_forfeit_initialize_description("dismemberment");
	set_forfeit_eligibility_condition(([
		Condition_Type_Code                     : Condition_Type_Limb_Class,
		Condition_Info                          : Limb_Class_Standard,
	]));
	set_challenge_component_selection_adjustments(([
		"becoming crippled"                     : 0.00,
		"dying"                                 : 0.00,
	]));
	set_forfeit_process((:
		object who = $1->ganesha_challenge_query_owner();
		int array limbs = ({});
		foreach(int ix : who->query_limb_indices())
			if(who->query_limb_class(ix) == Limb_Class_Standard && who->query_limb_status(ix) != Limb_Status_Amputated && who->query_limb_parent(ix) != Null)
				limbs += ({ ix });
		while(sizeof(limbs)) {
			int limb = random_element(limbs);
			who->sever_limb(limb);
			if(!random(2))
				break;
			limbs -= ({ limb });
		}
		return;
	:));
}
