#include <Travelers.h>
#include <daemon.h>

internal mapping adjustments;

inherit "/std/definition";

void set_challenge_component_selection_adjustments(mapping what) {
    adjustments = what;
}
    
mapping query_challenge_component_selection_adjustments() {
    return adjustments || ([]);
}   
