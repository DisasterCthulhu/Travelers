#include <Travelers.h>
#include <daemon.h>

inherit Travelers_Definition("Invocation");

void configure() {
	::configure();
	set_invocation_name("shree ganesh");
	set_invocation_summary("yield to challenge");
	set_invocation_key("o lord Ganesha, radiant as millions of suns, remove your obstacles from");
	set_invocation_argument("%s");
	set_invocation_argument_description("<my/target's> path");
	set_invocation_argument_required(True);
	set_invocation_help(
		"The Shree Ganesh mantra allows the Traveler to entreat Ganesha to remove one of his obstacles from those who "
		"have been unable to overcome it themselves.  For one to perform this on a person besides oneself, that person must "
		"explicitly trust the Traveler."
		"\n\f"
		"An obstacle removed in this way is tantamount to yielding to a challenge.  "
		"Ganesha is kind enough to remove an obstacle from any who need it, but firm enough "
		"to show no charity beyond mercy for the yielder.  "
		"The challenger will receive none of the benefits "
		"for overcoming it, other than the removable of the obstacle, and the challenger will not be able to receive a new "
		"challenge from Ganesha for an extended period of time -- usually around a day, but sometimes considerably shorter or longer "
		"depending on the difficulty of the challenge.  The challenger will also suffer a decrease in karma phala proportional "
		"to the difficulty of the challenge, that is, equal to twice the amount they would have gained had they completed it."
	);
	set_invocation_cost(({
		({ Energy_Spiritual, 5 }),
	}));
	set_invocation_delay(Time_Second * 4);
	set_invocation_ability_determinants(([
		Condition_Skill_Composite           : True,
		Skill_Invocation                    : 0.80,
		Skill_Abjuration                    : 0.20,
		([
			Condition_Type_Code             : Condition_Type_Attribute,
			Condition_Info                  : Attr_Cha,
			Condition_Value                 : 1.00,
		])                                  : True,
	]));
	set_invocation_begin_can_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		string argument = Process_Query_Info($1, "argument");
		string what;
		unless(sscanf(argument, "%s path", what) == 1)
			return Error(({
				who, ({ "sense", who }), "something amiss in", ({ 'r', who, "invocation" }),
			}));
		object target;
		if(what == "my")
			target = who;
		else {
			sscanf(what, "%s's", what);
			target = who->locate_name(what);
		}
		unless(target)
			return Error(({
				"there is no '" + what + "' here" 
			}));
		unless(environment(target) == environment(who))
			return Error(({
				target, ({ "are", target }), "not in", ({ 's', who, "presence" })
			}));
		unless(target->query_trusts(who))
			return Error(({
				target, ({ "must", target }), "trust", ({ 'o', 0 }), "for this invocation to affect", ({ 'o', target }),
			}));
		Process_Set(dxr, Process_Target, target);
		return True;
	:));
	set_invocation_continue_can_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		object target = Process_Query(dxr, Process_Target);
		if(!target)
			return Error(({
				({ 's', who, "target" }), "is no longer present",
			}));
		unless(environment(target) == environment(who))
			return Error(({
				target, ({ "are", target }), "no longer in", ({ 's', who, "presence" })
			}));
		unless(target->query_trusts(who))
			return Error(({
				target, "no longer", ({ "trust", target }), "trust", ({ 'o', 0 }),
			}));
		return True;
	:));
	set_invocation_execute_do_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		object target = Process_Query(dxr, Process_Target);
		object challenge = Travelers_Find_Challenge(target);
		if(challenge) {
			target->message(([
				Message_Content     : ({
					'a', self_color("rainbow", 0, "spiral"), "twists out of the center of", ({ 's', target, "body" }),
				}),
				Message_Senses      : Message_Sense_Astral,
			]));
			challenge->ganesha_challenge_yield();
			who->display(([
				Message_Content : ({
					0, ({ "sense", 0 }), "that Ganesha has removed his obstacle from", ({ 's', target, "path" }), ";",
					({ 's', target, "challenge" }), "is perforce forgone"
				}),
				Message_Senses  : Message_Sense_Cognitive,
			]));
		} else {
			who->display(([
				Message_Content : ({
					0, ({ "sense", 0 }), "that", target, ({ "were", target }), "not beset by any obstacles from Ganesha"
				}),
				Message_Senses  : Message_Sense_Cognitive,
			]));
		}
		return;
	:));
}
