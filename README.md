gui.cpp é o código principal;

primitivas.h é onde tudo acontece;

# Adicionando paralelismo no projeto

Para o programa rodar com mais eficiência, você pode implementar o paralelismo da seguinte forma:

- Para GCC/G++, adicione a flag -fopenmp:
```g++ -fopenmp gui.cpp -o {programa}```

- Para Visual Studio:
Properties > C/C++ > Language > Open MP Support.
