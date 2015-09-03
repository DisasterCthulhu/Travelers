#include <Travelers.h>
#include <daemon.h>
#include <Empathic_Bonds.h>
#include <bond.h>

inherit Travelers_Definition("Invocation");

nosave private object conflicted;

void kabatha_fail_equip_item(mapping args) {
	conflicted = args["conflict"];
}

string array query_resistance_types_available(object who) {
        string array out = ({});
        object link = who->query_affiliation_link(project_control());
        unless(link)
                return out;
        string array bestowals = link->travelers_query_bestowals();
        foreach(string bestowal : bestowals) {
                string array new_types = Travelers_Bestowal(bestowal)->query_bestowal_kabatha_resistances();
                if(new_types)
                        out += new_types;
        }
        return sort_array(unique(out), #'>);
}

void configure() {
	::configure();
	set_invocation_name("pancharatnam");
	set_invocation_summary("request kabatha");
	set_invocation_key(
		"o lord Ganesha, who holds the divine joy in his hand as modaka, who holds the digit of Ganapati, "
		"send me one of the five gems that encircle Yama, that the vahana you have granted me may be protected "
		"from demons and bear me forth upon the paths of enlightenment"
	);
	set_invocation_help((:
		"This invocation requests of Ganesha that he send unto your mount a kabatha, "
		"a form of divine armour to protect one's vahana."
		"\n\f"
		"A kabatha conjured in this manner will remain with one's vahana permanently.  "
		"If it is ever lost or destroyed for some reason, performing this mantra again "
		"will conjure another.  Reciting this invocation can also banish a kabatha, "
		"if one is present upon one's mount."
		"\n\fThe resistances your vahana's kabatha will be enhanced with are : " + list_array(query_resistance_types_available($1))
	:));
	set_invocation_cost(({
		({ Energy_Spiritual, 500 })
	}));
	set_invocation_bestowal("kabatha");
	set_invocation_delay(Time_Second * 15);
	set_invocation_ability_determinants(([
		Condition_Skill_Composite            : True,
		Skill_Conjuration                    : 0.50,
		Skill_Invocation                     : 0.50,
		([
			Condition_Type_Code              : Condition_Type_Attribute,
			Condition_Info                   : Attr_Cha,
			Condition_Value                  : 0.75,
		])                                   : True,
		([
			Condition_Type_Code              : Condition_Type_Attribute,
			Condition_Info                   : Attr_Int,
			Condition_Value                  : 0.25,
		])                                   : True,
	]));
	set_invocation_begin_can_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		// make sure the mount is present and isn't already wearing a kabatha
		descriptor bond = Empathic_Bonds_Find_Affiliation_Bond(who, project_control());
		unless(bond) {
			return Error(({
				who, ({ "must", who }), "possess a vahana granted by Ganesha to summon a kabatha"
			}));
		}
		object familiar = Bond_Query(bond, Bond_Familiar);
		unless(familiar && (who->query_character_object_nearby(familiar) || who->query_child_object(familiar))) {
			return Error(({
				({ 'r', who, "vahana" }), "must be present for Ganesha to bestow a kabatha upon them",
			}));
		}
		return True;
	:));
	set_invocation_execute_do_call((:
		descriptor dxr = $1;
		object who = Process_Query(dxr, Process_Actor);
		// initial msg
		who->message(([
			Message_Content                  : ({
				"a sonorous hum fills", ({ 't', Description(Description_Type_Ambient_Medium_Colored_Name) }),
				", as though a great choir has begun intoning a prayer"
			}),
			Message_Senses                   : Message_Sense_Auditory,
		]));
		object familiar = 0;
		descriptor bond = Empathic_Bonds_Find_Affiliation_Bond(who, project_control());
		if(bond) {
			familiar = Bond_Query(bond, Bond_Familiar);
		}
		unless(familiar && (who->query_character_object_nearby(familiar) || who->query_child_object(familiar))) {
			who->message(([
				Message_Content              : ({
					"however, the sonorous hum fades away after a moment, seeming incomplete",
				}),
				Message_Senses               : Message_Sense_Auditory,
			]));
			return;
		}
		object existing_kabatha = familiar->query_child_object("is_kabatha");
		if(existing_kabatha) {
			// remove existing
			familiar->message(([
				Message_Content              : ({
					({ 's', familiar, existing_kabatha }),
					([
						Message_Content      : ({
							({ "evaporate", existing_kabatha }),
						}),
						Message_Color        : "shimmering gold",
					]), "and",
					([
						Message_Content      : ({
							({ "dissipate", existing_kabatha }),
						}),
						Message_Color        : "opalescent",
					]),
					"into", ({ 't', Description(Description_Type_Ambient_Medium_Colored_Name) }),
				}),
				Message_Senses               : Message_Sense_Visual | Message_Sense_Astral,
			]));
			existing_kabatha->remove();
			who->message(([
				Message_Content              : ({
					"the sonorous hum fades away",
				}),
				Message_Senses               : Message_Sense_Auditory,
			]));
			return;
		}
		// cloud starts forming
		familiar->message(([
			Message_Content                  : ({
				"a {{shimmering gold}majestic} {{gold}golden cloud} begins to take shape in",
				({ 't', Description(Description_Type_Ambient_Medium_Colored_Name) }), "above", familiar,
			}),
			Message_Senses                   : Message_Sense_Visual | Message_Sense_Astral,
		]));
		// check_utilize and move
		object kabatha = new(Travelers_Armour("kabatha"));
		kabatha->move(familiar, Move_Flags_Force | Move_Flag_Failure_Destructs);
		unless(kabatha) {
			who->message(([
				Message_Content              : ({
					'v', ({ "hear", 'v' }), "the sonorous hum fade almost instantly as the",
					"{{gold}gold cloud} evaporates into the surrounding",
					Description(Description_Type_Ambient_Medium_Colored_Name),
					([
						Message_Content      : ({
							"and", 0, ({ "sense", 0 }), "Ganesha's power unable to manifest itself",
							"upon", ({ 'r', 0, familiar }),
						}),
						Message_Senses       : Message_Sense_Cognitive,
						Message_Flags        : Message_Flag_Source_Only,
					]),
				}),
				Message_Senses               : Message_Sense_Visual | Message_Sense_Astral,
			]));
			return;
		}
		conflicted = 0;
		familiar->require_hook(Fail_Equip_Item, #'kabatha_fail_equip_item);
		familiar->utilize_item(kabatha, Move_Flag_Usage_Silent);
		familiar->remove_hook(Fail_Equip_Item, #'kabatha_fail_equip_item);
		if(conflicted) {
			mixed res = familiar->check_deutilize_item(conflicted, Move_Flag_Usage_Silent);
			unless(Result_Success(res)) {
				who->message(([
					Message_Content          : ({
						'v', ({ "hear", 'v' }), "the sonorous hum fade almost instantly as the",
						"{{gold}gold cloud} evaporates into the surrounding",
						Description(Description_Type_Ambient_Medium_Colored_Name),
						([
							Message_Content  : ({
								"and", 0, ({ "sense", 0 }), "Ganesha's power unable to manifest",
								"with", ({ 'r', 0, familiar }), "already protected by",
								({ 'r', familiar, conflicted }),
							}),
							Message_Senses   : Message_Sense_Cognitive,
							Message_Flags    : Message_Flag_Source_Only,
						]),
					}),
					Message_Senses           : Message_Sense_Visual | Message_Sense_Astral,
				]));
				conflicted = 0;
				safe_destruct(kabatha);
				return;
			}
			familiar->message(([
				Message_Content              : ({
					"the {{gold}gold cloud} drifts downward over", ({ 's', 0, conflicted }), "and drifts upward again",
					", leaving", ({ 's', familiar, conflicted }), "removed from", ({ 'r', familiar, "body" })
				}),
				Message_Senses               : Message_Sense_Visual | Message_Sense_Astral,
			]));
			familiar->deutilize_item(conflicted, Move_Flag_Usage_Silent);
			conflicted = 0;
		}
		who->message(([
				Message_Content              : ({
					"the sonorous hum reaches a mighty crescendo",
				}),
				Message_Senses               : Message_Sense_Auditory,
			]));
		who->message(([
			Message_Content                  : ({
				"the {{gold}gold cloud} drifts over", ({ 's', 0, familiar }),
				"and quickly materializes into", ({ 'a', kabatha }), "upon", ({ 'o', familiar }),
				([
					Message_Content          : ({
						"and", 0, ({ "sense", 0 }), ({ 'o', kabatha }), "embracing and infusing",
						({ 'r', familiar,
							Description(([
								Description_Type     : Description_Type_Element_Name,
								Description_Index    : Material_Scales,
								Description_Object   : familiar,
								Description_Flags    : Description_Flag_Genericize,
							])),
						}), "with the protections of Ganesha",
					}),
					Message_Senses           : Message_Sense_Cognitive,
					Message_Flags            : Message_Flag_Source_Only,
				]),
			}),
			Message_Senses                   : Message_Sense_Visual | Message_Sense_Tactile_For_Participants,
		]));
		kabatha->armour()->perform_adapt(familiar);
		kabatha->kabatha_configure(who);
		familiar->utilize_item(kabatha, Move_Flag_Usage_Silent);
	:));
}