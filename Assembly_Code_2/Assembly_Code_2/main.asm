.include "m328def.inc" 
.org 0x0000
	RJMP begin ; jump to begin  

.org 0x0004 ; identify the push-button/change
	RJMP exint1 ; jump to sub-routine to do the job when button pressed

.org 0x0034 
	begin:
		CLI
        LDI		R16,low(RAMEND)
        OUT     SPL,R16
        LDI     R16,high(RAMEND)
        OUT     SPH, R16     
		 
		LDI		R16, (0<<ISC11) | (1<<ISC10) ; to identify change from button
		STS		EICRA, R16 ; load for EICRA

		LDI		R16, (1<<INT1)
		OUT		EIMSK, R16 ; instruction
		            
        LDI		R16,0x80
        OUT		DDRD, R16 ; data direction for all D pins

		sei

	main:
        LDI		R16,0xFF 
        OUT		PORTD, R16   
        RCALL	Delay

        LDI		R16,0x00 ; R16 on
        OUT     PORTD, R16 ; activate D pins to give a value
        RCALL	Delay

		RJMP	main

	  
	exint1: ; func to turn led off for a while when button pressed
		LDI R16, 0x00
		out PORTD, R16
	
		RCALL Delay
		reti

	
	Delay:
		LDI R17, 0xFF 
		LDI R18, 0xFF ; Made 3 loops as the required value can't be achieved in hexadecimal (for that high delay of 1s)
		LDI R19, 0x1F

	loop:   
		DEC R17 
        BRNE loop
             
		  
	loop2:   
		LDI R17, 0xFF 
		DEC R18
		BRNE loop   

	loop3:   
		LDI R18, 0xFF
		DEC R19
		BRNE loop2		
		RET 