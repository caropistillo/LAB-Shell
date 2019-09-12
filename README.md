# LAB-SHELL

Respuestas a las preguntas del [Lab Shell](https://fisop.github.io/7508/lab/shell/) del curso Mendez-Simó de Sistemas Operativos (7508) FIUBA

## Parte 1: Invocación de comandos
#### Pregunta: ¿Cuáles son las diferencias entre la syscall execve(2) y la familia de wrappers proporcionados por la librería estándar de C (libc) exec(3)?

En primer lugar, tal como se aclara en la pregunta, exec(3) es una familia de wrappers. Esto nos indica que su propósito principal es abstraerse de los detalles de implementación para facilitar la escritura de programas. En el "man" de esta familia se indica: "son front-ends de la syscall exceve(2)". 
