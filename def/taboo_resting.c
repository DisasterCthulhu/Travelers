#include <daemon.h>
#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_resting_do_begin_process(mapping args) {
    object who = args["who"];
    descriptor process = args["process"];
    if(Process_Query(process, Process_Type) != Process_Type_Resting)
        return;
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
    ::configure();
    set_taboo_name("resting");
    set_taboo_rarity(Rarity_Unusual);
    set_taboo_value(Travelers_Taboo_Value_Very_Low);
    set_taboo_initialize_description("resting or meditating");
    set_taboo_overcome_description("resting");
    add_taboo_hook(Do_Begin_Process, #'travelers_taboo_resting_do_begin_process);
}
