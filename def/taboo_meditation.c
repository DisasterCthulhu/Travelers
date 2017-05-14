#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <daemon.h>

inherit Travelers_Definition("Taboo");

void travelers_taboo_meditation_do_begin_process(mapping args) {
    object who = args["who"];
    descriptor process = args["process"];
    if(Process_Query(process, Process_Type) != Process_Type_Meditation)
        return;
    object challenge = Travelers_Find_Challenge(who);
    taboo_violation(challenge);
}

void configure() {
    ::configure();
    set_taboo_name("meditation");
    set_taboo_rarity(Rarity_Unusual);
    set_taboo_value(Travelers_Taboo_Value_Very_Low);
    set_taboo_initialize_description("meditation");
    set_taboo_overcome_description("meditation");
    add_taboo_hook(Do_Begin_Process, #'travelers_taboo_meditation_do_begin_process);
    set_taboo_attach_process((:
        object who = $1->ganesha_challenge_query_owner();
        who->interrupt_current_process_of_type(Process_Type_Meditation);
    :));
}
