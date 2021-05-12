# TRAB1_SB

Sistema Operacional onde o código foi feito/compilado/executado: Windows 10 Home Single Language

Sistema Operacional de 64 

g++ --version: (MinGW.org GCC Build-2) 9.2.0

Para compilar o montador, via vscode:

Abra a pasta no próprio vscode

Execute o comando: g++ main.cpp -o montador

Para executar o montador:

Execute o comando: ./montador nome_do_arquivo1.asm nome_do_arquivo2.asm nome_do_arquivo3.asm

Execute o comando: g++ ligador.cpp -o ligador

Para executar o ligador:

Execute o comando: ./ligador nome_do_arquivo1.o nome_do_arquivo2.o nome_do_arquivo3.o

Para compilar o simulador, via vscode:

Execute o comando: g++ simulador.cpp -o simulador

Para executar o simulador:

Execute o comando: ./simulador nome_do_arquivo.obj (saida do montador)

foi escolhido o formato: fat_mod_A.asm para funcionamento dos programas

