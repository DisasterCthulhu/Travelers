#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Forfeit");

void configure() {
    ::configure();
    set_creator("chaos");
    set_forfeit_name("blindness");
    set_forfeit_rarity(Rarity_Unusual);
    set_forfeit_value(Travelers_Forfeit_Value_Low);
    set_forfeit_initialize_description("blindness");
    set_forfeit_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_Sight,
    ]));
    set_challenge_component_selection_adjustments(([
        "conceptual navigation"                 : 0.00,
    ]));
    set_forfeit_process((:
        object who = $1->ganesha_challenge_query_owner();
        who->blind_living(semirandom(7200, 6), False, ({ "divine" }));
        return;
    :));
}
