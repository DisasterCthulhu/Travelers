#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <daemon.h>

inherit Travelers_Definition("Forfeit");

status forfeit_destitution_is_ruinable_item(object what, object who, string real_name, string atman_name) {
    if(!what->is_item())
        return False;
    if(!environment(what))
        return False;
    mapping keep_tracking = what->query_info("Atman_Keep_Tracking");
    if(!sizeof(keep_tracking))
        return False;
    if(sizeof(keep_tracking[atman_name]) < 2)
        return False;
    if(keep_tracking[atman_name][0] != real_name)
        return False;
    if(time() - Time_Week > keep_tracking[atman_name][1])
        return False;
    if(what->query_auto_keep(who))
        return False;
    if(what->drop())
        return False;
    if(what->query_indestructible())
        return False;
    return True;
}

object array forfeit_destitution_find_items_to_ruin(object who) {
    // i got chaos's approval to use this function in production code.  just because it's okay here
    // does not mean that it's okay in general.  please check with him before implementing it anywhere.
    // --twi
    return find_all_objects(#'forfeit_destitution_is_ruinable_item, who, who->query_real_name(), who->query_atman_name());
}

void configure() {
    ::configure();
    set_forfeit_name("destitution");
    set_forfeit_rarity(Rarity_Very_Rare);
    set_forfeit_value(Travelers_Forfeit_Value_Very_High);
    set_forfeit_initialize_description("destitution");
    set_forfeit_process((:
        object who = $1->ganesha_challenge_query_owner();
        int wealth = who->query_wealth();
        if(wealth) {
            who->set_wealth(semirandom(wealth / 10));
            who->display(([
                Message_Content                 : ({
                    0, ({ "feel", 0 }), ({ 'r', 0, "purse" }), "lighten dramatically",
                }),
                Message_Senses                  : Message_Sense_Cognitive | Message_Sense_Kinesthetic,
                Message_Color                   : "status: danger",
            ]));
        }
        int savings = who->query_info("SAVINGS");
        if(savings)
            who->set_info("SAVINGS", semirandom(savings / 2));
        object array ruinable = forfeit_destitution_find_items_to_ruin(who);
        int ruin_count = fuzz(round(diminishing_returns(sizeof(ruinable), 0.5)));
        if(ruin_count > 0) {
            object array to_ruin = ({});
            while(ruin_count) {
                object what = random_element(ruinable);
                ruinable -= ({ what });
                to_ruin += ({ what });
                ruin_count--;
            }
            descriptor message = Message(([
                Message_Content                 : ({
                    0, ({ "collapse", 0 }), "upon", ({ 'l', 0 }), "and", ({ "vanish", 0 }),
                    "in a ", self_color("iridescent", "wink of", "light")
                }),
                Message_Senses                  : Message_Sense_Visual | Message_Sense_Astral,
                Message_Flags                   : Message_Flag_Environment_User,
            ]));
            foreach(object what : to_ruin) {
                to_ruin->user_message(message);
                to_ruin->remove();
            }
        }
        return;
    :));
}
