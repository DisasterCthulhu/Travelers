#include <Travelers.h>
#include <item.h>
#include <extant.h>
#include <realtime.h>

inherit "/std/effect";

nosave private status recalculating;
private mapping challenge;
private status overcome;

status ganesha_challenge_initialize(object creator);
varargs void ganesha_challenge_detach(object who);
void ganesha_challenge_fail();

void configure() {
    ::configure();
    alter_identity(Identity_Special_Names, ({ "GANESHA_CHALLENGE" }), Identity_Alter_Add);
    set_short(0);
/*
    set_shape(Common_Shape_Tiny_Lump);
    add_proportion(([
        Element_Type        : Material_Kedusha,
        Element_Proportion  : 1.00,
    ]));
    add_proportion(([
        Element_Type        : Material_Lexae,
        Element_Proportion  : 1.00,
    ]));
    add_proportion(([
        Element_Type        : Material_Shekhinah,
        Element_Proportion  : 0.01,
    ]));
    set_suppress_condition_display(True);
    set_suppress_size_display(True);
    set_suppress_weight_display(True);
*/
    add_property(Prop_Manifestation);
    set_indestructible(True);
}

status is_ganesha_challenge() {
    return True;
}

object ganesha_challenge_query_owner() {
    return environment() && environment()->is_character() && environment();
}

mixed ganesha_challenge_query_creator() {
    string tag = challenge["creator"];
    return (tag && Extant_Find(tag)) || tag;
}

void ganesha_challenge_set_creator(mixed who) {
    if(Extant_Format_Match(who) && Extant_Find(who))
        challenge["creator"] = who;
    else if(objectp(who))
        challenge["creator"] = who->require_extant();
    else
        error("Was expecting extant or object, got '" + who + "'");
}

status query_auto_keep(object who) {
    return True;
}

status get() {
    return True;
}

status drop() {
    return True;
}

void ganesha_challenge_set_obstacle(string obs) {
    unless(Is_Travelers_Obstacle(obs))
        error("Was expecting obstacle, got '" + obs + "'; requirement was '" + challenge["requirement"] + "'");
    challenge["obstacle"] = obs;
}

string ganesha_challenge_query_obstacle() {
    return challenge["obstacle"];
}

object ganesha_challenge_obstacle() {
    return challenge["obstacle"] && Travelers_Obstacle(challenge["obstacle"]);
}

void ganesha_challenge_set_requirement(string requirement) {
    unless(Is_Travelers_Requirement(requirement))
        error("Was expecting requirement, got '" + requirement + "'; obstacle was '" + challenge["obstacle"] + "'");
    challenge["requirement"] = requirement;
}

string ganesha_challenge_query_requirement() {
    return challenge["requirement"];
}

object ganesha_challenge_requirement() {
    return challenge["requirement"] && Travelers_Requirement(challenge["requirement"]);
}

void ganesha_challenge_set_taboo(string tab) {
    unless(Is_Travelers_Taboo(tab))
        error("Was expecting taboo, got '" + tab + "'");
    challenge["taboo"] = tab;
}

string ganesha_challenge_query_taboo() {
    return challenge["taboo"];
}

object ganesha_challenge_taboo() {
    return challenge["taboo"] && Travelers_Taboo(challenge["taboo"]);
}

void ganesha_challenge_set_forfeit(string frft) {
    unless(Is_Travelers_Forfeit(frft))
        error("Was expecting forfeit, got '" + frft + "'");
    challenge["forfeit"] = frft;
}

string ganesha_challenge_query_forfeit() {
    return challenge["forfeit"];
}

object ganesha_challenge_forfeit() {
    return challenge["forfeit"] && Travelers_Forfeit(challenge["forfeit"]);
}

int ganesha_challenge_query_value() {
    int out = ganesha_challenge_requirement()->query_requirement_value(this_object());
    out += ganesha_challenge_obstacle() && ganesha_challenge_obstacle()->query_obstacle_value(this_object());
    out += ganesha_challenge_taboo() && ganesha_challenge_taboo()->query_taboo_value();
    out += ganesha_challenge_forfeit() && ganesha_challenge_forfeit()->query_forfeit_value();
    return out;
}

void ganesha_challenge_recalculate_obstacle() {
    object obstacle = ganesha_challenge_obstacle();
    object who = ganesha_challenge_query_owner();
    if(!obstacle || !who)
        return;
    recalculating = True;
    obstacle->recalculate_obstacle_scaled_attribute_modifiers(who);
    obstacle->recalculate_obstacle_scaled_skill_modifiers(who);
    recalculating = False;
}

void ganesha_challenge_overcome() {
    ganesha_challenge_obstacle() && ganesha_challenge_obstacle()->obstacle_overcome(this_object());
    ganesha_challenge_requirement()->requirement_overcome(this_object());
    ganesha_challenge_taboo() && ganesha_challenge_taboo()->taboo_overcome(this_object());
    ganesha_challenge_forfeit() && ganesha_challenge_forfeit()->forfeit_overcome(this_object());
    ganesha_challenge_detach();
    string array rewards = Travelers_Daemon("dharma")->challenge_overcome(this_object());
    object who = ganesha_challenge_query_owner();
    if(who) {
        who->set_info("Ganesha_Challenge_Overcame", time());
        if(!who->query_quest("ganesha's_challenge"))
            who->set_quest("ganesha's_challenge");
    }
    string log_string = ctime() + ": challenge overcome by: " + printable(who) + "; creator: " + printable(ganesha_challenge_query_creator());
    if(challenge["obstacle"])
        log_string += "; obstacle: " + challenge["obstacle"];
    log_string += "; requirement: " + challenge["requirement"];
    if(challenge["taboo"])
        log_string += "; taboo " + challenge["taboo"];
    if(challenge["forfeit"])
        log_string += "; forfeit " + challenge["forfeit"];
    log_string += "; bestowals: " + implode(rewards, ", ");
    log_file(Travelers_Data("challenges.log"), log_string);
    remove();
}

void ganesha_challenge_yield() {
    object who = ganesha_challenge_query_owner();
    if(!who) {
        remove();
        return;
    }
    who->set_info("Ganesha_Challenge_Allowed_Time", time() + ganesha_challenge_query_value() * Time_Hour * 5);
    Ganesha_Add_Karma_Phala(who, -ganesha_challenge_query_value() * 2);
    if(challenge["taboo"] == "yielding") {
        ganesha_challenge_fail();
        return;
    }
    ganesha_challenge_obstacle() && ganesha_challenge_obstacle()->obstacle_yield(this_object());
    ganesha_challenge_requirement()->requirement_yield(this_object());
    ganesha_challenge_taboo() && ganesha_challenge_taboo()->taboo_yield(this_object());
    ganesha_challenge_forfeit() && ganesha_challenge_forfeit()->forfeit_yield(this_object());
    string log_string = ctime() + ": challenge yielded by: " + printable(who) + "; creator: " + printable(ganesha_challenge_query_creator());
    if(challenge["obstacle"])
        log_string += "; obstacle: " + challenge["obstacle"];
    log_string += "; requirement: " + challenge["requirement"];
    if(challenge["taboo"])
        log_string += "; taboo " + challenge["taboo"];
    if(challenge["forfeit"])
        log_string += "; forfeit " + challenge["forfeit"];
    log_file(Travelers_Data("challenges.log"), log_string);
    remove();
}

void ganesha_challenge_fail() {
    ganesha_challenge_obstacle() && ganesha_challenge_obstacle()->obstacle_fail(this_object());
    ganesha_challenge_requirement()->requirement_fail(this_object());
    ganesha_challenge_taboo()->taboo_fail(this_object());
    ganesha_challenge_forfeit() && ganesha_challenge_forfeit()->forfeit_fail(this_object());
    object who = ganesha_challenge_query_owner();
    if(who) {
        who->set_info("Ganesha_Challenge_Failed", time());
        string log_string = ctime() + ": challenge failed by: " + printable(who) + "; creator: " + printable(ganesha_challenge_query_creator());
        if(challenge["obstacle"])
            log_string += "; obstacle: " + challenge["obstacle"];
        log_string += "; requirement: " + challenge["requirement"];
        log_string += "; taboo " + challenge["taboo"];
        if(challenge["forfeit"])
            log_string += "; forfeit " + challenge["forfeit"];
        log_file(Travelers_Data("challenges.log"), log_string);
    }
    remove();
}

void ganesha_challenge_at_revive(mapping args) {
    ganesha_challenge_recalculate_obstacle();
}

void heart_beat() {
    set_heart_beat(False);
    ganesha_challenge_recalculate_obstacle();
}

void ganesha_challenge_at_incarnate(object who) {
    if(!recalculating)
        set_heart_beat(True);
    who->remove_hook(At_Incarnate, #'ganesha_challenge_at_incarnate);
}

void ganesha_challenge_at_skill_update(record event) {
    if(!recalculating)
        set_heart_beat(True);
}

void ganesha_challenge_at_attribute_update(mapping args) {
    if(!recalculating)
        set_heart_beat(True);
}

void ganesha_challenge_attach(object who) {
    if(who && environment() != who)
        move(who, Move_Flags_Force);
    who ||= ganesha_challenge_query_owner();
    ganesha_challenge_obstacle() && ganesha_challenge_obstacle()->obstacle_attach(this_object());
    ganesha_challenge_requirement()->requirement_attach(this_object());
    ganesha_challenge_taboo() && ganesha_challenge_taboo()->taboo_attach(this_object());
    who->require_hook(At_Revive, #'ganesha_challenge_at_revive);
    who->require_hook(At_Skill_Update, #'ganesha_challenge_at_skill_update);
    who->require_hook(At_Attribute_Update, #'ganesha_challenge_at_attribute_update);
}

varargs void ganesha_challenge_detach(object who) {
    who ||= ganesha_challenge_query_owner();
    ganesha_challenge_obstacle() && ganesha_challenge_obstacle()->obstacle_detach(this_object());
    ganesha_challenge_requirement()->requirement_detach(this_object());
    ganesha_challenge_taboo() && ganesha_challenge_taboo()->taboo_detach(this_object());
    who->remove_hook(At_Revive, #'ganesha_challenge_at_revive);
    who->remove_hook(At_Skill_Update, #'ganesha_challenge_at_skill_update);
    who->remove_hook(At_Attribute_Update, #'ganesha_challenge_at_attribute_update);
}

void preinit() {
    ::preinit();
    challenge ||= ([]);
}

varargs void remove(int flags) {
    object who = ganesha_challenge_query_owner();
    if(who)
        ganesha_challenge_detach(who);
    ::remove(flags);
}

varargs status ganesha_challenge_initialize(object creator, object target, mixed obs, mixed req, mixed tab, mixed frft) {
    if(!req)
        error("cannot initialize a challenge without a requirement");
    if(!obs && !tab)
        error("cannot initialize challenge with neither obstacle nor taboo");
    if(!tab && frft)
        error("cannot initialize a challenge with a forfeit but not a taboo");
    ganesha_challenge_set_creator(creator);
    target ||= creator;
    if(Travelers_Find_Challenge(target))
        return False;
    if(move(target, Move_Flags_Force) != Move_Succeed)
        return False;
    if(obs == True) {
        string override = target->query_info("Ganesha_Challenge_Next_Obstacle");
        if(override) {
            obs = override;
            target->remove_info("Ganesha_Challenge_Next_Obstacle");
        } else {
            obs = Travelers_Daemon("dharma")->generate_random_obstacle(this_object());
        }
    }
    if(obs)
        ganesha_challenge_set_obstacle(obs);
    if(req == True) {
        string override = target->query_info("Ganesha_Challenge_Next_Requirement");
        if(override) {
            req = override;
            target->remove_info("Ganesha_Challenge_Next_Requirement");
        } else {
            req = Travelers_Daemon("dharma")->generate_random_requirement(this_object());
        }
    }
    ganesha_challenge_set_requirement(req);
    if(obs)
        ganesha_challenge_obstacle()->obstacle_initialize(this_object());
    ganesha_challenge_requirement()->requirement_initialize(this_object());
    if(tab == True) {
        string override = target->query_info("Ganesha_Challenge_Next_Taboo");
        if(override) {
            tab = override;
            target->remove_info("Ganesha_Challenge_Next_Taboo");
        } else {
            tab = Travelers_Daemon("dharma")->generate_random_taboo(this_object());
        }
    }
    if(tab)
        ganesha_challenge_set_taboo(tab);
    if(frft == True) {
        string override = target->query_info("Ganesha_Challenge_Next_Forfeit");
        if(override) {
            frft = override;
            target->remove_info("Ganesha_Challenge_Next_Forfeit");
        } else {
            frft = Travelers_Daemon("dharma")->generate_random_forfeit(this_object());
        }
    }
    if(frft)
        ganesha_challenge_set_forfeit(frft);
    if(tab)
        ganesha_challenge_taboo()->taboo_initialize(this_object());
    if(frft)
        ganesha_challenge_forfeit()->forfeit_initialize(this_object());
    ganesha_challenge_attach(target);
    string log_string = ctime() + ": challenge started by: " + printable(target) + "; creator: " + printable(creator);
    if(obs)
        log_string += "; obstacle: " + obs;
    log_string += "; requirement: " + req;
    if(tab)
        log_string += "; taboo: " + tab;
    if(frft)
        log_string += "; forfeit: " + frft;
    log_file(Travelers_Data("challenges.log"), log_string);
    return True;
}

void process_post_restore(object who, record save, int restore_flags) {
    ::process_post_restore(who, save, restore_flags);
    ganesha_challenge_attach(who);
    who->require_hook(At_Incarnate, #'ganesha_challenge_at_incarnate);
}

void reset() {
    ::reset();
    if(ganesha_challenge_query_owner())
        ganesha_challenge_recalculate_obstacle();
}

string stat() {
    string out = "\n";
    out += "{{orange}{{bright yellow}Ganesha Challenge} -----------------------------------------------------------}\n";
    out += "{{red}Creator                 :} {{orange}" + printable(ganesha_challenge_query_creator()) + "}\n";
    if(challenge["obstacle"])
        out += "{{red}Obstacle                :} {{orange}" + challenge["obstacle"] + "}\n";
    out += "{{red}Requirement             :} {{orange}" + challenge["requirement"] + "}\n";
    if(challenge["taboo"])
        out += "{{red}Taboo                   :} {{orange}" + challenge["taboo"] + "}\n";
    if(challenge["forfeit"])
        out += "{{red}Forfeit                 :} {{orange}" + challenge["forfeit"] + "}\n";
    mapping additional_info = copy(query_info());
    if(additional_info) {
        if(additional_info["Atman_Keep_Tracking"])
            m_delete(additional_info, "Atman_Keep_Tracking");
        if(additional_info["Last_Requirement_Notification"])
            m_delete(additional_info, "Last_Requirement_Notification");
        string array info_list = ({});
        foreach(mixed key, mixed value : additional_info) {
            string item = printable(key);
            item += ", ";
            item += printable(value);
            info_list += ({ item });
        }
        out += "{{red}Additional_Info         :} {{orange}" + implode(info_list, "; ") + "}\n";
    }
    out += "{{orange}-----------------------------------------------------------------------------}\n";
    return out;
}

string query_score_append() {
    string out = "";
    out += "Ganesha has given you a challenge, " + ganesha_challenge_requirement()->query_requirement_initialize_description();
    if(challenge["obstacle"]) {
        out += ", to be completed despite the obstacle of ";
        object obstacle = ganesha_challenge_obstacle();
        if(obstacle->query_obstacle_prepend_indefinite_article()) {
            out += a(obstacle->query_obstacle_name());
        } else {
            out += obstacle->query_obstacle_name();
        }
    }
    out += ".";
    if(challenge["taboo"]) {
        out += "  This challenge carries with it a taboo against " + ganesha_challenge_taboo()->query_taboo_name();
        if(challenge["forfeit"])
            out += " on penalty of " + ganesha_challenge_forfeit()->query_forfeit_name();
        out += ".";
    }
    if(query_info("Last_Requirement_Notification"))
        out += "  You have completed " + query_info("Last_Requirement_Notification") + "% of this challenge.";
    return out;
}
