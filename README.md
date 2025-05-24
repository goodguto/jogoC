
# 🎮 The Plumber Duo

**The Plumber Duo** é um jogo de plataforma 2D desenvolvido em linguagem C utilizando a biblioteca gráfica **Raylib**.

O objetivo do jogo é subir nas plataformas geradas aleatoriamente até alcançar o topo, desviando de inimigos (quadrados vermelhos) e coletando moedas (quadrados amarelos) no caminho. O jogo contabiliza quantas moedas você coletou e o tempo gasto para finalizar, salvando essas informações no ranking.

---


## 🎮 Controles

### Menu:
- **↑ / ↓** → Navegar
- **Enter** → Selecionar

### Jogador 1 (Setas):
- **← / →** → Movimentar
- **↑** → Pular

### Jogador 2 (WASD):
- **A / D** → Movimentar
- **W** → Pular

### Geral:
- **ESC** → Voltar ao menu ou sair do ranking

---

## 📂 Estrutura de Pastas do Projeto

```
/assets      → imagens, sons e recursos
/include     → arquivos de cabeçalho (.h)
/src         → arquivos fonte (.c)
/obj         → arquivos objeto (.o) gerados na compilação
ranking.txt  → arquivo gerado com o ranking
Makefile     → script de compilação
README.md    → este arquivo
```

---

## 📥 Como Instalar a Biblioteca Raylib

### ✔️ No Ubuntu, WSL ou Linux:

Instalação rápida via apt:

```bash
sudo apt install libraylib-dev
```

Ou instalando manualmente:

```bash
sudo apt install git build-essential
git clone https://github.com/raysan5/raylib.git
cd raylib
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```





---

## 🔧 Como Compilar e Executar

### ✔️ Usando o Makefile (Recomendado):

Para compilar:

```bash
make
```

Para executar:

```bash
./main
```

Para limpar os arquivos gerados:

```bash
make clean
```

---

### ✔️ Compilar Manualmente:

```bash
gcc src/*.c -Iinclude -o main -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
```

E para executar:

```bash
./main
```

---

## 🏆 Sistema de Ranking

- O jogo salva automaticamente:
  - ✅ O número de moedas coletadas.
  - ✅ O tempo (em segundos) que você levou para completar o jogo.

- O ranking é armazenado no arquivo `ranking.txt` na raiz do projeto.

- Para visualizar o ranking, use a opção **"Ver Ranking"** no menu inicial do jogo.

---

## 👨‍💻 Tecnologias e Conceitos Usados

- Linguagem C
- Biblioteca gráfica [Raylib](https://www.raylib.com/)
- Estruturas de dados:
  - ✅ Structs
  - ✅ Ponteiros
  - ✅ Alocação dinâmica (`malloc`, `free`)
  - ✅ Listas encadeadas
  - ✅ Matrizes (representação de posições no plano 2D)
  - ✅ Leitura e escrita em arquivos (`ranking.txt`)

---

[Vídeo do jogo](https://youtu.be/zY2CGH6Urwc?feature=shared)

## 🚀 Desenvolvido por:


|Nome                         |Email da school   |
|-----------------------------|------------------|
|Heloísa Borba de Barros Silva|hbbs@cesar.school |
|João Pedro Dornelas          |jprda@cesar.school|

