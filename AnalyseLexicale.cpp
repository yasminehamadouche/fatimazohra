#include "analyselexicale.h"

AnalyseLexicale::AnalyseLexicale()
{
    language.insert("Snl_Star"," : Mot rÃ©servÃ© dÃ©but d'un programme");
    language.insert("Snl_Close"," : Mot rÃ©servÃ© fin d'un programme");

    language.insert("Snl_Int"," : Mot rÃ©servÃ© dÃ©claration d'un entie");
    language.insert("Snl_Real"," : Mot rÃ©servÃ© dÃ©claration d'un rÃ©el");
    language.insert("Snl_Put"," : Mot rÃ©servÃ© pour affichage d'un message");
    language.insert("Set"," : Mot rÃ©servÃ© affectation d'une valeur");
    language.insert("Get"," : Mot rÃ©servÃ©");
    language.insert("If"," : Mot rÃ©servÃ©");
    language.insert("Else"," : Mot rÃ©servÃ©");
    language.insert("%."," : Mot rÃ©servÃ©");
    language.insert("%.."," : Mot rÃ©servÃ© pour commentaire");
    language.insert("Start"," : Mot rÃ©servÃ©");
    language.insert("Finish"," : Mot rÃ©servÃ©");
    language.insert("From"," : Mot rÃ©servÃ©");

    language.insert("--"," : Mot rÃ©servÃ© pour une condition");
    language.insert("\""," : caractere rÃ©serve pour chaine de caracter");
    language.insert("<"," : operateur de comparaison");
    language.insert(">"," : operateur de comparaison");
    language.insert("%"," : Mot rÃ©servÃ©");
     language.insert(","," : Mot rÃ©servÃ©");


}

QString AnalyseLexicale::analyse(QStringList lines)
{
    console.clear();
    // veux meux utilis foreach blast for bech matatkhaletch f les index i j u
    int msg_detected = 0 ;
    QString tmp ;
    for ( int i = 0 ; i < lines.length() ; i++ )
    {
        msg_detected = 0 ;
        QString line = lines[i] ;
        line.replace("<"," < ");
        line.replace(">"," > ");
        line.replace("="," = ");
        line.replace("  "," ");
        QStringList words = line.split(" ");
        for ( int j = 0 ; j < words.length() ; j++ )
        {
            QString word = words[j] ;
            qDebug() << word ;
            //this is stupid kont kader ndirha foug boucle j pour l'optimisation
            if( word.isEmpty())
                continue ; // continue if word is empty
            if( word.contains("//."))
                continue ; // exception pour line commentaire
            //exception
            if( word == "\"" && msg_detected == 0)
            {
                msg_detected = 1 ;
                tmp += "\"" ;
                continue ;
            }
            if( word == "\"" && msg_detected == 1)
                msg_detected = 2 ;
            if(msg_detected == 1 )
            {
                tmp += word+" " ;
                continue ;
            }

            if(msg_detected == 2 )
            {
                tmp += "\"" ;
                _settext(tmp+" : Affichage d'un message a l'ecran");
                tmp.clear();
                msg_detected=0;
                continue ;
            }

            if(!language.value(word).isEmpty())
                console += word + language.value(word) +"\n" ;
            else
            {
                if( _isIdentificateur(word))
                    _settext(word+" : identificateur");
                else if( _isInt(word))
                    _settext(word+" : Valeur entiere");
                else if( _isFloat(word))
                    _settext(word+" : Valeur flotante");
                else
                    _settext(word+" : erreur <ERR> dans la line "+QString::number(i+1));
            }


        }
    }

    return console ;
}

void AnalyseLexicale::checkDictionary(QString str)
{
    /*En C++ int != integer (cf wikipÃ©dia).
     *  Tu ne peux faire un switch que sur un type de donnÃ©es entiÃ¨res (int, char, enum...),
     *  pour tous les autres cas il te faut utiliser if.
     * url: https://openclassrooms.com/forum/sujet/switch-et-qstring-11302
     * */
    qDebug() << "str: " << str ;
    if( str == "Start_Program")
         _settext(str+" : Mot rÃ©servÃ© dÃ©but d'un programme");
    else if( str == "End_Program")
         _settext(str+" : Mot rÃ©servÃ© fin d'un programme");
    else if( str == "Int_Number")
         _settext(str+" : Mot rÃ©servÃ© dÃ©claration d'un entie");
    else if( str == "Real_Number")
         _settext(str+" : Mot rÃ©servÃ© dÃ©claration d'un rÃ©el");
    else if( str == "Give")
         _settext(str+" : Mot rÃ©servÃ©");
    else if( str == "Affect")
         _settext(str+" : Mot rÃ©servÃ©");
    else if( str == ";;")
         _settext(str+" : Mot rÃ©servÃ©");
    else if( str == "to")
         _settext(str+" : Mot rÃ©servÃ©");
    else if( str == ",")
         _settext(str+" : caractere rÃ©serve");
    else if( str == ":")
         _settext(str+" : caractere rÃ©serve");
    else if( _isIdentificateur(str))
         _settext(str+" : identificateur");
    else if( _isInt(str))
         _settext(str+" : Valeur entiere");
    else if( _isFloat(str))
         _settext(str+" : Valeur flotante");
    else
        _settext(str+" : erreur");

}
