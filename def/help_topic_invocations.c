#include <daemon.h>
#include <Travelers.h>

inherit "/std/def/help_topic";

string help_invocations_output(object who) {
    string out = "";
    out += "{{green}Invocations}";
    out += "\n\f";
    out += "You are able to invoke Ganesha by reciting certain mantras to him in Archaen.  More invocations will be revealed to you the more "
        "obstacles you overcome.  Each invocation has its own help file.  You must be in a fairly calm state to invoke Ganesha; you cannot invoke "
        "him while moving around or fighting.";
    out += "\n\f";
    out += "The invocations known to you are as follows:{{orange}\n\n";
    string array invocations = filter(Travelers_Daemon("dharma")->query_valid_invocations(), (: $1->query_invocation_known($2) :), who)->query_invocation_name();
    invocations = sort_array(invocations, #'>);
    mapping invocations_by_bestowal = ([]);
    foreach(string invocation : invocations) {
        string bestowal = Travelers_Invocation(invocation)->query_invocation_bestowal() || "default";
        invocations_by_bestowal[bestowal] ||= ({});
        invocations_by_bestowal[bestowal] += ({ invocation });
    }
    foreach(string invocation : invocations_by_bestowal["default"]) {
        out += "      ";
        out += capitalize_words(invocation, True);
        out += " {{golden}(";
        out += Travelers_Invocation(invocation)->query_invocation_summary();
        out += ")}\n";
    }
    out += "\n";
    m_delete(invocations_by_bestowal, "default");
    foreach(string bestowal, string array bestowal_invocations : invocations_by_bestowal) {
        foreach(string invocation : bestowal_invocations) {
            out += "      ";
            out += capitalize_words(invocation, True);
            out += " {{golden}(";
            out += Travelers_Invocation(invocation)->query_invocation_summary();
            out += ")}\n";
        }
        out += "\n";
    }
    out += "}";
    return out;
}

void configure() {
    ::configure();
    set_help_topic_name("invocations");
    set_help_topic_output(#'help_invocations_output);
}
