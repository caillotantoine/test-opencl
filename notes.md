# Notes sur OpenCL

## Getting started
Un peu le Hello world de OpenCL. On regarde et on affiche ce qu'il y a sur notre machine et avec quoi on va pouvoir jouer ensuite.

### Compiler

Sur macOs :
```bash
gcc main.c -o platform_details -framework OpenCL 
```

### Les header de OpenCL

```cpp
#ifdef __APPLE__
    #include <OpenCL/opencl.h>
#else
    #include <CL/cl.h>
#endif
```

<hr><br>

### List the Plateform:

**Code typique:** Un exemple se trouve dans le fichier **`platform_details.c`**

- [`clGetPlatformIDs`](https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clGetPlatformIDs.html) est une fonction qui peut permettre de savoir combien de plateformes OpenCL sont disponibles et de les retourner. Le comportement change en fonction des arguments: si le premier est 0 et le second `NULL` alors le nombre de plateformes est retourné. En revanche, si le premier correspond au nombre de plateformes, le deuxième argument récupère les plateformes (le dernier est quant à lui `NULL`).


- [`clGetPlatformInfo`](https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clGetPlatformInfo.html) a un comportement similaire. Il peut soit retourner la taille des infos:
    ```cpp
    error = clGetPlatformInfo(id, param_name, 0, NULL, &paramSize);
    ```
    ou retourner les infos une fois que la mémoire a été allouée.

    ```cpp
    error = clGetPlatformInfo(id, param_name, paramSize, moreInfo, NULL);
    ```

<br>

_Note: [`alloca`](https://man7.org/linux/man-pages/man3/alloca.3.html) est une fonction qui permet de créer une zone mémoire allouée en mémoire locale. C'est comme un `malloc` sauf que la zone mémoire est automatiquement effacée lorsque la fonction est terminée._

<br>
<hr><br>

### List the Devices:

**Code typique:** Un exemple se trouve dans le fichier **`device_details.c`**

[`clGetDeviceIDs`](https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clGetDeviceIDs.html)
et 
[`clGetDeviceInfo`](https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clGetDeviceInfo.html) ont des comportements similaires à ce qu'expliqué plus haut. Cependant, `clGetDeviceInfo` donne des résultats en fonction de ce qui est demandé et chacun à une manière d'être traité différente.