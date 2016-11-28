#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Invocation");

string array query_energy_types_available(object who) {
    string array out = ({});
    object link = who->query_affiliation_link(project_control());
    unless(link)
        return out;
    string array bestowals = link->travelers_query_bestowals();
    foreach(string bestowal : bestowals) {
        string array new_types = Travelers_Bestowal(bestowal)->query_bestowal_katakacha_damage_types_and_combos();
        if(new_types)
            out += new_types;
    }
    return sort_array(unique(out), #'>);
}

void configure() {
    ::configure();
    set_invocation_name("gajakarnikaya");
    set_invocation_summary("enhance katakacha");
    set_invocation_key(
        "o lord Ganesha, elephant god whose ears are constantly fanning, who hears all seven chakras and all seventy two thousand "
        "nadis, listen to Brahman, listen to Shakti, and find me"
    );
    set_invocation_argument("%s");
    set_invocation_argument_description("<[\"any\"]/[\"a\"/\"an\" enhancement type]> principle");
    set_invocation_argument_required(True);
    set_invocation_help((:
        "Once one has a katakacha one can learn to enhance it with cosmic energies such as one needs in various "
        "circumstances."
        "\n\f"
        "The enhancements you currently know are: " + list_array(query_energy_types_available($1)) + "."
    :));
    set_invocation_cost(({
        ({ Energy_Spiritual, 100 }),
    }));
    set_invocation_bestowal("katakacha");
    set_invocation_delay(Time_Second * 2);
    set_invocation_ability_minimum(Travelers_Invocation_Ability_Minimum_Low);
    set_invocation_ability_maximum(Travelers_Invocation_Ability_Maximum_High);
    set_invocation_begin_can_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        object katakacha = present("KATAKACHA", who, who);
        unless(katakacha)
            return Error(({
                who, ({ "are", who }), "not in possession of a katakacha",
            }));
        if(!katakacha->query_user())
            return Error(({
                who, ({ "must", who }), "be holding", ({ 't', katakacha }), "to enhance it with energy"
            }));
        string arg = Process_Query_Info(dxr, "argument");
        string what;
        if(!arg || (sscanf(arg, "%s principle", what) != 1))
            return Error(({
                "what principle are", who, "trying to enhance", ({ 'r', who, katakacha }), "with?"
            }));
        string no_article = what;
        if(begins_with(no_article, "a ")) {
            no_article = no_article[2..sizeof(no_article)];
        } else if(begins_with(no_article, "an ")) {
            no_article = no_article[3..sizeof(no_article)];
        } else if(no_article == "any") {
            no_article = random_element(query_energy_types_available(who));
            what = a(no_article);
        } else {
            return Error(({
                "what principle are", who, "trying to enhance", ({ 'r', who, katakacha }), "with?"
            }));
        }
        string array use = map(query_energy_types_available(who), (: a($1) :));
        unless(member(use, what) != Null)
            return Error(({
                who, ({ "do", who }), "not know of any '" + what + "' principle",
            }));
        string array types;
        if(Is_Damage_Combo(no_article))
            types = map(Damage_Combo(no_article)->query_damage_combo_types(), Damage_Type_Name_Lookup);
        else if(Is_Damage_Type(no_article))
            types = ({ no_article });
        Process_Set(dxr, Process_Target, katakacha);
        Process_Set_Info(dxr, "damage types", types);
        return True;
    :));
    set_invocation_execute_do_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        object katakacha = Process_Query(dxr, Process_Target);
        string array types = Process_Query_Info(dxr, "damage types");
        who->display(([
            Message_Content         : ({
                ({ 'p', 0 }), ({ "feel", 0 }), "Ganesha working within", ({ 'o', 0 }), ", tuning", ({ 'o', 0 }),
                "to strange cosmic energies, and channeling those energies into", ({ 'r', 0, katakacha })
            }),
            Message_Senses          : Message_Sense_Cognitive,
        ]));
        string color = Damage_Type_Effect(types)[Damage_Effect_Color];
                unless(color)
                        color = "shimmering";
        who->message(([
            Message_Content         : ({
                'a', self_color(color, "nimbus of", "light"), "surrounds both", who, "and", katakacha,
            }),
            Message_Senses          : Message_Sense_Astral,
        ]));
        katakacha->katakacha_set_enhancement(who, types);
        return;
    :));
}
