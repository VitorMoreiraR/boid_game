# Simulação de Movimentação de Pássaros 3D

Este projeto é uma simulação do movimento de um bando de pássaros com algoritmos básicos, desenvolvida em **C++** usando a biblioteca **OpenGL** (via **GLFW** e **GLEW**) para renderização.

## ✨ Recursos

* **Simulação de Voo:** Simulação da formação de voo com um pássaro líder.
* **Evasão de Obstáculos:** Os pássaros ajustam sua altitude (eixo Y) para evitar colidir com paralelepípedos e o cone central.
* **Controle de Câmera:** Três modos de visualização.
* **Controle de Velocidade:** Ajuste dinâmico da velocidade do bando.
* **Efeito de Névoa (Fog):** Ativado e desativado via tecla.
* **Pausar:** Possibilidade de pausar a simulação.
* **Redimensionar:** É possível redimensionar a tela.
* **Iluminação:** Iluminação básica do openGL.

## Recursos Técnicos

* **Linguagem:** C++
* **Gráficos:** OpenGL (Fixed Function Pipeline)
* **Bibliotecas:** **GLFW**, **GLEW**
* * **Sistema Operacional:** Linux(Ubuntu && WSL2)

## Como Executar

1.  É necessários ter as bibliotecas **GLFW**, **GLEW** e **GLU**.
2.  Execute o comando "make run".

## Controles do Usuário

| Tecla | Função | Descrição |
| :---: | :--- | :--- |
| **P** | Pausar/Retomar | Interrompe o movimento e outras ações, com exceção da câmera. |
| **A** | Adicionar Pássaro | Adiciona um pássaro na formação, caso não tenha passado do número limite(16). |
| **D** | Remover Pássaro | Remove um pássaro aleatório da formação. |
| **F** | Fog | Ativa/desativa o efeito de névoa (fog). |
| **Setas Direcionais (←/→)** | Virar o Bando | Altera a direção de voo do bando. |
| **Setas Direcionais (↑/↓)** | Acelerar/Desacelerar | Ajusta a velocidade de movimento e a taxa de batimento das asas. |
| **ESC** | Sair | Fecha a aplicação. |

### Modos de Câmera

A câmera pode ser alternada entre diferentes modos de visualização:

| Tecla | Modo de Câmera | Descrição |
| :---: | :--- | :--- |
| **C** | **Central** | Câmera acima do cone. |
| **Z** | **Bando** | Câmera atrás do bando. |
| **X** | **Vista Superior** | Câmera ortogonal de cima, olhando para baixo. |
