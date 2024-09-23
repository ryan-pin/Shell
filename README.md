# Shell em C++

Este projeto implementa uma shell simples em C++ que suporta comandos internos e externos.

## Comandos Internos
- **exit**: Finaliza o shell.
- **pwd**: Exibe o diretório atual.
- **cd**: Muda o diretório de trabalho.
- **history**: Exibe o histórico dos últimos 10 comandos digitados.
- **history -c**: Limpa o histórico de comandos.

## Comandos Externos
Comandos externos são executados utilizando as funções **fork** e **execve**.

## Histórico de Comandos
O shell mantém um histórico dos últimos 10 comandos digitados e permite limpá-lo com o comando **history -c**.
