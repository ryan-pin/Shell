Shell em C++ implementa um interpretador de comandos simples que suporta comandos internos (exit, pwd, cd, history, history -c) e externos. Ela mantém um histórico dos últimos 10 comandos digitados e permite limpar o histórico com history -c. Comandos externos são executados usando fork e execve.
