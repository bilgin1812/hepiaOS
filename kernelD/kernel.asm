
[BITS 16]				; mets nasm en mode 16 bits
kernel:
;----------définition représentation à l'écran---------;
MOV AH, 0x0E			; mode teletype
MOV BH, 0x00			; mode page zero
MOV BL, 0x07			; gris léger

MOV SI, HelloString 	; place l'adresse de la phrase dans le registre SI
Call PrintString		; appel la fonction d'impression
      
;-------------- Fonction Impression --------------------;

PrintString:  			;

next_character:			;

MOV AL, [SI]			; Met le contenu du pointeur SI dans registre AL
OR AL, AL				; Compare si zéro
JZ exit_function		; Si oui on quitte
Call PrintCharacter     ; Si non, appel impression du charactère à l'écran
INC SI					; On passe à l'adresse suivante
jmp next_character		; On revient au début de la boucle

exit_function:

RET


PrintCharacter: 		; impression à l'écran

INT 0x10  				
RET 

;------------------ Bloc de données -------------------;
HelloString db 'Loading KERNEL', 0; 










  		
