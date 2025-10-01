# Ordenação Externa em C (Intercalação de Vários Caminhos + QuickSort Externo)

Este projeto implementa **ordenação externa** para grandes volumes de dados armazenados em disco, usando:
- **Intercalação de Vários Caminhos** (Multiway Merge), com e sem seleção;
- **QuickSort Externo**.

Opera sobre um arquivo binário de dados (`PROVAO.bin`) e arquivos temporários de fita (`fita_0.bin` … `fita_39.bin`).

---

## Arquivos
- `ordena.c` – ponto de entrada (CLI, orquestração, E/S e métricas)
- `IntercalacaoVariosCaminhos.c/.h` – implementações de multiway merge
- `QuickSortExterno.c/.h` – implementação do quicksort externo
- `Structs.h` – tipos e constantes compartilhados (`F = 20` fitas por lado, total de 40)

---

## Compilação
Comando único (GCC):
```bash
gcc ordena.c IntercalacaoVariosCaminhos.c QuickSortExterno.c -o exe -Wall
```

(Se preferir objetos: `gcc -c *.c -Wall && gcc *.o -o exe`.)

---

## Uso
```
./exe <metodo> <quantidade> <situacao> [-P]
```

**Parâmetros**
- `<metodo>`: `1` = Multiway Merge; `2` = Multiway Merge + Seleção; `3` = QuickSort Externo  
- `<quantidade>`: número de registros a processar (0 < N ≤ 471705)  
- `<situacao>`: `1` = origem ascendente, `2` = descendente, `3` = aleatória  
- `-P` (opcional): imprime o conteúdo **antes** e **depois** da ordenação

**Fonte dos dados**  
O programa lê um arquivo texto para gerar o binário automaticamente:
- `1` → `PROVAO_ASC.txt`
- `2` → `PROVAO_DES.txt`
- `3` → `PROVAO_ALE.txt`

Cria/substitui `PROVAO.bin` e, para os métodos de intercalação, usa as fitas temporárias `fita_0.bin` … `fita_39.bin`.

---

## Exemplos
```bash
# Multiway Merge (origem aleatória, 100k registros)
./exe 1 100000 3

# Multiway Merge + Seleção (origem ascendente, com impressão)
./exe 2 200000 1 -P

# QuickSort Externo (origem descendente, 50k registros)
./exe 3 50000 2
```

---

## Saída e Métricas
Após a execução, o programa imprime métricas (tempo e contadores de leituras/gravações/comparações).  
Com `-P`, também imprime o conjunto antes e depois da ordenação.

---

## Notas
- Considere ignorar ou versionar arquivos `.bin` com Git LFS.  
- As fitas temporárias são recriadas a cada execução.  
- Testado com GCC em Linux/WSL. Qualquer compilador C ISO deve funcionar.

---

## Licença
Livre para uso educacional e acadêmico.
