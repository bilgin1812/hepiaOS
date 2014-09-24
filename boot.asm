[BITS 16]				; mets nasm en mode 16 bits
[ORG 0x7C00]			; ce code est enregistrer à 0x7c00

	; initialize segments
	; ...

	; setup stack
	; ...

	; reset drive
	; ...

	; display message
	

	; load kernel
	; ...

	; execute kernel
	; ...


;----------Code principale du bootloader-----------;

MOV SI, HelloString 	; place l'adresse de la phrase dans le registre SI
Call PrintString		; appel la fonction d'impression
jmp 0x8000				     
	


;---------Définition de toutes les fonctions utilisées dans le bootloader --------------------;

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
HelloString db 'le bloc est charge', 0; 


;--------------Padding, Signature ---------------------;
; $ is current line, $$ is first line, db 0 is a 00000000 byte
; So, pad the code with 0s until you reach 510 bytes
TIMES 510 - ($ - $$) DB 0x90

; Fill last two bytes (a word) with the MBR signature 0xAA55
DW 0xAA55
 




;---------Notes personnelles-----------------------;
;Le registre CS contient l'adresse de la prochaine instruction a exécuter













  		
