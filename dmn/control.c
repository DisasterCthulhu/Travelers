#include <Travelers.h>
#include <daemon.h>
#include <alignment.h>
#include <Empathic_Bonds.h>
#include <bond.h>

inherit "/std/daemon";
inherit "/mod/daemon/affiliation";
inherit "/mod/daemon/control";

varargs mixed travelers_check_alignment(object who, object ctl, int flags) {
	if(who->query_alignment(Alignment_Order_Chaos) < Travelers_Minimum_Orderliness)
		return ({
			({ 's', 0, "devotion" }), "to the well-being of all is insufficient to advance in level",
		});
	if(who->query_alignment(Alignment_Good_Evil) < Travelers_Minimum_Goodness)
		return ({
			({ 's', 0, "devotion" }), "to", ({ 'r', 0, "place in the dharmic order" }), "is insufficient to advance in level",
		});
	return 0;
}

string travelers_join(object who, object ctl, object how) {
	string array disallowed_worships = who->query_worships() - ({
		"ganesha",
		"shiva",
		"vishnu",
		"brahma",
		"parvati",
		"skanda",
		"surya",
	});
	if(sizeof(disallowed_worships)) {
		disallowed_worships = map(disallowed_worships, (: capitalize_words($1) :));
		return "You cannot be a devotee of Ganesha while you worship " + list_array(disallowed_worships, "or") + ".";
	}
	int challenge_time = who->query_info("Ganesha_Challenge_Overcame");
	if(!challenge_time)
		return "You wish to be a devotee of the remover and placer of obstacles?  Prove it.  Overcome one of Ganesha's obstacles.  You "
			"may request one of me, if you like.";
	if(time() - challenge_time > Time_Week)
		return "You must prove your worth by overcoming one of Ganesha's obstacles.  You have not done so in a very long time%g.";
	if(travelers_check_alignment(who) != 0)
		return "Your ethical outlook needs some work before you could possibly understand what Ganesha wants of you.";
	return 0;
}

void travelers_new_member(object who, object ctl, object how) {
	object rep = how->affiliation_representative();
	if(who->query_info("Travelers_Left")) {
		rep->init_command("say Excellent!  Ganesha welcomes you back.", Command_Flags_System);
		foreach(int attr : Basic_Attributes)
			who->add_permanent_attribute_adjustment(attr, 10);
		who->display(([
			Message_Content     : ({
				0, ({ "feel", 0 }), ({ 'r', 0, "personal essence" }), "return to its full potential"
		}),
			Message_Senses      : Message_Sense_Kinesthetic | Message_Sense_Cognitive,
		]));
		who->remove_info("Travelers_Left");
		who->remove_info("Travelers_Left_Time");
		rep->init_command("traveler " + who->query_full_name() + " has returned to our fold.", Command_Flags_System);
	} else {
		rep->init_command("say Excellent!  May your journeys be long and enlightening.", Command_Flags_System);
		rep->init_command("traveler " + who->query_full_name() + " has joined our fold.", Command_Flags_System);
	}
	log_file("dev/twilight.travelers", ctime() + ": " + printable(who) + " joined");
}

string travelers_leave(object who, object ctl, object how) {
	return 0;
}

void travelers_left(object who, object ctl, object how) {
	object rep = how->affiliation_representative();
	rep->init_command("say Very well.  Ganesha will be waiting for your return to the paths he affords you.  Until then, he leaves you with one last obstacle.", Command_Flags_System);
	who->set_info("Travelers_Left", "voluntary");
	who->set_info("Travelers_Left_Time", time());
	rep->init_command("traveler " + who->query_full_name() + " has left our fold.", Command_Flags_System);
	foreach(int attr : Basic_Attributes)
		who->add_permanent_attribute_adjustment(attr, -10);
	who->display(([
		Message_Content     : ({
			0, ({ "feel", 0 }), ({ 'r', 0, "personal essence" }), "diminish somewhat"
		}),
		Message_Senses      : Message_Sense_Kinesthetic | Message_Sense_Cognitive,
	]));
	foreach(object obj : deep_inventory(who))
		if(begins_with(obj, Travelers_Weapon("")) || begins_with(obj, Travelers_Misc("")))
			obj->remove();
	descriptor bond = Empathic_Bonds_Find_Affiliation_Bond(who, project_control());
	if(bond) {
	    descriptor familiar = Bond_Query(bond, Bond_Familiar);
	    if(familiar) {
	        who->query_affiliation_link("Empathic Bonds")->empathic_bonds_remove_familiar(familiar);
	        familiar->message(([
	            Message_Senses      : Message_Sense_Visual | Message_Sense_Astral,
	            Message_Content     : ({
	                0, ({ "vanish", 0 }), "in", 'a', self_color("prismatic", "burst of", "sparks"),
	            }),
	        ]));
	        familiar->remove();
	    }
	    Empathic_Bonds_Remove_Bond_Means(who, Bond_Query(bond, Bond_Means));
	}
}

void configure() {
	::configure();
	set_creator("den");
	set_maintainer("twilight");
	set_affiliation_class(Affiliation_Class_Guild);
	set_affiliation_name("the Travelers");
	set_affiliation_aliases(({ "Travelers" }));
	set_affiliation_title("Traveler");
	set_affiliation_link(Travelers_Misc("link"));
	set_affiliation_worships(({
		"ganesha",
	}));
	set_affiliation_advance_func(#'travelers_check_alignment);
	set_affiliation_join_func(#'travelers_join);
	set_affiliation_new_member_func(#'travelers_new_member);
	set_affiliation_leave_func(#'travelers_leave);
	set_affiliation_left_func(#'travelers_left);
	set_affiliation_type(({
		Affiliation_Type_Altruist,
		Affiliation_Type_Devotional,
		Affiliation_Type_Invested,
		Affiliation_Type_Mystic,
		Affiliation_Type_Ordered,
		Affiliation_Type_Priest,
		Affiliation_Type_Wanderer,
		Affiliation_Type_Warrior,
	}));
	set_affiliation_type_exclude(({
		Affiliation_Type_Devotional,
	}));
	add_affiliation_quote_style(Travelers_Definition("quote_style_traveler"));
	set_affiliation_public_information(
		"A group of adventurers calling themselves the Travelers have begun to roam the lands in a quest for enlightenment "
		"through overcoming hardship."
	);
	set_affiliation_keep_cost(Keep_Cost_Affiliation_High);
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			Skill_Invocation,
			Skill_Sephirotic_Affinity,
			Skill_Order_Affinity,
			Skill_Goetic_Investiture,
		}),
		Specialty_Degree      : 15,
		Specialty_Required    : 1,
		Specialty_Recommended : 3,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			Skill_Abjuration,
			Skill_Conjuration,
			Skill_Theurgy,
		}),
		Specialty_Degree      : 10,
		Specialty_Required    : 1,
		Specialty_Recommended : 2,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			// thinking of archaen as somewhat like hindi, or an esoterically powerful version
			// of hindi
			Skill_Archaen,
		}),
		Specialty_Degree      : 10,
		Specialty_Recommended : 1,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			// all extremely common languages, trade languages, and compromise languages, and
			// otherwise diplomatic-seeming languages should be granted at relatively high access
			Skill_Anglic,
			Skill_Angrak,
			Skill_Assarith,
			Skill_Dazask,
			Skill_Esperanto,
			Skill_Lingua_Manu,
			Skill_Nilasnai,
		}),
		Specialty_Degree      : 5,
		Specialty_Recommended : 1,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			// less common but still widespread
			Skill_Aelvalie,
			Skill_Dethek,
			Skill_Kyotsugo,
			Skill_Sperethiel,
			Skill_Theran,
			Skill_Xhax,
		}),
		Specialty_Degree      : 3,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			// languages of esoteric power that a traveler might have some familiarity with by
			// consorting with spirits
			Skill_Cilaghai,
			Skill_Enochian,
			Skill_Graecan,
			Skill_Latin,
			Skill_Thari,
			Skill_Sumerian,
		}),
		Specialty_Degree      : 1,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			Skill_Cartography,
			Skill_Dancing,
			Skill_Diplomacy,
			Skill_Empathy,
			Skill_Introspection,
			Skill_Legend_Lore,
			Skill_Linguistics,
			Skill_Literacy,
			Skill_Meditation,
			Skill_Navigation,
			Skill_Orienteering,
			Skill_Rhetoric,
			Skill_Seamanship,
			Skill_Vocalization,
		}),
		Specialty_Degree      : 10,
		Specialty_Recommended : 2,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			Skill_Appraisal,
			Skill_Archery,
			Skill_Axe,
			Skill_Bludgeon,
			Skill_Centering,
			Skill_Channeling,
			Skill_Combat_Reflexes,
			Skill_Qlippotic_Lore,
			Skill_Dagger,
			Skill_Gaze_Weapon,
			Skill_Dodge,
			Skill_Enchantment,
			Skill_Eructation,
			Skill_Find_Weakness,
			Skill_First_Aid,
			Skill_Flail,
			Skill_Hammer,
			Skill_Hardiness,
			Skill_Instant_Stand,
			Skill_Lack_of_Weakness,
			Skill_Legerdemain,
			Skill_Lockpicking,
			Skill_Massive_Blow,
			Skill_Mounted_Combat,
			Skill_Ownership,
			Skill_Pole_Arm,
			Skill_Precision_Strike,
			Skill_Recuperation,
			Skill_Resilience,
			Skill_Rune_Lore,
			Skill_Shield,
			Skill_Sling,
			Skill_Spear,
			Skill_Staff,
			Skill_Stamina,
			Skill_Steadiness,
			Skill_Stealth,
			Skill_Sword,
			Skill_Telesmatic_Weapon,
			Skill_Throwing,
			Skill_Traps,
			Skill_Unarmed_Combat
		}),
		Specialty_Degree      : 4,
		Specialty_Recommended : 1,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : Skill_Class(Skill_Class_Fieldcraft)->query_skill_class_contents(),
		Specialty_Degree      : 3,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			Skill_Arcane_Lore,
			Skill_Astrology,
			Skill_Balance,
			Skill_Break_Fall,
			Skill_Climbing,
			Skill_Concentration,
			Skill_Cosmology,
			Skill_Courage,
			Skill_Discipline,
			Skill_Elude_Pursuit,
			Skill_Fishing,
			Skill_Imagination,
			Skill_Law,
			Skill_Lip_Reading,
			Skill_Load_Bearing,
			Skill_Locksmithing,
			Skill_Memory,
			Skill_Pain_Tolerance,
			Skill_Poetry,
			Skill_Practice,
			Skill_Prose,
			Skill_Riding,
			Skill_Running,
			Skill_Swimming,
			Skill_Training,
		}),
		Specialty_Degree      : 2,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			Skill_Armour_Adaptation,
			Skill_Somatesthesia,
			Skill_Breath_Control,
			Skill_Carousing,
			Skill_Disguise,
			Skill_Eavesdropping,
			Skill_Elder_Lore,
			Skill_Fast_Talk,
			Skill_Finance,
			Skill_Flight,
			Skill_History,
			Skill_Leadership,
			Skill_Philosophy,
			Skill_Picking_Pockets,
			Skill_Psychology,
			Skill_Quickness,
			Skill_Subordination,
			Skill_Symbology,
			Skill_Theology,
			Skill_Weapon_Lore,
		}),
		Specialty_Degree      : 1,
	]));
	add_affiliation_specialty(([
		Specialty_Skills      : ({
			Skill_Torture,
		}),
		Specialty_Forbidden   : 2,
	]));
}

descriptor array query_affiliation_subjective_specialties(object who) {
	descriptor array out = ::query_affiliation_subjective_specialties(who);
	object link = who->query_affiliation_link(project_control());
	unless(link)
		return out;
	string array bestowals = link->travelers_query_bestowals();
	foreach(string bestowal : bestowals) {
		descriptor array specs = Travelers_Bestowal(bestowal)->query_bestowal_specialty_access();
		if(specs)
			out += specs;
	}
	return out;
}

string travelers_flower_color() {
    string flower_color = random_element(({
        "paisley",
        "iridescent",
        "pink",
        "light blue",
        "prismatic",
        "milky",
    }));
    flower_color += "-";
    flower_color += random_element(({
        "mottled",
        "flecked",
        "spotted",
        "banded",
        "striped",
        "speckled",
        "streaked",
        "tinted",
    }));
    flower_color += " ";
    flower_color += random_element(Travelers_Rainbow);
    return flower_color;
}
