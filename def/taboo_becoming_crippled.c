#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_becoming_crippled_do_have_limb_disabled(mapping args) {
    object who = args["who"];
    if(who->query_dying())
        return;
    int limb = args["limb"];
    if(who->query_limb_class(limb) != Limb_Class_Standard)
        return;
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
    ::configure();
    set_taboo_name("becoming crippled");
    set_taboo_rarity(Rarity_Unusual);
    set_taboo_value(Travelers_Taboo_Value_Moderate);
    set_taboo_eligibility_condition(([
        Condition_Type_Code                     : Condition_Type_Limb_Class,
        Condition_Info                          : Limb_Class_Standard,
    ]));
    set_taboo_initialize_description("becoming crippled by losing functionality in one's extremities");
    set_taboo_overcome_description("becoming crippled");
    set_challenge_component_selection_adjustments(([
        "charity"                               : 0.05,
        "experience"                            : 1.00,
        "exploration"                           : 0.10,
        "pilgrimage"                            : 0.25,
        "quest"                                 : 0.75,
        "skill experience"                      : 0.05,
        "neophyte guidance"                     : 0.50,
        //
        "frailty"                               : 1.75,
    ]));
    add_taboo_hook(Do_Have_Limb_Disabled, #'travelers_taboo_becoming_crippled_do_have_limb_disabled);
}
