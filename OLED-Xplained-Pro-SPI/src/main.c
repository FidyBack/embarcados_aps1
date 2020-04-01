/************************************************************************/
/* includes                                                             */
/************************************************************************/
#include "asf.h"
#include "musics.h"
#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/
// Musica
typedef struct  {
	int *mel;
	int *t;
	int size;
} Music;

// Led 1
#define LED1_PIO			PIOA
#define LED1_PIO_ID			ID_PIOA
#define LED1_PIO_IDX		0
#define LED1_PIO_IDX_MASK	(1u << LED1_PIO_IDX)

// Led 2
#define LED2_PIO			PIOC
#define LED2_PIO_ID			ID_PIOC
#define LED2_PIO_IDX		30
#define LED2_PIO_IDX_MASK	(1u << LED2_PIO_IDX)

// Botão 1
#define BUT1_PIO			PIOD
#define BUT1_PIO_ID			ID_PIOD
#define BUT1_PIO_IDX		28
#define BUT1_PIO_IDX_MASK	(1u << BUT1_PIO_IDX)

// Botão 2
#define BUT2_PIO			PIOC
#define BUT2_PIO_ID			ID_PIOC
#define BUT2_PIO_IDX		31
#define BUT2_PIO_IDX_MASK	(1u << BUT2_PIO_IDX)

// Buzzer
#define WOODY_PIO			PIOA
#define WOODY_PIO_ID		ID_PIOA
#define WOODY_PIO_IDX		4
#define WOODY_PIO_IDX_MASK	(1u << WOODY_PIO_IDX)

/************************************************************************/
/* Variaveis Globais                                                    */
/************************************************************************/
volatile char flag_but_1 = 0;
volatile char flag_but_2 = 0;

/************************************************************************/
/* Protótipos                                                           */
/************************************************************************/
void init(void);
void monofony(Music music);
void but1_callBack(void);
void but2_callBack(void);

/************************************************************************/
/* CallBack / Handler													*/
/************************************************************************/
void but1_callBack(void){
	flag_but_1 = 1;
}

void but2_callBack(void) {
	flag_but_2 = 1;
}

/************************************************************************/
/* Functions                                                            */
/************************************************************************/
// Função de inicialização do uC
void init(void) {
	// Inicia OLED
	gfx_mono_ssd1306_init();
		
	// Ativa os periféricos que serão usados
	pmc_enable_periph_clk(LED1_PIO_ID);
	pmc_enable_periph_clk(LED2_PIO_ID);
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(WOODY_PIO_ID);
	
	// Inicia os periféricos de saida
	pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 1, 0, 0);
	pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 1, 0, 0);
	pio_set_output(WOODY_PIO, WOODY_PIO_IDX_MASK, 1, 0, 0);
	
	// Inicia os periféricos de entrada
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	
	// Inicia os Callbacks
	pio_handler_set(BUT1_PIO, BUT1_PIO_ID, BUT1_PIO_IDX_MASK, PIO_IT_RISE_EDGE, but1_callBack);
	pio_handler_set(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK, PIO_IT_RISE_EDGE, but2_callBack);
	
	// Ativa interrupção
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	
	// Configura NVIC
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_EnableIRQ(BUT2_PIO_ID);
}


void monofony(Music music) {
	for (int note = 0; note < music.size; note++){

		long delay = 1000000/music.mel[note]/2;
		int duration = 1000/music.t[note];
		long cycles = music.mel[note]*duration/1000;
		
		if (flag_but_1){
			flag_but_1 = 0;
			delay_ms(100);
			while(1) {
				if (flag_but_1) {
					flag_but_1 = 0;
					break;
				}
			}
		}
		
		for(int i = 0; i < cycles; i++){
			pio_set(WOODY_PIO, WOODY_PIO_IDX_MASK);
			pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
			delay_us(delay);
			pio_clear(WOODY_PIO, WOODY_PIO_IDX_MASK);
			pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
			delay_us(delay);
		}
		delay_ms(duration);
	}
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void) {
	init();
	// Inicia o Board
	board_init();
	// Inicia o Clock do sistema
	sysclk_init();
	// Desativa o WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// Declaração das variáveis de cada música
	Music mario, underworld;
	
	mario.mel = &mario_melody;
	mario.t = &mario_tempo;
	mario.size = sizeof(mario_melody)/sizeof(int);
	
	underworld.mel = &underworld_melody;
	underworld.t = &underworld_tempo;
	underworld.size = sizeof(underworld_melody)/sizeof(int);

	int music = 0;

	while (1) {
		if (flag_but_2){
			if (music < 1){
				music++;
			} else{
				music = 0;
			}
			flag_but_2 = 0;
		}
		if (music == 0) {
			pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
			monofony(underworld);
		}
		if (music == 1) {
			pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);
			monofony(mario);
		}
	}
	return 0;
}

