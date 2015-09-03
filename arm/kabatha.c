#include <Travelers.h>
#include <configuration.h>
#include <damage_types.h>
#include <interval.h>
#include <item.h>
#include <locations.h>
#include <metrics.h>
#include <realtime.h>
#include <transmute.h>

inherit "/std/item";

private descriptor resistance_mod;
private descriptor intrinsic_immunities;

// kabatha control metrics

status is_kabatha() {
	return True;
}

varargs float kabatha_query_owner_spiritual_communion_factor(object who) {
	who ||= query_user();
	unless(who)
		return 0.0;
	int communion = Metric_Retrieve("spiritual communion with Ganesha", who);
	return scale_conversion(to_float(communion), 50.0, 250.0, 0.1, 1.0);
}

varargs float kabatha_query_owner_pancharatnam_invocation_factor(object who) {
	who ||= query_user();
	unless(who)
		return 0.0;
	int pancharatnam_sonority = Travelers_Invocation("pancharatnam")->query_invocation_sonority(who);
	return scale_conversion(to_float(pancharatnam_sonority), 0.0, 100.0, 0.1, 1.0);
}

varargs float kabatha_query_owner_s_communion_with_ganesha(object who) {
	who ||= query_user();
	return 100 * kabatha_query_owner_spiritual_communion_factor(who);
}

varargs float kabatha_query_total_facility(object who) {
	who ||= query_user();
	return kabatha_query_owner_s_communion_with_ganesha(who) * kabatha_query_owner_pancharatnam_invocation_factor(who);
}

void kabatha_configure(object who) {
	float level = kabatha_query_total_facility(who); // make sure we pass the owner here, not the wearer
	set_will_update_configuration(True);
	switch(round(level)) {
	case 41..75                                 :
		set_craft(Craft_Excellent);
		break;
	case 76..99                                 :
		set_craft(Craft_Exquisite);
		break;
	case 100                                    :
		set_craft(Craft_Unearthly);
		break;
	default                                     :
		set_craft(Craft_Good);
		break;
	}
	update_configuration(True);
}

varargs void kabatha_update_resistances(object who, mixed types) {
	object owner = who->query_owner();
	object link = owner && owner->query_affiliation_link(project_control());
	unless(link)
		return;
	int amount = round(diminishing_returns(kabatha_query_total_facility(owner), 50));
	if(types) {
		foreach(string type : types)
			armour()->set_granted_resistance(type, amount);
		return;
	}
	string array resistances = 0;
	string array bestowals = link->travelers_query_bestowals();
	foreach(string bestowal : bestowals) {
		string array types = Travelers_Bestowal(bestowal)->query_bestowal_kabatha_resistances();
		if(types)
			foreach(string type : types)
				array_push(resistances, type);
	}
	if(resistances)
		foreach(string type : resistances)
			armour()->set_granted_resistance(type, amount);
}

void kabatha_do_equip(mapping args) {
	object who = args["who"];
	kabatha_update_resistances(who);
}

void kabatha_dissipate() {
	message(([
		Message_Content : ({
			0, ({ "evanesce", 0 }), "into a {{gold}majestic golden cloud}, which swiftly",
			({ "dissipate", 0 }), "into nothingness",
		}),
		Message_Senses : Message_Sense_Visual | Message_Sense_Astral,
	]));
	remove();
}

mixed kabatha_do_unequip(mapping args) {
	kabatha_dissipate();
}

mixed kabatha_do_drop(mapping args) {
	kabatha_dissipate();
}

void configure() {
	::configure();
	set_creator("marcosy");
	alter_identity(([
		Identity_Code                           : "kabatha",
		Identity_Known_Nouns                    : ({ "kabatha" }),
		Identity_Flags                          : Identity_Flag_Suppress_Name_Code,
	]));
	alter_identity(Identity_Special_Names, ({ "KABATHA" }), Identity_Alter_Add);
	set_craft(Craft_Good);
	set_indestructible(True);
	armour()->set_armour_type(Armour_Type_Ophiscutia);
	armour()->set_granted_resistances(([
		"holy"                                  : 100,
		"order"                                 : 100,
	]));
	//add_description(Description_Type_Generic); // makes the description look dumb
	add_description(({
		"this", ({ "are", 0 }), 'a', "complex assembly of interlapping {{gold}golden bands} and {{gold}gold} leaves,",
		"arranged with", Description(Description_Type_Craftsmanship_Adjective), "craftsmanship into a flowing embracement of armour that",
		({ "appear", 0 }), "to be mystically intertwined and intergrown with the scales of", ({ 'r', 0, "wearer" }),
	}));
	add_known_description(([
		Description_Type                        : Description_Type_Viewer_Condition,
		Description_Index                       : ([
			Condition_Type_Code                 : Condition_Type_Total,
			Condition_Value                     : 60,
			Condition_Info                      : ({
				([
					Condition_Type_Code         : Condition_Type_Skill,
					Condition_Info              : Skill_Theology,
				]),
				([
					Condition_Type_Code         : Condition_Type_Skill,
					Condition_Info              : Skill_Legend_Lore,
				]),

			}),
		]),
		Description_Content                     : ({
			"It is said that Ganesha sends kabatha, divine armour, to protect the vahanas, or mounts, of his faithful who might otherwise",
			"be stricken by the abhorrence of harm coming unto Ganesha's divine gift",
		}),
	]));
	add_proportion(([
		Element_Type                            : Material_Gold,
		Element_Proportion                      : 1.0,
		Element_Flags                           : Element_Flag_Untransmutable | Element_Flag_Suppress_Material_Known_Adjectives | Element_Flag_Provide_Default_Color,
	]));
	add_proportion(([
		Element_Type                            : Material_Goetiam,
		Element_Proportion                      : 1.0,
		Element_Flags                           : Element_Flag_Untransmutable | Element_Flag_Secondary,
	]));
	set_light(0.05);
	set_transactional_container(False);
	set_configuration_freeze(Configuration_Container_Status, True);
	add_hook(Do_Equip_Item, #'kabatha_do_equip);
	add_hook(Do_Unequip_Item, #'kabatha_do_unequip);
	add_hook(Do_Drop, #'kabatha_do_drop);
}

status query_auto_keep(object who) {
	object owner = who && who->query_owner();
	return owner && owner->query_affiliation(project_control());
}
