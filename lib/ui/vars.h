#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_NONE
};

// Native global variables

extern float get_var_temperature();
extern void set_var_temperature(float value);
extern float get_var_humidity();
extern void set_var_humidity(float value);
extern float get_var_temperature_setpoint();
extern void set_var_temperature_setpoint(float value);
extern int32_t get_var_solid_relay_st();
extern void set_var_solid_relay_st(int32_t value);
extern float get_var_time_setpoint();
extern void set_var_time_setpoint(float value);
extern int32_t get_var_timer_seconds();
extern void set_var_timer_seconds(int32_t value);
extern int32_t get_var_drier_state();
extern void set_var_drier_state(int32_t value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/