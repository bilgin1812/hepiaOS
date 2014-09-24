[BITS 16]				; mets nasm en mode 16 bits
[ORG 0x8000]			; ce code est enregistrer à 0x7c00


MOV SI, HelloString 	; place l'adresse de la phrase dans le registre SI
Call PrintString		; appel la fonction d'impression
      
HLT	
;

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

;----------définition représentation à l'écran---------;
MOV AH, 0x0E			; mode teletype
MOV BH, 0x00			; mode page zero
MOV BL, 0x07			; gris léger

INT 0x10  				; Gris léger
RET 

;------------------ Bloc de données -------------------;
HelloString db 'KERNELL', 0; 










  		
