#include <Travelers.h>
#include <daemon.h>
#include <Catelii.h>

inherit Travelers_Definition("Bestowal");

internal mixed potions;

mixed bestowal_potions() {
    mixed out = bestowal_find_safe_master_items(LS_Potion("great_"), Item_Category_Comestible | Item_Category_Drink | Item_Category_Magical);
    out += ({
        LS_Potion("heroism"),
        Catelii_Comestible("metacognition_potion"),
        Catelii_Comestible("powercraft_potion"),
        Catelii_Comestible("spellcraft_potion"),
        Catelii_Comestible("revitalization_potion"),
    });
	return out;
}

void configure() {
	::configure();
	set_bestowal_name("great potion");
	set_bestowal_type(Travelers_Bestowal_Type_Recurring);
	set_bestowal_rarity(Rarity_Very_Unusual);
	set_bestowal_universality(True);
	set_bestowal_value(Travelers_Bestowal_Value_Very_Low);
	set_bestowal_description("a potion of great power");
    set_bestowal_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_Or,
        Condition_Flags                         : Condition_Flag_Inverse,
        Condition_Info                          : ({
            ([
                Condition_Type_Code             : Condition_Type_Trait,
                Condition_Info                  : Trait_Diet,
                Condition_Value                 : Diet_Incendivore,
            ]),
            ([
                Condition_Type_Code             : Condition_Type_Trait,
                Condition_Info                  : Trait_Diet,
                Condition_Value                 : Diet_Photovore,
            ]),
            ([
                Condition_Type_Code             : Condition_Type_Trait,
                Condition_Info                  : Trait_Diet,
                Condition_Value                 : Diet_Sanguivore,
            ]),
            ([
                Condition_Type_Code             : Condition_Type_Trait,
                Condition_Info                  : Trait_Diet,
                Condition_Value                 : Diet_Ametabolic,
            ]),
        }),
    ]));
	set_bestowal_reward_process((:
		object who = $1;
		string potion_blueprint = random_element(potions ||= bestowal_potions());
		if(!potion_blueprint) {
			warn("failed to select potion blueprint");
			return;
		}
		bestowal_deliver(who, potion_blueprint);
		return;
	:));
}
