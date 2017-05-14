#include <Travelers.h>
#include <daemon.h>

inherit "/std/definition";
inherit "/mod/basic/credits";

nosave private mapping adjustments;

void set_challenge_component_selection_adjustments(mapping what) {
    adjustments = what;
}
    
mapping query_challenge_component_selection_adjustments() {
    return adjustments || ([]);
}   
