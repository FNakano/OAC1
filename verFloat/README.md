A partir de um número em ponto flutuante, f, quão bom é usar sua representação como IEEE754 float como uma aproximação para log2(f) e, consequentemente, usar seu deslocamento de 1 bit à direita como a primeira aproximação para sqrt(f)?

Surpreendentemente, a resposta é: a aproximação é boa, a ponto de, refinando-a com um passo do algoritmo de Newton, chegar a um valor que pode ser usado para normalizar vetores em gráficos.

O que torna essa aproximação boa? Resp.: a forma como os bits são posicionados e interpretados.

Tomando a fórmula x=(+-)S*2^E, é razoavelmente claro que E é uma aproximação de log2(x). S, como (1.S)2 (representação normal) pode ser considerada uma boa aproximação da parte fracionária do log2(x). A disposição dos bits é 0ES pois o sinal é sempre positivo, o expoente tem uma questão pois é enviesado por 127 e o significando é *simplesmente utilizado* (raciocínio de uma heurística). Depois avalia-se se há justificativa para tal (por isso numa das referências diz-se que m é uma aproximação para log2(1+m) quando 1+m é o significando de um número normal.

Se considerar esse argumento correto, considerar a representação IEE754 de um float f como uma aproximação para log2(f), deslocar de 1 bit à direita, compensar o viés do expoente e interpretar o resultado como um novo float, digamos g, resulta que g é uma aproximação para sqrt(f).
 
