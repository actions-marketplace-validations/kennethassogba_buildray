---
title: Dans GitHub Actions
---

# Surveiller le temps de build en CI {#watch-build-time-in-ci}

Buildray est disponible sur le [GitHub Marketplace](https://github.com/marketplace/actions/buildray-build-time).

La GitHub Action compare une pull request à sa révision de base. Elle construit et teste les deux versions, puis mesure leurs builds complets en alternance sur le même runner.

## Ajouter le workflow {#add-the-workflow}

Créez `.github/workflows/build-time.yml` dans votre projet :

```yaml
name: Build time
on: [pull_request]
permissions:
  contents: read
jobs:
  buildray:
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@v6
        with:
          fetch-depth: 0
      - uses: kennethassogba/buildray@v0.13.0
        with:
          source: .
          cmake-args: '["-DBUILD_TESTING=ON"]'
          runs: '5'
          jobs: '2'
          max-regression-percent: '5'
```

Cet exemple utilise une version publiée. Pour une référence immuable, utilisez son SHA complet. Adaptez l’option de tests CMake à votre projet. L’Action installe ses outils sur Ubuntu 24.04 et compile la CLI ; ce travail de préparation est hors des temps comparés. Elle fonctionne aussi directement depuis le dépôt.

## Retrouver les résultats {#find-the-results}

Le résumé du job affiche la comparaison. Ses artefacts conservent les rapports et, pour une expérience d’optimisation, le patch. La rétention est de 30 jours par défaut. Le contrôle échoue si un ralentissement dépasse le budget et le seuil de bruit.

L’Action n’applique pas le patch et n’ouvre pas de PR. Les sources et les tests exécutent le code du projet : utilisez les mêmes règles de confiance que pour votre workflow de build habituel.

Les liens symboliques relatifs vers des fichiers de la copie des sources sont pris en charge. Les liens absolus, les liens vers des répertoires, les liens cassés et ceux qui sortent de la copie sont refusés.

## Évaluer un autre dépôt {#evaluate-another-repository}

Dans Buildray, ouvrez **Actions → Evaluate a repository → Run workflow**, puis indiquez un dépôt public, une révision et ses options de tests CMake. Choisissez `clean-deps`, `clean` ou `measure`. Aucun fork ni nouveau fichier de workflow n’est nécessaire. Les artefacts conservent les rapports, les tests, le patch éventuel et les révisions exactes des sources.

Pour le nettoyage des dépendances, ajoutez `rebuild_files`, par exemple `["src/model.cc"]`, afin de mesurer les rebuilds après modification de ces fichiers. Les mesures répétées des builds complets sont alors omises ; les deux variantes restent entièrement compilées et testées. Laissez une liste vide pour mesurer les builds complets. Trois paires sont mesurées par défaut ; un résultat incertain ne compte pas comme un gain.

## Essayer une optimisation {#try-an-optimization}

Dans l’étape Buildray, ajoutez par exemple ces entrées à `with` :

```yaml
command: clean-deps
runs: '3'
jobs: '2'
```

`command: clean` nettoie les includes ; `command: clean-deps` cherche les bibliothèques statiques privées inutiles et choisit LLD automatiquement. Les anciens noms restent disponibles.

Les expériences d’optimisation comparent le projet avec sa propre copie modifiée. Elles ignorent `baseline-ref`, qui sert à la comparaison entre révisions.

Le PCH est désactivé par défaut. Il demande un choix explicite, `command: pch` ; le nettoyage des includes et des dépendances ne l’active pas.

Si le projet ignore volontairement certains tests à l’exécution, `allow-baseline-skips: 'true'` permet de conserver ces mêmes exceptions pendant la validation. Elles sont comptées séparément des tests réussis. Par défaut, tout test ignoré bloque la validation.

La comparaison automatique exporte la base avec `git archive`. Elle ne prépare donc pas les sous-modules Git ni les contenus Git LFS. Si votre projet en dépend, préparez les deux sources et utilisez la CLI `compare` dans votre workflow.

Les [entrées, sorties et limites complètes de l’Action](https://github.com/kennethassogba/buildray/blob/main/action.yml) et le [guide CI technique](https://github.com/kennethassogba/buildray/blob/main/docs/ci.md) couvrent les autres modes.
