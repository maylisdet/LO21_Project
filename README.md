# TradingSimulator
TradingSimulator est une application de simulation de stratégie trading. 
L'utilisateur doit pouvoir simuler des achats/ventes de devises en fonction des cotations courantes dss paires.
Pour cela, il charger un fichier CSV dans lequel se trouve l'Open, High, Low, Close prices d'une paire de devise par jour. 
Une fois le fichier chargé, l'utilisateur choisit des paramètres de simulations : 
  - solde de la devise de base
  - solde de la devise de cotation 
  - pourcentage du brooker
  - les dates de début et fin de la simulation (la plage maximal est limitée par les dates du fichier csv)
L'application crée alors un graphique indiquant pour chaque jour la chandelle (formée à l'aide du cours OHLC) de la paire de devise.

L'application propose à l'utilisateur 3 modes de simulations : 
  - Manuel : L'utilisateur voit toutes les bougies et clique sur l'une d'entre elles pour acheter ou vendre
  - Pas à pas : L'utilisateur voit les bougies jour après jour, il peut décider d'acheter/vendre pour la journée en cours ou passer à la suivante. 
  Ce mode permet aussi d'annuler la dernière transaction. 
  - Automatique : Après le choix d'une stratégie choisie par l'application, les transactions se font automatiquement jour après jour. 
  Les stratégies sont définies à l'aide d'indicateurs financiers. 
  
  L'application permet a tout moment d'acceder à un editeur de texte et au tableau des transactions effectuées. 
