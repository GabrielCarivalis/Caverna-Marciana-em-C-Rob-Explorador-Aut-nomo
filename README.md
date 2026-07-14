Caverna Marciana — Robô Explorador Autônomo

Projeto desenvolvido em C como trabalho acadêmico de programação. 

O objetivo é implementar um módulo decisor autônomo para um robô explorador que deve navegar por uma caverna marciana desconhecida, encontrar uma poça d'água e retornar à entrada — sem morrer em poços de lava.

Sobre o jogo:

A caverna é representada como uma matriz gerada aleatoriamente. O robô não tem visão global do mapa: a cada rodada recebe apenas sua posição atual e quantos vizinhos imediatos contêm lava. A dinâmica é similar à do campo minado — é preciso deduzir o que é seguro com informação local.

O que foi implementado:

O módulo decisor (Decisor) mantém duas matrizes de memória: uma com o estado de cada célula (DESCONHECIDO, SEGURO, VISITADO, LAVA) e outra com os valores de calor registrados em cada posição visitada.

A exploração utiliza DFS iterativo com backtrack, onde o robô prioriza sempre células marcadas como seguras antes de recuar. A dedução de segurança das células é feita por um algoritmo de propagação em cascata inspirado no campo minado: se o número de lavas já conhecidas ao redor de uma célula bate com o sensor, todos os vizinhos desconhecidos restantes são marcados como seguros; se a soma de lavas e desconhecidos bate com o sensor, todos os desconhecidos são marcados como lava.
Quando o backtrack se esgota e ainda existem regiões desconhecidas no mapa, um BFS de navegação traça o caminho mínimo até a célula desconhecida mais próxima passando apenas por células já visitadas — evitando arriscar lava desnecessariamente.

Ao encontrar a água, um segundo BFS (algoritmo de ondas) calcula as distâncias de cada célula conhecida até a entrada (0,0), e o caminho de retorno ótimo é reconstruído e executado passo a passo.

Resultado:

O decisor foi testado em 5.700 jogos (57 configurações de caverna × 100 partidas cada), atingindo aproximadamente 90% de vitórias.

Tecnologias:

C puro, alocação dinâmica de memória, BFS, DFS com backtrack, dedução lógica estilo campo minado.
