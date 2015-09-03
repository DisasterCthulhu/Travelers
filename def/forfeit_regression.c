#include <Travelers.h>
#include <add_experience.h>
#include <comestible.h>
#include <conditions.h>
#include <daemon.h>
#include <experience.h>

inherit Travelers_Definition("Forfeit");

void configure() {
	::configure();
	set_forfeit_name("regression");
	set_forfeit_rarity(Rarity_Very_Unusual);
	set_forfeit_value(Travelers_Forfeit_Value_High);
	set_forfeit_initialize_description("regression to a less developed state");
    set_forfeit_message(([
        Message_Content                         : ({
            'a',
            ([
                Message_Content                 : ({
                    "beautiful",
                }),
                Message_Senses                  : Message_Sense_Emotive,
            ]),
            "pure tone fills the", Description(Description_Type_Ambient_Medium_Colored_Name),
        }),
        Message_Senses                          : Message_Sense_Auditory,
    ]));
    set_forfeit_display(([
        Message_Content                         : ({
            0, ({ "feel", 0 }), "a horrible combination of innocence and loss as some of", ({ 'r', 0, "accumulated wisdom" }),
            "recedes from", ({ 'r', 0, "mind" }),
        }),
        Message_Senses                          : Message_Sense_Spiritual | Message_Sense_Cognitive,
        Message_Color                           : "status: loss",
    ]));
	set_forfeit_process((:
	    object who = $1->ganesha_challenge_query_owner();
	    int xp_loss = Experience_Reward(Effect_Class_Great, who);
	    who->add_experience(-xp_loss, Add_Experience_Flag_Unsplit | Add_Experience_Flag_Suppress_Level_Adjustment | Add_Experience_Flag_Emotion);
        int level = Experience_Level(who->query_experience());
        if(level < who->query_level())
            who->set_level(level);
        return;
	:));
}
