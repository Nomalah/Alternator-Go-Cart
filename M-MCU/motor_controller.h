#pragma once

#include "pindefs.h"
#include "../message.h"

namespace MotorController {
    static byte duty_cycle_ = 0;
    static bool hi_a_ = false;
    static bool hi_b_ = false;
    static bool hi_c_ = false;
    static bool li_a_ = false;
    static bool li_b_ = false;
    static bool li_c_ = false;

    static bool hi_d_ = false;
    static bool hi_e_ = false;
    static bool hi_f_ = false;
    static bool li_d_ = false;
    static bool li_e_ = false;
    static bool li_f_ = false;

    void IRAM_ATTR updateMosfets(){

    }

    void IRAM_ATTR step1_abc() {
        hi_a_ = false;
        li_a_ = true;
        hi_b_ = true;
        li_b_ = false;
        hi_c_ = false;
        li_c_ = false;
    }

    void IRAM_ATTR step2_abc() {
        hi_a_ = false;
        li_a_ = true;
        hi_b_ = false;
        li_b_ = false;
        hi_c_ = true;
        li_c_ = false;
    }

    void IRAM_ATTR step3_abc() {
        hi_a_ = false;
        li_a_ = false;
        hi_b_ = false;
        li_b_ = true;
        hi_c_ = true;
        li_c_ = false;
    }

    void IRAM_ATTR step4_abc() {
        hi_a_ = true;
        li_a_ = false;
        hi_b_ = false;
        li_b_ = true;
        hi_c_ = false;
        li_c_ = false;
    }

    void IRAM_ATTR step5_abc() {
        hi_a_ = true;
        li_a_ = false;
        hi_b_ = false;
        li_b_ = false;
        hi_c_ = false;
        li_c_ = true;
    }

    void IRAM_ATTR step6_abc() {
        hi_a_ = false;
        li_a_ = false;
        hi_b_ = true;
        li_b_ = false;
        hi_c_ = false;
        li_c_ = true;
    }

    void IRAM_ATTR step1_def() {
        hi_d_ = false;
        li_d_ = true;
        hi_e_ = true;
        li_e_ = false;
        hi_f_ = false;
        li_f_ = false;
    }

    void IRAM_ATTR step2_def() {
        hi_d_ = false;
        li_d_ = true;
        hi_e_ = false;
        li_e_ = false;
        hi_f_ = true;
        li_f_ = false;
    }

    void IRAM_ATTR step3_def() {
        hi_d_ = false;
        li_d_ = false;
        hi_e_ = false;
        li_e_ = true;
        hi_f_ = true;
        li_f_ = false;
    }

    void IRAM_ATTR step4_def() {
        hi_d_ = true;
        li_d_ = false;
        hi_e_ = false;
        li_e_ = true;
        hi_f_ = false;
        li_f_ = false;
    }

    void IRAM_ATTR step5_def() {
        hi_d_ = true;
        li_d_ = false;
        hi_e_ = false;
        li_e_ = false;
        hi_f_ = false;
        li_f_ = true;
    }

    void IRAM_ATTR step6_def() {
        hi_d_ = false;
        li_d_ = false;
        hi_e_ = true;
        li_e_ = false;
        hi_f_ = false;
        li_f_ = true;
    }

    void beep(int freq, int durationms){ // freq <= 500000hz
        int periodus = 1000000/freq;
        int halfPeriodus = periodus/2;
        int durationus = durationms * 1000;
        for (int elapsed = 0; elapsed < durationus; elapsed += periodus){
            step1_abc();
            updateMosfets();
            delayMicroseconds(halfPeriodus);
            step2_abc();
            updateMosfets();
            delayMicroseconds(halfPeriodus);
        }
    }

    void startupBeep() {

    }

    void begin() {
        attachInterrupt(HS_A, step1_abc, RISING);
        attachInterrupt(HS_C, step2_abc, FALLING);
        attachInterrupt(HS_B, step3_abc, RISING);
        attachInterrupt(HS_A, step4_abc, FALLING);
        attachInterrupt(HS_C, step5_abc, RISING);
        attachInterrupt(HS_B, step6_abc, FALLING);

        attachInterrupt(HS_D, step1_def, RISING);
        attachInterrupt(HS_F, step2_def, FALLING);
        attachInterrupt(HS_E, step3_def, RISING);
        attachInterrupt(HS_D, step4_def, FALLING);
        attachInterrupt(HS_F, step5_def, RISING);
        attachInterrupt(HS_E, step6_def, FALLING);

        startupBeep();
    }
     
    void update(message new_values) {
        
    }

} // namespace MotorController
