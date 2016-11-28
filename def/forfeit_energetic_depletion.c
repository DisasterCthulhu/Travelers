#include <Travelers.h>
#include <comestible.h>
#include <conditions.h>
#include <daemon.h>

inherit Travelers_Definition("Forfeit");

void configure() {
    ::configure();
    set_forfeit_name("energetic depletion");
    set_forfeit_rarity(Rarity_Common);
    set_forfeit_value(Travelers_Forfeit_Value_Low);
    set_forfeit_initialize_description("the depletion of all energy stores");
    set_forfeit_message(([
        Message_Content                         : ({
            'a', "{{dazzling shadowy}strange shifting shadow} passes over", 0
        }),
        Message_Senses                          : Message_Sense_Astral,
    ]));
    set_forfeit_display(([
        Message_Content                         : ({
            0, ({ "feel", 0 }), ({ 'l', 0 }), "being drained of energy",
        }),
        Message_Senses                          : Message_Sense_Spiritual | Message_Sense_Kinesthetic,
        Message_Color                           : "status: danger",
    ]));
    set_forfeit_process((:
        object who = $1->ganesha_challenge_query_owner();
        foreach(object energy : Daemon_Energy->query_energies()) {
            unless(energy->query_energy_retrieve_maximum() || energy->query_energy_capacity_skills())
                continue;
            unless(who->query_energy_amount(energy) > 0)
                continue;
            who->set_energy_amount(energy, 0);
        }
        object array chakras = filter(all_inventory(who), (: $1->is_chakra() :));
        foreach(object chakra : chakras)
            chakra->add_chakra_energy(-1000);
        object array matrices = filter(all_inventory(who), (: $1->is_psionic_matrix() && $1->query_psionic_matrix_user() == $2 :), who);
        foreach(object matrix : matrices)
            matrix->set_psionic_matric_energy(0);
        return;
    :));
}
