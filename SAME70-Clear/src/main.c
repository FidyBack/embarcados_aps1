/************************************************************************/
/* includes                                                             */
/************************************************************************/
#include "asf.h"
#include "musics.h"

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
#define LED1_PIO           PIOC
#define LED1_PIO_ID        ID_PIOC
#define LED1_PIO_IDX       19
#define LED1_PIO_IDX_MASK  (1u << LED1_PIO_IDX)

// Led 2
#define LED2_PIO           PIOD
#define LED2_PIO_ID        ID_PIOD
#define LED2_PIO_IDX       26
#define LED2_PIO_IDX_MASK  (1u << LED2_PIO_IDX)

// Botão 1
#define BUT1_PIO			PIOA
#define BUT1_PIO_ID			ID_PIOA
#define BUT1_PIO_IDX		2
#define BUT1_PIO_IDX_MASK	(1u << BUT1_PIO_IDX)

// Botão 2
#define BUT2_PIO			PIOD
#define BUT2_PIO_ID			ID_PIOD
#define BUT2_PIO_IDX		30
#define BUT2_PIO_IDX_MASK	(1u << BUT2_PIO_IDX)

// Buzzer
#define WOODY_PIO			PIOA
#define WOODY_PIO_ID		ID_PIOA
#define WOODY_PIO_IDX		19
#define WOODY_PIO_IDX_MASK	(1u << WOODY_PIO_IDX)

/************************************************************************/
/* Protótipos                                                           */
/************************************************************************/
void init(void);
void monofony(Music music);

/************************************************************************/
/* Functions                                                            */
/************************************************************************/
// Função de inicialização do uC
void init(void) {
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
}

void monofony(Music music) {
	for (int note = 0; note < music.size; note++){

		long delay = 1000000/music.mel[note]/2;
		int duration = 1000/music.t[note];
		long cycles = music.mel[note]*duration/1000;
		
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

	while (1) {
		monofony(underworld);
	}
	return 0;
}