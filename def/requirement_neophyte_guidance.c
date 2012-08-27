#include <daemon.h>
#include <experience.h>
#include <Travelers.h>

inherit Travelers_Definition("Requirement");

void travelers_requirement_neophyte_guidance_at_warden_client_experience_gained(mapping args) {
	object who = args["who"];
	int exp = args["experience"];
	object challenge = Travelers_Find_Challenge(who);
	int gained = challenge->query_info("Client_Experience_Gained");
	gained += exp;
	if(gained >= challenge->query_info("Client_Experience_Required")) {
		challenge->ganesha_challenge_overcome();
	} else {
		challenge->set_info("Client_Experience_Gained", gained);
		requirement_check_notification(challenge, gained, 0, "Client_Experience_Required");
	}
}

void configure() {
	::configure();
	set_requirement_name("neophyte guidance");
	set_requirement_rarity(Rarity_Common);
	set_requirement_value(Travelers_Requirement_Value_Very_High);
	set_requirement_eligibility_condition(([
	    Condition_Type_Code                     : Condition_Type_And,
	    Condition_Info                          : ({
	        ([
		        Condition_Type_Code             : Condition_Type_Affiliation,
		        Condition_Info                  : "Ashe's Wardens",
		    ]),
	        ([
		        Condition_Type_Code             : Condition_Type_Metric,
		        Condition_Info                  : "number of unguided neophytes",
		        Condition_Value                 : 1,
		    ]),
		}),
	]));
	set_requirement_initialize_description("guiding a neophyte in your capacity as a Warden");
	set_requirement_overcome_description("provided enough guidance");
	set_requirement_initialize_process((:
		int client_exp_needed = 100;
		$1->set_info("Client_Experience_Required", client_exp_needed);
		$1->set_info("Client_Experience_Gained", 0);
		return;
	:));
	add_requirement_hook("at_warden_client_experience_gained", #'travelers_requirement_neophyte_guidance_at_warden_client_experience_gained);
}
