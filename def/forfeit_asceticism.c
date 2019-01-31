#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <daemon.h>

inherit Travelers_Definition("Forfeit");

void configure() {
    ::configure();
    set_forfeit_name("asceticism");
    set_forfeit_rarity(Rarity_Common);
    set_forfeit_value(Travelers_Forfeit_Value_Low);
    set_forfeit_initialize_description("starvation and exhaustion");
    set_forfeit_message(([
        Message_Content                         : ({
            'a', "{{sickly-veined white}aura of light} surrounds", 0, "briefly",
        }),
        Message_Senses                          : Message_Sense_Astral,
    ]));
    set_forfeit_display(([
        Message_Content                         : ({
            0, ({ "feel", 0 }), "abruptly starved and exhausted, but somehow enlightened",
        }),
        Message_Senses                          : Message_Sense_Metabolic,
        Message_Color                           : "status: danger",
    ]));
    set_forfeit_process((:
        object who = $1->ganesha_challenge_query_owner();
        who->set_energy_amount(Energy_Metabolic, 0);
        who->set_hunger(500);
        who->set_thirst(500);
        int duration = semirandom(Time_Minute * 5);
        who->add_trait_modifier(([
            Modifier_Index                      : Trait_Diet,
            Modifier_Specification              : Diet_Nihilvore,
            Modifier_Duration                   : duration,
            Modifier_Amount                     : 10,
            Modifier_Remove_Display             : ({
                Message(([
                    Message_Content             : ({
                        0, ({ "feel", 0 }), ({ 'r', 0, "metabolism" }), "return to normal",
                    }),
                    Message_Senses              : Message_Sense_Metabolic,
                    Message_Color               : "status: recovery",
                ])),
            }),
        ]));
        return;
    :));
}
