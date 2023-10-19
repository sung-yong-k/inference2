#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_PIN PB0

const char morseCode[26][5] = {
    ".-",   // A
    "-...", // B
    "-.-.", // C
    "-..",  // D
    ".",    // E
    "..-.", // F
    "--.",  // G
    "....", // H
    "..",   // I
    ".---", // J
    "-.-",  // K
    ".-..", // L
    "--",   // M
    "-.",   // N
    "---",  // O
    ".--.", // P
    "--.-", // Q
    ".-.",  // R
    "...",  // S
    "-",    // T
    "..-",  // U
    "...-", // V
    ".--",  // W
    "-..-", // X
    "-.--", // Y
    "--.."  // Z
};

void blink(char* morse) {
    LED_DDR |= (1 << LED_PIN); // Set LED_PIN as output

    for (int i = 0; morse[i] != '\0'; i++) {
        if (morse[i] == '.') {
            LED_PORT |= (1 << LED_PIN); // Turn LED on
            _delay_ms(250);             // Delay for dot duration
            LED_PORT &= ~(1 << LED_PIN); // Turn LED off
            _delay_ms(250);             // Delay between dots and dashes
        } else if (morse[i] == '-') {
            LED_PORT |= (1 << LED_PIN); // Turn LED on
            _delay_ms(750);             // Delay for dash duration
            LED_PORT &= ~(1 << LED_PIN); // Turn LED off
            _delay_ms(250);             // Delay between dots and dashes
        }
    }
}

int main() {
    char input[] = "SOS"; // Word to be blinked in Morse code

    while (1) {
        for (int i = 0; input[i] != '\0'; i++) {
            char c = toupper(input[i]);

            if (c >= 'A' && c <= 'Z') {
                int index = c - 'A';
                blink(morseCode[index]);
            }

            _delay_ms(750); // Delay between letters
        }

        _delay_ms(2000); // Delay between words
    }

    return 0;
}