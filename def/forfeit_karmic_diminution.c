#include <Travelers.h>
#include <add_experience.h>
#include <comestible.h>
#include <conditions.h>
#include <daemon.h>

inherit Travelers_Definition("Forfeit");

void configure() {
    ::configure();
    set_forfeit_name("karmic diminution");
    set_forfeit_rarity(Rarity_Unusual);
    set_forfeit_value(Travelers_Forfeit_Value_Very_Low);
    set_forfeit_initialize_description("a loss of karma");
    set_forfeit_message(([
        Message_Content                         : ({
            "the scent of wildflowers drifts through the",
            Description(Description_Type_Ambient_Medium_Colored_Name),
        }),
        Message_Senses                          : Message_Sense_Olfactory,
    ]));
    set_forfeit_display(([
        Message_Content                         : ({
            0, ({ "feel", 0 }), "strangely out of sync with the world"
        }),
        Message_Senses                          : Message_Sense_Spiritual,
        Message_Color                           : "status: loss",
    ]));
    set_forfeit_process((:
        object who = $1->ganesha_challenge_query_owner();
        Ganesha_Add_Karma_Phala(who, -fuzz(100));
        return;
    :));
}
