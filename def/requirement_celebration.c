#include <Travelers.h>
#include <daemon.h>
#include <experience.h>
#include <regexp.h>

inherit Travelers_Definition("Requirement");

nosave private string ganesha_pattern;

void travelers_requirement_celebration_do_speak(descriptor dxr) {
    if(Speech_Flag_Check(dxr, Speech_Flag_OOC))
        return;
    object who = Speech_Query(dxr, Speech_Speaker);
    if(!who->query_intoxication())
        return;
    string msg = Speech_Query(dxr, Speech_Original_Message);
    if(!regmatch(msg, ganesha_pattern))
        return;
    object env = environment(who);
    if(!env)
        return;
    int count = 0;
    for(object obj = first_inventory(env); obj; obj = next_inventory(obj))
        if(obj != who && obj->is_character() && obj->sentience() && obj->sentience()->query_sentience_emotion() && obj->query_intoxication())
            count++;
    if(count < 3)
        return;
    object challenge = Travelers_Find_Challenge(who);
    if(challenge->ganesha_challenge_query_taboo() == "speaking other than by shouting" && Speech_Query(dxr, Speech_Command) != Speech_Command_Shout)
        return;
    challenge->ganesha_challenge_overcome();
}

void preinit() {
    ::preinit();
    if(regexp_package() == RE_PCRE)
        ganesha_pattern = "\\b(?i:Ganesha)\\b";
    else
        ganesha_pattern = "\\<[Gg][Aa][Nn][Ee][Ss][Hh][Aa]\\>";
}

void configure() {
    ::configure();
    set_creator("chaos");
    set_requirement_name("celebration");
    set_requirement_rarity(Rarity_Very_Rare);
    set_requirement_value(Travelers_Requirement_Value_Low);
    set_requirement_initialize_description("speaking Ganesha's name aloud while intoxicated and with at least three other celebrants nearby who are also intoxicated");
    set_requirement_overcome_description("celebrated in Ganesha's name");
    add_requirement_hook(Do_Speak, #'travelers_requirement_celebration_do_speak);
    set_requirement_eligibility_condition(([
        Condition_Type_Code             : Condition_Type_And,
        Condition_Info                  : ({
            ([
                Condition_Type_Code     : Condition_Type_Trait,
                Condition_Info          : Trait_Diet,
                Condition_Value         : Diet_Ametabolic,
                Condition_Flags         : Condition_Flag_Inverse | Condition_Flag_Unmodified_Values,
            ]),
            ([
                Condition_Type_Code     : Condition_Type_Speech,
                Condition_Flags         : Condition_Flag_Unmodified_Values,
            ]),
        }),
    ]));

}
