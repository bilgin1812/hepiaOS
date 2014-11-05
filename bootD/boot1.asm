[BITS 16]				; mets nasm en mode 16 bits
[ORG 0x7C00]			; ce code est enregistrer à 0x7c00



;-----------------------STACK----------------------------------------;
;Set the base of the stack a little above where BIOS
mov bp,0x8000
mov sp,bp  


;---------Display Message---------------------------------------;

;----------définition représentation à l'écran
MOV AH, 0x0E			; mode teletype
MOV BH, 0x00			; mode page zero
MOV BL, 0x07			; gris léger

MOV SI, HelloString 	; place l'adresse de la phrase dans le registre SI
Call PrintString		; appel la fonction d'impression

jmp LoadKern
;-------Fonction Impression 

PrintString:  			;

MOV AL, [SI]			; Met le contenu du pointeur SI dans registre AL
OR AL, AL				; Compare si zéro
JZ exit_function		; Si oui on quitte
Call PrintCharacter     ; Si non, appel impression du charactère à l'écran
INC SI					; On passe à l'adresse suivante
jmp PrintString		; On revient au début de la boucle

exit_function:
RET

PrintCharacter: 		; impression à l'écran

INT 0x10  				
RET 

;------------------ Bloc de données -------------------;
HelloString db 'Bootlader charged ', 0; 


;--------------------- Load kernel procedure
LoadKern:
        mov ah, 0x02    ; Read Disk Sectors
        mov al, 0x01    ; Read one sector only (512 bytes per sector)
        mov ch, 0x00    ; Track 0
        mov cl, 0x02    ; Sector 2
        mov dh, 0x00    ; Head 0

        mov dl,dl; 0x00    ; dl is the boot disk
        mov bx, 0x2000  ; Segment 0x2000
        mov es, bx      ;  again remember segments bust be loaded from non immediate data
        mov bx, 0x0000  ; Start of segment - offset value
readsector:
        int 0x13        ; Call BIOS Read Disk Sectors function
        jc readsector  ; If there was an error, try again

        mov ax, 0x2000  ; Set the data segment register
        mov ds, ax      ;  to point to the kernel location in memory

        jmp 0x2000:0x0000       ; Jump to the kernel





;--------------Padding, Signature ---------------------;
; $ is current line, $$ is first line, db 0 is a 00000000 byte
; So, pad the code with 0s until you reach 510 bytes
TIMES 510 - ($ - $$) DB 0x90

; Fill last two bytes (a word) with the MBR signature 0xAA55
DW 0xAA55
 




;---------Notes personnelles-----------------------;
;Le registre CS contient l'adresse de la prochaine instruction a exécuter













  		
