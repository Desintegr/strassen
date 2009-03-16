- contenu de l'archive

- explication de l'installation :
    - copier les fichiers à partir de devoir
    - OPENMP=0/1 dans Makefile

- on a gardé notre version la plus performante : C++ / OpenMP
- on a essayé d'autres versions :
    - version dérécursivée avec pthread
    - version récursive avec pthread

C++ :
    - implémentation de haut niveau, (surcharge des opérateurs *, +, -), code concis
    - ceci permet d'obtenir un code plus facilement lisible et maintenable
    - nous avons également cherché à optimiser cet algorithme de haut niveau en utilisant les fonctionnalités avancées du C++ comme les rvalue references (move semantics). Cette technique permet d'éviter un grand nombre de recopies et ainsi augmenter les performances

C++ / OpenMP :
    - on a essayé de //iser les petites opérations sur les matrices (addition, soustraction) mais cette version était beaucoup plus lente à cause de l'overhead provoqué par la création des threads. Le système passait plus de temps à gérer les threads que faire les calculs.
    - nous avons donc préféré mettre en // l'exécution de la méthode de strassen
    - utilisation du « nested parallelism » d'OpenMP
    - ce //isme est limité au niveau de la récursion par la formule log2(taille_matrice)/2 et nous permet d'obtenir un bon compromis entre le nombre de calcul effectué et le nombre de threads créés. Notre version OpenMP nous permet de gagner environ 12% par rapport à la même version sans OpenMP.

mettre tableau résumé, expliquer tests (5 fois), commande time
mettre graphique
préciser le processeur utilisé
