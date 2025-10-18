gui.cpp é o código principal;

primitivas.h é onde tudo acontece;

# Adicionando paralelismo no projeto

Para o programa rodar com mais eficiência, você pode implementar o paralelismo da seguinte forma:

- Para GCC/G++, adicione a flag -fopenmp:
```g++ -fopenmp gui.cpp -o {programa}```

- Para Visual Studio:
Properties > C/C++ > Language > Open MP Support.

<img width="403" height="377" alt="image" src="https://github.com/user-attachments/assets/6c59c00d-8b1f-44bf-9ddd-514e78b85f38" />
<img width="403" height="375" alt="image" src="https://github.com/user-attachments/assets/ecc8c9e1-656e-40e9-ae86-7e9b3393561b" />

# Plotando com matplotlib-cpp

Foi adicionado o matplotlib-cpp para a plotagem do gráfico velocidade x raio. Mas, para que esta lib funcione, é necessário fazer as seguintes mudanças em seu compilador:
- Mudar a depuração de Debug para Release (pois no modo Debug se faz necessário o arquivo python311_d.lib);
- É necessário ter o Python instalado no computador. O matplotlib-cpp adicionado não foi uma versão oficial, mas sim uma revisão pois a versão oficial está desatualizada e não funciona com as versões mais recentes do Python;
- Mudar o padrão de linguagem do C++ para C++20;
- Em Propriedades do Projeto:
  - Adicionar ao C/C++ -> Diretório de Inclusão Adicionais: ```C:\Users\{Diretório do Python}\Python\Python311\include; C:\Users\{Diretório do Python}\AppData\Local\Programs\Python\Python311\Lib\site-packages\numpy\_core\include```;
  - Adicionar ao Vinculador -> Diretórios de Bibliotecas Adicionais: ```C:\Users\{Diretório do Python}\Python\Python311\libs```;
  - Adicionar ao Vinculador -> Entrada -> Dependências Adicionais: ```python311.lib```.

