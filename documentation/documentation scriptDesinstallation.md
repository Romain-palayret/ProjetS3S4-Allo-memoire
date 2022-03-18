# Description générale :
Le script scriptDesinstallation est un script bash qui prend en argument le nom d'un paquet.
Il essayera de désinstaller le paquet en question, et il redirige la sortie standart et la sortie d'erreur dans un Log.
Il écrira dans le log la date et l'heure à laquelle a eu lieu la tentative de désinstallation du paquet, qu'elle ai marchée ou pas.

```
# On récupère le chemin du log
var=$(sed -n '3p' /usr/bin/serec.config)
chemin=${var##* }      

# On écrit la date dans le log
Ladate=$(date)  
echo -e \\n$Ladate \\n >>$chemin/LogInstallation.log

# On désinstalle le paquet pris en argument et on redirige la sortie standart et la sortie d'erreur dans le log.
apt-get -y remove --purge $1 2>/dev/null >>$chemin/LogInstallation.log
```
