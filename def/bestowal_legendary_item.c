#include <Travelers.h>
#include <daemon.h>
#include <Kazarzeth.h>
#include <selection.h>

inherit Travelers_Definition("Bestowal");

object generate_bestowal_legendary_item() {
    return Random_Item(({
        Select_Include_Category(Item_Category_Significant),
        Select_Adjust_Rarity(Select_Compare_Less_Equal, Rarity_Common, 0),
        Select_Adjust_Rarity(Select_Compare_Less_Equal, Rarity_Very_Unusual, 0),
        Select_Adjust_Category(Item_Category_Comestible, 0),
        Select_Adjust_Category(Item_Category_Weapon, 2.0),
        Select_Adjust_Category(Item_Category_Armour, 2.0),
        Select_Adjust_Category(Item_Category_Shield, 2.0),
        Select_Adjust_Danger(Select_Compare_Unequal, 0, 0.1),
        Select_Exclude_Material(Material_Qlippotam),
        Select_Exclude_Custom((: Is_Kazarix($1) :)),
    }));
}

void configure() {
    ::configure();
    set_bestowal_name("legendary item");
    set_bestowal_type(Travelers_Bestowal_Type_Recurring);
    set_bestowal_rarity(Rarity_Very_Unusual);
    set_bestowal_universality(True);
    set_bestowal_value(Travelers_Bestowal_Value_Moderate);
    set_bestowal_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_And,
        Condition_Info                          : ({
            ([
                Condition_Type_Code             : Condition_Type_Level,
                Condition_Value                 : Hero,
            ]),
            ([
                Condition_Type_Code             : Condition_Type_Metric,
                Condition_Info                  : "karma phala",
                Condition_Value                 : 500,
            ]),
        }),
    ]));
    set_bestowal_description("an item of legend");
    set_bestowal_reward_process((:
        object who = $1;
        object what = generate_bestowal_legendary_item();
        if(!what) {
            warn("failed to generate legendary item");
            return;
        }
        bestowal_deliver(who, what);
        return;
    :));
}
