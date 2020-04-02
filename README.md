# embarcados_aps1
1- conecte a placa "OLED1 X PLANED pro" no suporte para periferico "EXT1", na sua placa SAME70.

 
2- conecte um pino de um buzzer no Pio A19, e o outro pino ligue no GND.

 
3- conecte um botao externo no Pio A2, e outro no Pio D30 da placa.


4- conecte a placa com um cabo no pc.


5- rode o arquivo aps.c.


O projeto é um tocador de musicas em um buzzer. Ele permite duas musicas, e alterna entre elas com o botão que está no Pio A2.
É possivel pausar a musica tocada ao apertar o botão que esta conectado com o Pio D30.


Para começar, um struct de cada uma das duas musicas foi criado, com notas, tammanhos, e tempos.
Assim, a função monofony reproduz a nota na frequecia calculada, pelo tempo determinado pela lista de tempo, e depois vai para a proxima nota, assim por diante.
Com interrupções e flags é possivel fazer as funções de pause e de trocar de musica.
Alem disso, é possivel ver o nome da musica que está tocando na tela do OLED.

 
Link para o video:https://youtu.be/BtykFkvqvRY
