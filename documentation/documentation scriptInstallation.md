# Description générale :
Le script scriptInstallation est un script bash qui prend en argument le nom d'un paquet.
Il essayera d'installer le paquet en question, et redirige la sortie standart et la sortie d'erreur dans un Log.
Il ecrira dans le log la date et l'heure à laquelle à eu lieu la tentative d'installation du paquet, qu'elle ai marchée ou pas.

```
# On récupère le chemin du log
var=$(sed -n '3p' /usr/bin/serec.config)
chemin=${var##* }      

# On écrit la date dans le log
Ladate=$(date)  
echo -e \\n$Ladate \\n >>$chemin/LogInstallation.log

# On installe le paquet pris en argument et on redirige la sortie standart et la sortie d'erreur dans le log.
apt-get  install -y $1 2>/dev/null >>$chemin/LogInstallation.log 
```
