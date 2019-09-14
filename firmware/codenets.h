/*
Codenets: https://github.com/ibnteo/codenets

ProMicro
R4 = PD1 (2)   (A3) PF4 = R1
R5 = PD0 (3)   (A2) PF5 = R2
C6 = PD4 (4)   (A1) PF6 = C3
C5 = PC6 (5)   (A0) PF7 = C4
R6 = PD7 (6)   (15) PB1 = R3
C7 = PE6 (7)   (14) PB3 = C2
C8 = PB4 (8)   (16) PB2 = C1
R0 = PB5 (9)   (10) PB6 = C0
*/

void Ports_Init() {
	// Init LEDs
	DDRB  |= 1<<0; // B0 (LED1)
	PORTB |= 1<<0; // LED Off

	DDRD  |= 1<<5; // D5 (LED2)
	PORTD |= 1<<5; // LED Off

	// Set 1
	DDRF  |= 1<<6;
	PORTF |= 1<<6;

	DDRF  |= 1<<7;
	PORTF |= 1<<7;

	DDRB  |= 1<<3;
	PORTB |= 1<<3;

	DDRB  |= 1<<2;
	PORTB |= 1<<2;

	DDRB  |= 1<<6;
	PORTB |= 1<<6;

	DDRB  |= 1<<4;
	PORTB |= 1<<4;

	DDRE  |= 1<<6;
	PORTE |= 1<<6;

	DDRC  |= 1<<6;
	PORTC |= 1<<6;

	DDRD  |= 1<<4;
	PORTD |= 1<<4;

	// PullUp
	DDRF &= ~(1<<4);
	PORTF |= 1<<4;

	DDRF &= ~(1<<5);
	PORTF |= 1<<5;

	DDRB &= ~(1<<1);
	PORTB |= 1<<1;

	DDRB &= ~(1<<5);
	PORTB |= 1<<5;

	DDRD &= ~(1<<7);
	PORTD |= 1<<7;

	DDRD &= ~(1<<0);
	PORTD |= 1<<0;

	DDRD &= ~(1<<1);
	PORTD |= 1<<1;
}

void LED_On() {
	PORTB &= ~(1<<0);
}
void LED_Off() {
	PORTB |= 1<<0;
}
void LED_Toggle() {
	PORTB ^= 1<<0;
}
void LED_Switch(bool on) {
	if (on) LED_On(); else LED_Off();
}

void LED2_On() {
	PORTD &= ~(1<<5);
}
void LED2_Off() {
	PORTD |= 1<<5;
}
void LED2_Toggle() {
	PORTD ^= 1<<5;
}
void LED2_Switch(bool on) {
	if (on) LED2_On(); else LED2_Off();
}

// D5 <=> B0

uint8_t Scan_Tick = 0;
uint16_t Chords_Scan[7];
void Keyboard_Scan() {
	if (Scan_Tick == 0) {
		Chords_Scan[0] = 0;
		Chords_Scan[1] = 0;
		Chords_Scan[2] = 0;
		Chords_Scan[3] = 0;
		Chords_Scan[4] = 0;
		Chords_Scan[5] = 0;
		Chords_Scan[6] = 0;

		// Set 0
		PORTB &= ~(1<<6); // C0
	} else if (Scan_Tick == 1) {
		if (! (PIND & 1<<1)) Chords_Scan[1] |= 1<<0;
		if (! (PIND & 1<<0)) Chords_Scan[2] |= 1<<0;
		if (! (PIND & 1<<7)) Chords_Scan[3] |= 1<<0;
		if (! (PINF & 1<<4)) Chords_Scan[4] |= 1<<4;
		if (! (PINF & 1<<5)) Chords_Scan[5] |= 1<<4;
		if (! (PINB & 1<<1)) Chords_Scan[6] |= 1<<4;

		// Set 1
		PORTB |= 1<<6;

		// Set 0
		PORTB &= ~(1<<2); // C1
		PORTB &= ~(1<<4); // C8
	} else if (Scan_Tick == 2) {
		if (! (PIND & 1<<1)) Chords_Scan[1] |= 1<<1;
		if (! (PIND & 1<<0)) Chords_Scan[2] |= 1<<1;
		if (! (PIND & 1<<7)) Chords_Scan[3] |= 1<<1;
		if (! (PINF & 1<<4)) Chords_Scan[4] |= 1<<3;
		if (! (PINF & 1<<5)) Chords_Scan[5] |= 1<<3;
		if (! (PINB & 1<<1)) Chords_Scan[6] |= 1<<3;

		// Set 1
		PORTB |= 1<<2;
		PORTB |= 1<<4;

		// Set 0
		PORTB &= ~(1<<3); // C2
	} else if (Scan_Tick == 3) {
		if (! (PINF & 1<<4)) Chords_Scan[4] |= 1<<2;
		if (! (PINF & 1<<5)) Chords_Scan[5] |= 1<<2;
		if (! (PINB & 1<<1)) Chords_Scan[6] |= 1<<2;
		if (! (PINB & 1<<5)) Chords_Scan[0] |= 1<<5;

		// Set 1
		PORTB |= 1<<3;

		// Set 0
		PORTE &= ~(1<<6); // C7
	} else if (Scan_Tick == 4) {
		if (! (PIND & 1<<1)) Chords_Scan[1] |= 1<<2;
		if (! (PIND & 1<<0)) Chords_Scan[2] |= 1<<2;
		if (! (PIND & 1<<7)) Chords_Scan[3] |= 1<<2;
		if (! (PINB & 1<<5)) Chords_Scan[0] |= 1<<0;

		// Set 1
		PORTE |= 1<<6;

		// Set 0
		PORTF &= ~(1<<6); // C3
	} else if (Scan_Tick == 5) {
		if (! (PINF & 1<<4)) Chords_Scan[4] |= 1<<1;
		if (! (PINF & 1<<5)) Chords_Scan[5] |= 1<<1;
		if (! (PINB & 1<<1)) Chords_Scan[6] |= 1<<1;
		if (! (PINB & 1<<5)) Chords_Scan[0] |= 1<<4;

		// Set 1
		PORTF |= 1<<6;

		// Set 0
		PORTD &= ~(1<<4); // C6
	} else if (Scan_Tick == 6) {
		if (! (PIND & 1<<1)) Chords_Scan[1] |= 1<<3;
		if (! (PIND & 1<<0)) Chords_Scan[2] |= 1<<3;
		if (! (PIND & 1<<7)) Chords_Scan[3] |= 1<<3;
		if (! (PINB & 1<<5)) Chords_Scan[0] |= 1<<1;

		// Set 1
		PORTD |= 1<<4;

		// Set 0
		PORTF &= ~(1<<7); // C4
	} else if (Scan_Tick == 7) {
		if (! (PINF & 1<<4)) Chords_Scan[4] |= 1<<0;
		if (! (PINF & 1<<5)) Chords_Scan[5] |= 1<<0;
		if (! (PINB & 1<<1)) Chords_Scan[6] |= 1<<0;
		if (! (PINB & 1<<5)) Chords_Scan[0] |= 1<<3;

		// Set 1
		PORTF |= 1<<7;

		// Set 0
		PORTC &= ~(1<<6); // C5
	} else if (Scan_Tick == 8) {
		if (! (PIND & 1<<1)) Chords_Scan[1] |= 1<<4;
		if (! (PIND & 1<<0)) Chords_Scan[2] |= 1<<4;
		if (! (PIND & 1<<7)) Chords_Scan[3] |= 1<<4;
		if (! (PINB & 1<<5)) Chords_Scan[0] |= 1<<2;

		// Set 1
		PORTC |= 1<<6;

		Chords[0] = Chords_Scan[0];
		Chords[1] = Chords_Scan[1];
		Chords[2] = Chords_Scan[2];
		Chords[3] = Chords_Scan[3];
		Chords[4] = Chords_Scan[4];
		Chords[5] = Chords_Scan[5];
		Chords[6] = Chords_Scan[6];
	}
	Scan_Tick ++;
	if (Scan_Tick > 20) { // Scan delay
		Scan_Tick = 0;
	}
}
