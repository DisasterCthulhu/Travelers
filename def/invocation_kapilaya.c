#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Invocation");

void configure() {
    ::configure();
    set_invocation_name("kapilaya");
    set_invocation_summary("shape katakacha");
    set_invocation_key(
        "o lord Ganesha, god of molten gold and master of all colors, I offer you obeisance and ask for"
    );
    set_invocation_argument("%s");
    set_invocation_argument_description("<\"a\"/\"an\" weapon type>");
    set_invocation_argument_required(True);
    set_invocation_help(
        "Once one has summoned a katakacha, this invocation requests of Ganesha to serve as a divine intermediary "
        "between one and one's katakacha, to communicate to the katakacha the shape that one is envisioning.  The shapes "
        "that one knows are dependent on one's weapon lore."
    );
    set_invocation_cost(({
        ({ Energy_Spiritual, 50 }),
    }));
    set_invocation_bestowal("katakacha");
    set_invocation_delay(Time_Second * 4);
    set_invocation_ability_minimum(Travelers_Invocation_Ability_Minimum_Low);
    set_invocation_ability_maximum(Travelers_Invocation_Ability_Maximum_Very_High);
    set_invocation_ability_determinants(([
        Condition_Skill_Composite           : True,
        Skill_Theurgy                         : 0.40,
        Skill_Weapon_Lore                   : 0.10,
        Skill_Invocation                    : 0.50,
        ([
            Condition_Type_Code             : Condition_Type_Attribute,
            Condition_Info                  : Attr_Ego,
            Condition_Value                 : 0.65,
        ])                                  : True,
        ([
            Condition_Type_Code             : Condition_Type_Attribute,
            Condition_Info                  : Attr_Int,
            Condition_Value                 : 0.35,
        ])                                  : True,
    ]));
    set_invocation_begin_can_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        object katakacha = present("KATAKACHA", who, who);
        unless(katakacha)
            return Error(({
                who, ({ "are", who }), "not in possession of a katakacha",
            }));
        if(katakacha->query_user())
            return Error(({
                who, ({ "cannot", who }), "be holding", ({ 't', katakacha }), "while", ({ 'p', katakacha }),
                ({ "change", katakacha }), "forms",
            }));
        string arg = Process_Query_Info(dxr, "argument");
        unless(arg)
            return Error (({ "ask for what?" }));
        string orig = arg;
        if(begins_with(arg, "a ")) {
            arg = arg[2..sizeof(arg)];
        } else if(begins_with(arg, "an ")) {
            arg = arg[3..sizeof(arg)];
        } else {
            return Error(({
                "what weapon type are", who, "trying to ask for?"
            }));
        }
        if(orig != a(arg))
            return Error(({
                who, ({ "do", who }), "not know what '" + orig + "'", "is"
            }));
        object def;
        if(Is_Weapon_Type(arg))
            def = Weapon_Type(arg);
        if(def && def->query_weapon_type_code() == Weapon_Type_Shard) { // Allow default weapon type regardless of handedness
            Process_Set(dxr, Process_Target, katakacha);
            Process_Set_Info(dxr, "weapon type", def);
            return True;
        }
        if(!def || (def->query_weapon_type_identity_code() && !who->query_known(def->query_weapon_type_identity_code())))
            return Error(({
                who, ({ "do", who }), "not know what", ({ 'a', "'" + arg + "'" }), "is"
            }));
        if(def->query_weapon_type_generic())
            return Error(({
                "one must be more precise in one's communication of a desired form"
            }));
        if(def->query_weapon_type_rarity() == Rarity_Special)
            return Error(({
                "one must choose a more generic and less unique tool"
            }));
        int number_of_hands_required = Weapon_Size(def->query_weapon_type_size())->query_weapon_size_hands_required(who);
        int number_of_hands = who->existing_limb_count(Limb_Type_Hand);
        if(!number_of_hands_required)
            return Error(({
                a(def->query_weapon_type_name()), "would be too small for", who, "to use"
            }));
        if(number_of_hands_required > number_of_hands)
            return Error(({
                a(def->query_weapon_type_name()), "would be too large for", who, "to use"
            }));
        Process_Set(dxr, Process_Target, katakacha);
        Process_Set_Info(dxr, "weapon type", def);
        return True;
    :));
    set_invocation_execute_do_call((:
        descriptor dxr = $1;
        object who = Process_Query(dxr, Process_Actor);
        object katakacha = Process_Query(dxr, Process_Target);
        object def = Process_Query_Info(dxr, "weapon type");
        who->display(([
            Message_Content         : ({
                "as", 0, ({ "picture", 0 }), 'a', def->query_weapon_type_name(), ",", ({ 'p', 0 }), ({ "feel", 0 }),
                "Ganesha working from within", ({ 'o', 0 }), "to convey the image to", ({ 'r', 0, katakacha }),
            }),
            Message_Senses          : Message_Sense_Cognitive,
        ]));
        who->message(([
            Message_Content         : ({
                'a', self_color("auroric", "band of", "light"), "forms between", who, "and", katakacha,
            }),
            Message_Senses          : Message_Sense_Astral,
        ]));
        katakacha->katakacha_set_weapon_type(who, def->query_weapon_type_code());
        who->validate_equipment_usage();
        return;
    :));
}
