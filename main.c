/*
 * Author: Deepanshu Tanwar
 * Inputs: PA0(Board button) and PA1(External button)
 * Outputs: PD14(Board Red LED) and PD12(Board Green LED)
 * 
 * Working: We have used two buttons PA0 and PA1 when either is pressed and hold for more than debounce time then
 *          respective LED will glow up
 *          for PA0: Red LED will glow up(PD14)
 *          for PA1: Green LED will glow up(PD12)
 */


#include <stdint.h>

#define DEBOUNCE_TIME 1000000 // Debounce time in cycles
#define BUTTON0_MASK 0x01     // Bit mask for button on PA0
#define BUTTON1_MASK 0x02     // Bit mask for button on PA1

void Fdelay(uint32_t delay) {
    for (uint32_t i = 0; i < delay; i++) {
        // Wait
    }
}

int main() {
    uint32_t *pClkCtrlReg = (uint32_t *)0x40023830;
    uint32_t *pPortAModeReg = (uint32_t *)0x40020000;
    uint32_t *pPortDModeReg = (uint32_t *)0x40020C00;
    uint32_t *pPortAInputReg = (uint32_t *)0x40020010;
    uint32_t *pPortDOutputReg = (uint32_t *)0x40020C14;
    uint32_t *pPortAPullUpDownReg = (uint32_t *)0x4002000C;

    // Enable the clock for GPIOA and GPIOD peripherals
    // For GPIOA
    *pClkCtrlReg |= 0x01;

    // For GPIOD
    *pClkCtrlReg |= (0x01 << 3);

    // Configure PortA and Pin0 as input
    *pPortAModeReg &= ~(0x03);

    // Configure PortA and Pin1 as input
    *pPortAModeReg &= ~(0x03 << 2);

    // Configure PortD and Pin14 as output
    *pPortDModeReg &= ~(0x03 << 28); // Reset
    *pPortDModeReg |= (0x01 << 28);  // Set

    // Configure PortD and Pin12 as output
    *pPortDModeReg &= ~(0x03 << 24);
    *pPortDModeReg |= (0x01 << 24);

    // Configure PortA and Pin1 to pull-up
    *pPortAPullUpDownReg &= ~(0x03 << 2);
    *pPortAPullUpDownReg |= (0x01 << 2);

    while (1) {
        volatile uint32_t pin0 = (*pPortAInputReg & BUTTON0_MASK);
        volatile uint32_t pin1 = (*pPortAInputReg & BUTTON1_MASK);

        if (pin0) {
            Fdelay(DEBOUNCE_TIME); // Wait for debounce time
            if (pin0) {
                *pPortDOutputReg |= (0x01 << 14);
            }
        } else {
            *pPortDOutputReg &= ~(0x01 << 14);
        }

        if (!pin1) {
            Fdelay(DEBOUNCE_TIME); // Wait for debounce time
            if (!pin1) {
                *pPortDOutputReg |= (0x01 << 12);
            }
        } else {
            *pPortDOutputReg &= ~(0x01 << 12);
        }
    }
}
