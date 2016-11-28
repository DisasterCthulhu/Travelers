#include <Travelers.h>
#include <metrics.h>
#include <skills.h>

inherit "/std/def/metric";

void configure() {
    ::configure();
    set_metric_name("spiritual communion with Ganesha");
    set_metric_data_type(T_NUMBER);
    set_metric_retrieve_value((:
        return $1->query_average_effective_skill(({ Skill_Order_Affinity, Skill_Sephirotic_Affinity, Skill_Goetic_Investiture }), ({}));
    :));
}
