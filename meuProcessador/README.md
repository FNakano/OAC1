# Explicando a construção de uma unidade de processamento (em simulador), passo a passo, a partir de memórias, registradores, MUX, decodificadores e outros componentes com esse nível de complexidade ou mais simples.

## Motivação


1. Faz a conexão entre conceitos como operações lógicas, circuitos combinacionais e sequenciais, ULA, UC, registradores, máquina de estados, processador, memória, conjunto de instruções, arquitetura do conjunto de instruções, OpCodes, programa;
2. Apresentar um processador pronto, mesmo que simples, e explicá-lo costuma ser muito extenso e complicado; 
3. Desconheço trabalho semelhante.

## Resultados

### Considerações iniciais

#### É diferente do IAS, do ARM, de qualquer outro

- Porque não visa maximizar o uso dos blocos funcionais, nem minimizar a quantidade de componentes;
- visa maximizar a compreensão do circuito;

#### Escolha do comprimento da palavra de memória e da palavra de endereço.

A conexão de muitas vias, mesmo que simplificada no simulador, implica em uma certa carga cognitiva.

Mostrar os sinais auxilia muito a compreensão do funcionamento, mas barramentos muito largos são dificeis de ler;

O IAS usa palavra de memória de 40 bits dividido em 2 instruções. Cada instrução tem 8 bits de OpCode, 12 de endereço.

O simplecpudesign usa palavra de memória de 8 bits com 3 bits de OpCode. O autor menciona que 8 instruções é pouco.

O simplecpudesign usa endereço de 8 bits, o que acho pouco.

Processadores RISC em geral têm da ordem de 30 OpCodes.

Tentei com OpCodes de 4 bits e endereçamento de memória de 10 bits, mas 14 bits é ruim de separar os sinais (que geralmente são divididos em grupos de 1, 4, 8, 16). Então passei para endereçamento de 12 bits.

### Máquina que carrega conteúdo da memória endereçada por um contador em um registrador

Meta: Acessar as posições de memória sequencialmente e carregar o conteúdo armazenado na posição em um registrador. 

Motivo: Este é o funcionamento da busca de instrução quando não há desvio.

[Circuito em logisim](carregaInstrucao.circ)

![Tela do simulador apresentando o circuito.](Captura%20de%20tela%20de%202022-05-03%2010-03-29.png)

### Máquina que permite outras operações.

Meta: Criar instantes (bordas de clock) em que algo diferente de carga sequencial de memória possa ser feito.

Motivo: "Abrir espaço no ciclo de funcionamento da máquina" para criar outras instruções.

Acrescenta um contador e um decodificador. Cada saída do decodificador (no máximo uma pode estar em 1, qualquer que seja o instante) pode receber um significado e permitir as operações/sinais associados. Inspirado no simplecpudesign, o primeiro sinal corresponde à busca de instrução, o último ao incremento do contador de programa.

Um ciclo de CPU passa a ter 4 ciclos de clock.

[Circuito em logisim](BuscaIncrementa.circ)

![Tela do simulador apresentando o circuito](Captura%20de%20tela%20de%202021-05-04%2014-34-19.png)

Extra: botão de reset para reiniciar a unidade de processamento (UP). Agora o estado da UP inclui o estado do contador (passo no ciclo do processador).

### Máquina que interpreta os 12 LSB da instrução como endereço e acessa esse endereço se o OpCode for 0b0001

Defini no início que os 4 bits mais significativos em uma instrução vão armazenar o OpCode. A partir deste circuito, eles passam a ter significado distinto. O OpCode 0b0001, os 12 bits do registrador de instrução (RI) são expostos no barramento de endereçamento de memória.

Para selecionar que sinal vai para o barramento - se a saída do contador de programa ou se os 12 bits do RI - uso um MUX de 12 bits de largura e 2 entradas.

[Circuito em logisim](Endereca.circ)

[Conteúdo da memória (carregar na memória para executar)](Endereca.mem)

![Tela do simulador apresentando o circuito](Captura%20de%20tela%20de%202021-05-04%2015-21-56.png)

### Máquina que armazena o conteúdo do endereço dos 12LSB em um registrador (acumulador) se o OpCode for 0b0001.

Esta é a primeira instrução implementada! O OpCode é 0001, mnemônico LDA(X), descrição carrega acumulador com conteúdo da memória endereçado por X. 

Acrescentei o registrador que corresponde ao acumulador. O armazenamento no acumulador ocorre na subida do clock ao FINAL do passo do processador (quando o LED com rótulo "execução da instrução" está apagando).

[Circuito em logisim](Carrega.circ)

[Conteúdo da memória (carregar na memória para executar)](Endereca.mem)

![Tela do simulador apresentando o circuito](Captura%20de%20tela%20de%202021-05-04%2015-58-00.png)

### Máquina que armazena o conteúdo do acumulador no endereço dos 12LSB se o OpCode for 0b0010.

Esta é a segunda instrução implementada! O OpCode é 0010, mnemônico STA(X), descrição armazena o conteúdo do acumulador na posição de endereço X da memória. 

Ajustei o controle do MUX de endereçamento e expus o sinal de gravação de memória. O armazenamento na memória ocorre na subida do clock ao FINAL do passo do processador (quando o LED com rótulo "execução da instrução" está apagando).

[Circuito em logisim](CarregaArmazena.circ)

[Conteúdo da memória (carregar na memória para executar)](CarregaArmazena.mem)

![Tela do simulador apresentando o circuito](Captura%20de%20tela%20de%202021-05-04%2016-22-05.png)

## <a id="2021-05-04-162551" href="#2021-05-04-162551">2021-05-04-162551</a>

Atualmente tem duas instruções implementadas: LDA(X) e STA(X). Acredito que seja suficiente como demonstração de como instruções são implementadas em um processador e como são executadas. A partir daqui, acrescentar uma ULA e os desvios de fluxo devem acrescentar uns MUX e circuitos combinacionais simples. O decodificador de instruções e o ciclo de execução têm "espaço" para tal. Acho que o mais complicado será acrescentar registradores de uso geral, as instruções para usá-los e os sinais para ativá-los.

## <a id="2021-05-06-094429" href="#2021-05-06-094429">2021-05-06-094429</a>

Implementei a instrução de salto incondicional.

Seria "só" fazer a carga paralela do endereço contido na instrução para dentro do contador de programa, ié: conectar os bits do endereço e, se o OpCode for para saltar, fazer load=1, mas... 

...tive problema por causa da lógica de controle do contador, que é diferente do que eu estou acostumado.

| Carga Paralela | Estado atual | Count | Load | Próximo Estado |
| --- | --- | --- | --- | --- |
| X | Q<sub>i</sub> | 0 | 0 | Q<sub>i</sub> |
| P | Q<sub>i</sub> | 0 | 1 | P |
| X | Q<sub>i</sub> | 1 | 0 | Q<sub>i</sub>+1 |
| X | Q<sub>i</sub> | 1 | 1 | Q<sub>i</sub>-1 |

**nota**: P é o sinal presente na entrada paralela. X significa que não é usado. O uso de X instruções é outro. Tenha cuidado!

... o contador decrementa se Count=1 e Load=1. Geralmente o componente tem um sinal up/down para selecionar se incrementa ou decrementa, mas não é o caso aqui.

A combinação count=1, load=1 acontecia (antes de colocar a porta AND, fazendo load= JMP + Execução) quando a instrução era JMP e era para não incrementar o PC (no passo seguinte ao de execução, que, por enquanto, está em branco. Isto fazia o contador de programa decrementar. Da forma como está agora, ele não decrementa, mas passa pelo passo de incremento, o que faz executar a partir do endereço seguinte ao do endereço-alvo, o que é igualmente ruim (circuito no logisim)[CarregaArmazenaSalta.circ]. 

- Uma solução seria reiniciar o contador de passos na etapa 3 quando a instrução for JMP, mas (ainda) não consigo imaginar os efeitos colaterais disso (além de um encurtamento do ciclo do processador).
- Outra seria anular o sinal de incremento do PC se a instrução for JMP.
- Outra seria colocar decodificadores para considerar o OpCode e o passo juntos - o circuito ficaria enorme....
- Outra seria escolher entre carga paralela e incrementar na última fase (acho que esta é a boa).
   - Acho que o que atrapalha meu raciocínio na hora de implementar é mudar dois sinais ao mesmo tempo. No caso, count=~load.
   - Na minha primeira tentativa, [circuito no logisim](CarregaArmazenaSalta2.circ), apenas fiz count=~load, o que fez o pc incrementar em todas as transições de clock (pois não acontece count=0 e load=0, o que faria não incrementar, que é o necessário nos passos diferentes da atualização do PC. Acho que tenho que fazer uma tabela-verdade.
      - fiz uma inversão na ordem das operações que nem sempre é verdade - e isso causou minha confusão: **a inversão**: se for JMP então count=~load, logo, **count=~load e load=JMP** (conclusão errada). Isto fez com que eu não avaliasse os casos em que JMP=0 (apresentação do erro seguido de mostração), onde count deve ser 0 e load deve ser 0, mostrando que count=~load não é sempre verdade (que foi o erro que cometi inicialmente).


| JMP | Atualiza | Estado Atual | Próximo Estado | count | load |
| --- | --- | --- | --- | --- | --- |
| 0 | 0 | Q<sub>i</sub> | Q<sub>i</sub> | ? | ? |
| 0 | 1 | Q<sub>i</sub> | Q<sub>i</sub> +1 | ? | ? |
| 1 | 0 | Q<sub>i</sub> | Q<sub>i</sub> | ? | ? |
| 1 | 1 | Q<sub>i</sub> | P | ? | ? |

**nota**: P é o sinal presente na entrada paralela.

Para realizar a TV acima, load e count precisam ser:


| JMP | Atualiza | Estado Atual | Próximo Estado | count | load |
| --- | --- | --- | --- | --- | --- |
| 0 | 0 | Q<sub>i</sub> | Q<sub>i</sub> | 0 | 0 |
| 0 | 1 | Q<sub>i</sub> | Q<sub>i</sub> +1 | 1 | 0 |
| 1 | 0 | Q<sub>i</sub> | Q<sub>i</sub> | 0 | 0 |
| 1 | 1 | Q<sub>i</sub> | P | 0 | 1 |

Resolvido!!! [circuito no logisim](CarregaArmazenaSalta3.circ)

**nota** ter o conteúdo da memória igual ao endereço da memória gera confusão na simulação pois os números iguais no registrador de instrução, e no barramento de endereçamento induzem ao engano.

[circuito 4 no logisim](CarregaArmazenaSalta4.circ) recoloca algumas conexões para colocar rótulos que correspondem aos mnemonicos junto ao decodificador de instruções. A [imagem de memória](CarregaArmazenaSalta4.mem) contém o programa para demonstrar em aula.

Formato da instrução: Palavra de 16 bits com os quatro primeiros armazenando o OpCode e os doze seguintes armazenando endereço (de operando ou alvo para salto).

![instrução na memória](CodeCogsEqn.gif)

<!-- $`\overbrace{bbbb}^{OpCode}\underbrace{bbbbbbbbbbb}_{X}`$ -->

| OpCode  | Mnemônico | Descrição |
| ------  | --------- | --------- |
| (0000)b | NOP       | Não operação |
| (0001)b | LDA(X)    | Carrega conteúdo do acumulador com conteúdo armazenado no endereço X da memória |
| (0002)b | STA(X)    | Armazena no endereço X da memória o conteúdo do acumulador |
| (0003)b | JMP X     | Salta execução para instrução armazenada no endereço X da memória |


![alt text](Captura%20de%20tela%20de%202021-05-06%2012-47-37.png)


## Discussão

Acredito que seja suficiente como exemplo didático, por um balanço entre poder explicativo, tamanho do circuito, tempo disponível para ampliações e existência de implementações mais completas na Internet.

A intenção deste projeto é mostrar os passos iniciais de **um** processo de elaboração, a fim de ter idéia do processo que levou à construção dos processadores apresentados por outros na internet (tanto didáticos quanto comerciais).

Implementar mais funcionalidades e as correspondentes instruções aumentaria o tamanho, aumentaria o tempo necessário para implementar e explicar essas funcionalidades, aproximaria este projeto de poder ser usado de fato (que ousadia!), mas necessita de tempo de que não disponho no curto prazo.

O resultado foi além do proposto pois ilustra as escolhas de projeto ligadas a arquitetura e organização, induz a pensar na escolha de instruções a implementar (*Instruction Set Architecture*).

Apresenta também alguns erros que cometi durante o projeto, como pensei e implementei a correção, à semelhança da implementação e depuração de um programa.

Próximos passos que podem ser interessantes são:

- Acrescentar registradores de uso geral e disponibilizá-los no conjunto de instruções;
- Implementar funções (CALL/RET) - requer registrador para armazenar endereço do topo da pilha;
- Acrescentar unidade lógica e aritmética e disponibilizá-la no conjunto de instruções;
- Implementar instruções de salto condicional;
- Implementar tratamento de interrupções;
- criar um montador (preferencialmente com uma linguagem de montagem compatível com alguma gerada pelo GCC para que o ASM gerado possa ser passado para este montador, aí passa a ser possível reusar os compiladores de GCC);


