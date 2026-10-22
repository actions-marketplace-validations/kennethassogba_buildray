---
title: La suite
---

# Élargir le nettoyage des dépendances {#test-dependency-cleanup-at-scale}

`prune-deps` traite les noms de cibles et les listes de bibliothèques stockés dans des variables CMake, y compris dans des boucles et des fonctions. Sur un projet de test contenant 40 bibliothèques inutiles, un essai et une vérification finale suffisent toujours. Les définitions des variables partagées restent intactes.

Sur XGrammar, retirer une dépendance inutilisée évite une étape de lien d’environ 32 secondes après une modification du code de GoogleMock. Modifier le parseur ne donne aucun gain confirmé. Le gain concerne cette reconstruction précise. Le nettoyage des dépendances n’a pas encore montré de gain sur un build complet de ces projets.

## Les prochaines étapes {#next-steps}

1. Résoudre les alias CMake, qui excluent encore de la recherche les liens des programmes SentencePiece.
2. Chercher les dépendances Highway restantes après le nettoyage accepté. Les copies redondantes d’archives sont désormais prises en charge, avec la même exigence de binaire identique.
3. Prendre en charge les fichiers d’arguments du compilateur pour éviter un contournement de la détection des modules dans les projets concernés.

Les [résultats du nettoyage](https://github.com/kennethassogba/buildray/blob/main/docs/unused-dependencies-validation.md) distinguent le projet de test des mesures sur les vrais projets. Ne trouver aucun retrait éligible ne signifie pas que toutes les dépendances sont nécessaires.

## Plus tard {#later}

Expliquer quelle dépendance déclenche chaque reconstruction. Puis étudier des cibles plus petites à partir de l’usage mesuré des symboles. Une grosse bibliothèque statique ne justifie pas à elle seule un découpage : le linker choisit déjà les fichiers objets dont il a besoin.
