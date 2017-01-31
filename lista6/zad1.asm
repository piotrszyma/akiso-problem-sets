opt f-g-h+l+o+
                org $1000

start           equ *

                lda <text
                sta $80
                lda >text
                sta $81
                ldy #01
                lda #%11111010
                jsr phex

                lda <text
                ldx >text
                jsr $ff80
                brk

phex	          pha
        				jsr pxdig
        				pla
        				lsr @  ; Przesuniêcia bitowe w prawo
        				lsr @
        				lsr @
        				lsr @

pxdig	          and #%00001111  ;Wyzerowanie bitów oprócz ostatnich 4
        				ora #'0'    ; OR z zerem
        				cmp #'9'+1  ;Porównanie akumulatora z tym
        				bcc pr
        				adc #'A'-'9'-2 ;Jest to litera

pr				      sta ($80),y
        				dey
        				rts

                org $2000
text            equ *
                dta b(0),b(0)
                dta b(10) ; '\n'
                dta b(0)

                org $2E0
                dta a(start)

                end of file
