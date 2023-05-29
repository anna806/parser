# parser

A parser program futtatásához szükség van a **libwebp** csomag telepítésére.

A programot a repository-ban található makefile segítségével lehet egyszerűen fordítani. A parancssorból ki kell adni egy make utasítást, melynek hatására lefutnak a makefile-ban megadott parancsok. Végeredményként előáll a futtatható program, melyet kétféleképpen lehet parancssorból elindítani.

1. ./parser -caff [path].caff => Egy caff kiterjesztésű fájl feldolgozásához a -caff kapcsoló szükséges.
2. ./parser -ciff [path].ciff => Egy ciff kiterjesztésű fájl feldolgozásához a -ciff kapcsoló szükséges.

Sikeres futás esetén a program mappájában létrejön egy webp kiterjesztésű fájl a megadott fájl nevével.