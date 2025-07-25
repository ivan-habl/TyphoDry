#include "vars.h"

float temperature;
float relative_humidity;
float temperature_setpoint;
int solid_relay_st;
float time_setpoint;
int32_t timer_seconds;
int32_t drier_state = 0;
float absolute_humidity;

float get_var_temperature() {
    return temperature;
}

void set_var_temperature(float value) {
    temperature = value;
}

float get_var_relative_humidity() {
    return relative_humidity;
}

void set_var_relative_humidity(float value) {
    relative_humidity = value;
}

float get_var_absolute_humidity() {
    return absolute_humidity;
}

void set_var_absolute_humidity(float value) {
    absolute_humidity = value;
}

float get_var_temperature_setpoint() {
    return temperature_setpoint;
}

void set_var_temperature_setpoint(float value) {
    temperature_setpoint = value;
}

void set_var_solid_relay_st(int32_t value) {
    solid_relay_st = value;
}

int32_t get_var_solid_relay_st() {
    return solid_relay_st;
}

float get_var_time_setpoint() {
    return time_setpoint;
}

void set_var_time_setpoint(float value) {
    time_setpoint = value;
}

int32_t get_var_timer_seconds() {
    return timer_seconds;
}

void set_var_timer_seconds(int32_t value) {
    timer_seconds = value;
}

int32_t get_var_drier_state() {
    return drier_state;
}

void set_var_drier_state(int32_t value) {
    drier_state = value;
}
