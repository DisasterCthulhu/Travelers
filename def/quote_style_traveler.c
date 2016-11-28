#include <Travelers.h>
#include <quotes.h>

inherit "/std/def/quote_style";

// colors based on chakra colors.  order is relevant!

#define Travelers_Quote_Style_Colors ({\
    ({ "red", "violet" }),\
    ({ "violet", "indigo" }),\
    ({ "indigo", "blue" }),\
    ({ "blue", "green" }),\
    ({ "green", "yellow" }),\
    ({ "yellow", "orange" }),\
    ({ "orange", "red" }),\
})

void travelers_quote_style_maintenance() {
    string array colors = random_element(Travelers_Quote_Style_Colors);
    set_quote_style_quotes(({
        "{{" + colors[0] + "}o} {{white}",
        "}{{" + colors[1] + "} o}",
    }));
}

void heart_beat() {
    travelers_quote_style_maintenance();
}

void configure() {
    ::configure();
    set_quote_style_name("Traveler");
    travelers_quote_style_maintenance();
    set_quote_style_priority(Quote_Style_Priority_Guild);
    set_quote_style_mimicry_access(24);
    set_quote_style_vocalization_access(81);
}

void create() {
    ::create();
    set_heart_beat(True);
}
