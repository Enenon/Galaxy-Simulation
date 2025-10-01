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




