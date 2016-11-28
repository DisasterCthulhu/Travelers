inherit "/std/def/channel";

void configure() {
    ::configure();
    set_channel_name("traveler");
    set_channel_borders( ({ "{{yellow}(}{{orange}o }{{white}Traveler ", "}{{blue} o}{{green})}" }) );
    set_channel_verbs(({ "traveler", "guild" }));
    set_channel_cost(1);
    set_channel_list_header("{{yellow}------------------- (} {{white}The Travelers} {{orange}) ------------------}\n");
    set_channel_list_footer("{{blue}------------------------- (}{{white} o }{{green}) -------------------------}\n");
}
