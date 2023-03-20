# O "problema"

Na leitura sequencial do livro, chega-se ao capítulo 5, onde apresenta-se uma célula de memória:
	
![Célula de memória](./CelulaDeMemoria.png)

Fonte: STALLINGS, Computer Organization and Architectures, 10thEd.

Os circuitos são compostos por MOSFETs e, no caso do quadro (b), podem até ser agrupados em um flip-flop tipo RS e lógica de seleção, ou em dois inversores e lógica de seleção.

Flip-flops são apresentados na seção 11.4. Isto é feito a partir de portas lógicas, o que é tradicional, mas os circuitos das portas lógicas a partir de MOSFETs não é apresentado. O funcionamento e a estrutura física dos MOSFETs também não é apresentado, embora o MOSFET de porta flutuante seja mostrado (livro, figura 5.15), talvez por não ser um componente tradicional. Diagramas, como o do esquemático de partes da memória FLASH (livro, figura 5.16), mostram os MOSFETs de porta flutuante representados pelos símbolos dos MOSFETs tradicionais.

Acredito que isto justifique a elaboração de um texto que apresente algo sobre eletricidade, talvez, a partir do acender de uma lâmpada ou LED até a construção de uma porta lógica ou de uma célula de memória.

## Resistor

### Lei de Ohm

## Lâmpada comparada ao LED
 
Uma lâmpada de filamento (lâmpada de Edison) é, fisica e eletricamente, muito parecida com um resistor. É constituída por um filamento cujas extremidades são conectadas aos polos de uma fonte de energia. O aquecimento do filamento faz com que ele emita luz. Este mesmo aquecimento faz o material do filamento reagir com o ar (oxidação/combustão), o que modifica a composição do filamento, geralmente, fazendo que ele pare de emitir luz.

Um LED é um diodo. *Light Emitting Diode* = LED. Fisica e eletricamente bastante distinto de um resistor. Um diodo é composto por dois "blocos" de semicondutor com dopagens diferentes. A propriedade considerada mais importante num diodo é a passagem da corrente elétrica em uma só direção. A emissão de (e sensibilidade a) luz é uma propriedade "colateral", descoberta depois, e muito usada atualmente.

Conectados corretamente, é possível dizer que um LED é equivalente a uma lâmpada. Por exemplo, ambas acendem quando passa por elas corrente suficiente. 

## Chaves (interruptores)

São componentes que interrompem a passagem de corrente.

### Os circuitos do vestibular (ou seria do ENEM?)

Geralmente não se explica além dos interruptores mecânicos o que induz associar que "é necessária uma pessoa para acionar o interruptor".

### Indo um pouco além do vestibular

Interruptores de acionamento elétrico (https://en.wikipedia.org/wiki/Relay) existem há mais de dois séculos. O relé é um componente eletromecânico.

Válvulas termiônicas (https://en.wikipedia.org/wiki/Vacuum_tube), um *spinoff* das lâmpadas, também podem ser usadas como interruptores de acionamento elétrico. Válvulas usam vácuo ou gases ionizáveis em seu interior.

Transistores (https://pt.wikipedia.org/wiki/Trans%C3%ADstor) também podem ser usados como interruptores de acionamento elétrico. Todos os transistores têm três terminais. A função de cada um varia de acordo com o tipo.

Existem muitos tipos de transistores que diferem nos encapsulamentos e nos princípios de operação. Indo direto ao ponto, os transistores usados em processadores, memórias, ... são MOSFETs, de Metal-Oxide Silicon Field Effect Transistor. Suas características físicas e elétricas e a melhor adequação dos processos de fabricação são as vantagens que provocaram a escolha dessa tecnologia.

MOSFETs "individuais" são componentes proporcionalmente caros e menos comuns que transistores bipolares (BJT). 

Há MOSFET de canal P e de canal N. Os terminais de um MOSFET são *Fonte*, *Dreno* e *Porta*. 

Há BJT PNP e NPN. Os terminais de um BJT são *Emissor*, *Coletor* e *Base*.

![](./photo1679342357.jpeg)

Nos usos a seguir um MOSFET de canal P equivale a um BJT PNP e um MOSFET de canal N equivale a um BJT NPN.

É possível apresentar essa equivalência usando simuladores de circuito.

https://www.tinkercad.com/things/6VaiTyRdRat-glorious-bruticus-luulia/editel?tenant=circuits

**nota**: Em livros em que o tema principal não são os circuitos, seus diagramas podem estar simplificados, por exemplo, omitindo componentes como resistores. Às vezes a ausência do resistor resulta em um curto-circuito. Abstratamente, nada ocorre, a idéia até é transmitida corretamente. Na prática, em determinadas condições, o circuito queima.

#### Transistor como chave

## Inversor (porta lógica)


![](./photo1679353103.jpeg)


## Célula de memória


![](./photo1679352856.jpeg)


Inspiração para circuito de endereçamento: https://www.researchgate.net/figure/Circuit-diagram-of-an-optimized-4H-SiC-4T-SRAM-cell-The-optimized-cell-operates-on-a_fig1_314296600

O circuito com BJT tem uma não conformidade pois as linhas de bit são somente para escrita na célula de memória. A linha de bit da célula de memória costuma ser bidirecional, algo que fica claro no diagrama do livro.

## Implementação da célula de memória com BJT

![](./photo1679353482.jpeg)

