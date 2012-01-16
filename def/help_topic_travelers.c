#include <daemon.h>
#include <Aligned.h>

inherit "/std/def/help_topic";

string array help_travelers_bestowals(object who) {
	string array out = ({});
	object link = who->query_affiliation_link(project_control());
	unless(link)
		return out;
	out += link->travelers_query_bestowals();
	return out;
}

string help_travelers_output(object who) {
	string array bestowals = help_travelers_bestowals(who);
	string out = "";
	out += "{{yellow}(}{{orange}o} {{white}the Travelers} {{blue}o}{{green})}";
	out += "{{indent 4}";
	out += "\n\f";
	out += "You are a Traveler, a wandering priest of Ganesha, lord of gateways, places and remover of obstacles, "
		"guide to enlightenment, protector to freedom.";
	out += "\n\f";
	out += "A Traveler's primary task is overcoming obstacles: overcoming them himself, and helping others overcome them.  "
		"As he does so, he will find himself rewarded by Ganesha in many ways.  These rewards are called bestowals, and "
		"they are primarily of two types: recurring boons, most of which can be acquired by anyone, and more permanent "
		"boons that can be acquired only by Travelers.  ";
	if(!sizeof(bestowals)) {
		out += "You have not yet acquired any permanent bestowals.";
	} else {
		out += "You have acquired " + things("permanent bestowal", sizeof(bestowals)) + ".";
	}
	out += "\n\f";
	out += "Travelers spend their lives spreading freedom and enlightenment, journeying across the world and doing good deeds, "
		"in a never-ending quest to better their understanding of kharma and dharma.  Doing so brings them closer to Ganesha, "
		"makes their invocations and powers more effective, enables them to discover hidden energy stores in themselves in the "
		"form of chakras, allowed them to live longer -- and has other, still stranger effects.";
	out += "\n\f";
	out += "As a Traveler you are able to invoke Ganesha to various effects.  See 'help invocations' for more details.";
	out += "}";
	return out;
}

void configure() {
	::configure();
	set_help_topic_name("travelers");
	set_help_topic_output(#'help_travelers_output);
}

