#include <Travelers.h>
#include <autonomon.h>

inherit "/std/autonomon";

#define Divine_Amphiptere_Hyperspatiality_Mod	"Amphiptere_Mounted"
#define Amphiptere_Race							"amphiptere"

private void remove_rider_modifiers() {
    foreach(descriptor mod : query_trait_modifiers())
        if(Modifier_Query(mod, Modifier_Key) == Divine_Amphiptere_Hyperspatiality_Mod)
            remove_trait_modifier(mod);
}

void amphiptere_do_be_dismounted(mapping args) {
    remove_rider_modifiers();
}

void amphiptere_do_be_mounted(mapping args) {
    object who = args["who"];
    unless(who && who->query_affiliation(project_control()))
        return;
    int rider_hyperspatiality = who->query_trait(Trait_Hyperspatiality);
    if(rider_hyperspatiality && rider_hyperspatiality > 0) {
    add_trait_modifier(([
            Modifier_Index                          : Trait_Hyperspatiality,
            Modifier_Amount                         : rider_hyperspatiality,
            Modifier_Key                            : Divine_Amphiptere_Hyperspatiality_Mod,
        ]));
    }
}

status amphiptere_validate_race() {
	return query_race() == Amphiptere_Race;
}

void amphiptere_at_update_configuration(mapping args) {
    if(amphiptere_validate_race())
        return;
    remove_hook(Do_Be_Mounted, #'amphiptere_do_be_mounted);
    remove_hook(Do_Be_Dismounted, #'amphiptere_do_be_dismounted);
    remove_rider_modifiers();
    remove_hook(At_Update_Configuration, #'amphiptere_at_update_configuration);
}

void amphiptere_setuphooks() {
	unless(amphiptere_validate_race())
        return;
    require_hook(At_Update_Configuration, #'amphiptere_at_update_configuration);
    require_hook(Do_Be_Mounted, #'amphiptere_do_be_mounted);
    require_hook(Do_Be_Dismounted, #'amphiptere_do_be_dismounted);
}

void configure() {
    ::configure();
    set_race(Amphiptere_Race);
    set_level(20);
    add_description(Description_Type_Generic);
    add_skill_points(Skill_Swimming, 80);
    add_skill_points(Skill_Flight, 80);
    add_skill_points(Skill_Hardiness, 80);
    add_skill_points(Skill_Stamina, 80);
    add_skill_points(Skill_Recuperation, 80);
    add_skill_points(Skill_Regeneration, 80);
	amphiptere_setuphooks();
}

void reset() {
	::reset();
	amphiptere_setuphooks();
}
