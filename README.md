# Assembler-IA32

### Alunos

Matrícula | Nome
--- | ---
16/0030081 | Hélio Adson Bernardo
16/0013615 | Luiz Antônio Borges Martins

### Requisitos

- Para compilar é necessário make e g++
- Também é necessário ter os cabeçalhos da biblioteca elfio no diretório /usr/include/elfio
- Foi usado padrão c++11

### Compilação

Execute o comando:
`make`

### Execução

Execute o comando:
`./assembler path/to/file.s`

A saída será o executável `output`

### Endereço de entrypoint

A depender de seu computador e sistema operacional o executável pode apresentar falha de segmentação por estar utilizando endereços para o início da seção de texto ou de dados incorreta, caso isso aconteça altere as macros `START_TEXT_ADDRESS` e `START_DATA_ADDRESS` no arquivo `inc/statement.hpp` para os valores adequados.
