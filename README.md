# fatimazohra
syntaxic et semantic et lexical analyser pour un type de langage qui est Snail
on vas décoder et rendre ce fichier snail en 3 type d'analyseurs
dans l'analyse lexical on déclare tout les mot de déclaration qui ce trouve en snail tel que Snl_Star pour le début du programme 
dans l'analyse syntaxic on a mis une exception pour line commentair
et on a utiliser le meme principe dans la semantic 
remarque : on ne peux pas faire l'analyse semantic si l'analyse syntaxic n'est pas éffectuer 
le programme :

Snl_Start
Snl_Int i,j,Aft_5,f_f_5 %.
Set i 23 %.
Snl_Real Aft34_2 %.
If % i<j %.
 
Start

Snl_Put "Hello everyone" %.
Set Aft_5 10 %.
	Finish
Else

Start
 Get j from i %.
		
Set Aft34_2   123.54 %.
	
Finish
Snl_Put " ceci est un message " %.

Snl_Put i %.
%.. ceci est un commentaire
Snl_Close


## 
## 
