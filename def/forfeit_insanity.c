#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <daemon.h>
#include <mental_disorders.h>

inherit Travelers_Definition("Forfeit");

void configure() {
    ::configure();
    set_forfeit_name("insanity");
    set_forfeit_rarity(Rarity_Very_Rare);
    set_forfeit_value(Travelers_Forfeit_Value_Very_High);
    set_forfeit_initialize_description("insanity and mania");
    set_forfeit_message(([
        Message_Content                         : ({
            'a',
            ([
                Message_Content                 : ({
                    "eerie",
                }),
                Message_Senses                  : Message_Sense_Emotive,
            ]),
            "chime sound fills the", Description(Description_Type_Ambient_Medium_Colored_Name),
        }),
        Message_Senses                          : Message_Sense_Auditory,
    ]));
    set_forfeit_display(([
        Message_Content                         : ({
            0, ({ "feel", 0 }), ({ 'r', 0, "mind" }), "splinter and disintegrate",
        }),
        Message_Senses                          : Message_Sense_Spiritual | Message_Sense_Kinesthetic,
        Message_Color                           : "status: danger",
    ]));
    set_forfeit_process((:
        object who = $1->ganesha_challenge_query_owner();
        for(int i = 0, int count = fuzz(15, 0.8); i < count; i++) {
            string type = Daemon_Mental_Disorders->query_random_new_mental_disorder(who);
            if(type) {
                type->inflict_mental_disorder(who, 0, True);
            } else {
                break;
            }
        }
        return;
    :));
}
