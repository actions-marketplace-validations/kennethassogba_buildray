---
title: La suite
---

# Élargir le nettoyage des dépendances {#test-dependency-cleanup-at-scale}

`prune-deps` traite les noms de cibles et les listes de bibliothèques stockés dans des variables CMake, y compris dans des boucles et des fonctions. Sur un projet de test contenant 40 bibliothèques inutiles, un essai et une vérification finale suffisent toujours. Les définitions des variables partagées restent intactes.

Sur XGrammar, retirer une dépendance inutilisée évite une étape de lien d’environ 32 secondes après une modification du code de GoogleMock. Modifier le parseur ne donne aucun gain confirmé. Le gain concerne cette reconstruction précise. Le nettoyage des dépendances n’a pas encore montré de gain sur un build complet de ces projets.

Essayez un nouveau projet depuis **Actions → Evaluate a repository**. Le [guide CI](./ci#evaluate-another-repository) explique les entrées.

## Les prochaines étapes {#next-steps}

1. Comprendre les changements de lien rejetés sur MuJoCo à partir des commandes conservées, sans assouplir les contrôles sur les binaires.
2. Terminer la validation de la [draft Highway](https://github.com/google/highway/pull/3509), puis tester de nouveaux projets avec le workflow générique. Garder les nettoyages dont le gain mesuré est utile.
3. Prendre en charge les fichiers d’arguments du compilateur pour éviter le contournement de la détection des modules.

Les [résultats du nettoyage](https://github.com/kennethassogba/buildray/blob/main/docs/unused-dependencies-validation.md) distinguent le projet de test des mesures sur les vrais projets. Ne trouver aucun retrait éligible ne signifie pas que toutes les dépendances sont nécessaires.

## Plus tard {#later}

Expliquer quelle dépendance déclenche chaque reconstruction. Puis étudier des cibles plus petites à partir de l’usage mesuré des symboles. Une grosse bibliothèque statique ne justifie pas à elle seule un découpage : le linker choisit déjà les fichiers objets dont il a besoin.
