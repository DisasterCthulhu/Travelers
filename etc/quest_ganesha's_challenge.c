#include <Travelers.h>
#include <daemon.h>

inherit "/std/quest";

void configure() {
    ::configure();
    set_creator("chaos");
    set_name("ganesha's_challenge");
    set_desc("Ganesha's Challenge");
    set_hint(
        "The wanderers of the world of " + World_Name + " called Travelers can issue a challenge from their deity, Ganesha, to "
        "those who wish to undertake such a task.  Find a Traveler, receive a challenge, and complete it."
    );
    set_points(5);
    set_difficulties(({ 3, 3, 3 }));
}
