.include "m328def.inc" 
.org 0x0000 ; Reset
RJMP begin; Jump to begin  
.org 0x0034  ; Set R16 register high through LDI

begin:
	CLI  ; disable interrupts during timed/ avoid interrupts
	LDI    R16,low(RAMEND)
		OUT    SPL,R16 ; set stack pointer to top of ram
    LDI     R16,high(RAMEND) ; load  register(R16) to high
        OUT    SPH, R16                
    LDI    R16,0xFF ; 
        OUT    DDRD, R16   ; Control direction(out/in) of port
    LDI     R16,0xFF ; Turn on as high(loads value 0xFF to the register)
        OUT    PORTD, R16 (Contains value for port + sends data to PORT D as (PORT"D")
        RCALL  Delay ; Calls our function delay
    LDI     R16,0x00 ; Turn of as low value send, Loads 0x00 to R16
        OUT     PORTD, R16 ; Sends 0x00 from register R16 to PORTD as loaded above
        RCALL  Delay

	; Loading high values to Registers
Delay:
	LDI R17, 0xFF
	LDI R18, 0xFF
	LDI R19, 0x1F

	; Made 3 loops as the required delay cant be achieved in hexadecimal with one/two
loop:
	DEC R17 
    BRNE loop ; Branch if not equal
             
loop2:
	LDI R17, 0xFF 
	dec R18
	BRNE loop   

loop3:
	LDI R18, 0xFF
	dec R19
	BRNE loop2		
	RET  ; Return to main
