#include "analysesyntaxique.h"

AnalyseSyntaxique::AnalyseSyntaxique()
{

}

QString AnalyseSyntaxique::analyse(QStringList lines)
{
    //declaration ( int_number, real,number )
    //affectation ( give, affect )
    //affichage ( showmsg, showval )
    //condition ( if, else, start, finnish ) pas besion f analyse syntaxique
    console.clear();
    // veux meux utilis foreach blast for bech matatkhaletch f les index i j u
    QStringList pile ;
    int dejaEXsite = -1 ;
    for ( int i = 0 ; i < lines.length() ; i++ )
    {

        QString line = lines[i] ;
        QStringList words = line.split(" ");
        QString word = words[0] ;

        if( word.isEmpty())
            continue ;
        if( word.contains("%.."))
            continue ; // exception pour line commentaire

        if(word == "Snl_Int" || word == "Snl_Real")
            if(isDeclaration(words)) continue ;

        if(word == "Get" || word == "Set")
            if(isAffectation(words)) continue ;

        if(word == "Snl_Put" || word == "Snl_Put")
            if(isAffichage(words)) continue ;

        if(word == "If")
            dejaEXsite = i ;
        if( word == "Else")
        {
            if(dejaEXsite == -1)
                console += "If not found erreur dans la line " + QString::number(i) + "\n";
            else
                dejaEXsite = -1 ;
            continue ;
        }
        if( word == "Snl_Star" || word == "Snl_Close"
                || word == "Start" || word == "Finish"  )
        {
            pile << word ;
            continue ;
        }

        if( words[0] == "If" && words[1] == "%" && words[3] == "%" && isCondition(words[2]))
        {
            QString tmp01;
            for ( int j = 4 ; j < words.length() ; j++)
            {
                if( j < words.length()-1)
                    tmp01 += words[j] + " " ;
                else
                    tmp01 += words[j] ;
            }
            lines[i] = tmp01 ;
            i = i -1 ;

            qDebug() << "words " << words << tmp01 ;
            continue;
        }
        qDebug() << "word " << word ;
        console += line +" erreur dans la line " + QString::number(i+1) + "\n";
//            break; // break c'est optional

    }


    QStringList pile1 , pile2 ;
    QString tmpMSG ;
    bool erreur = false ;
    for ( int i = 0 ; i < pile.length() ; i++ )
    {
        QString word = pile[i];
        tmpMSG += word + " " ;
        if( word == "Snl_Star" || word == "Start" )
            pile1 << word ;

        if( word == "Snl_Close")
        {
            if(!pile1.contains("Snl_Star"))
                erreur = true ;
            pile2 << word ;
        }
        if( word == "Finish")
        {
            if(!pile1.contains("Start"))
                erreur = true ;
            pile2 << word ;
        }
        if(pile1.isEmpty() || pile2.isEmpty())
            continue;
        if(pile1[pile1.length()-1] == "Snl_Star" &&
                pile2[0] == "Snl_Close")
        {
            pile1.removeLast();
            pile2.removeFirst();
            if(!pile1.isEmpty() && !pile2.isEmpty())
            {
                console += "erreur dans la pile: "+ tmpMSG + "\n" ;
                return console ;
            }
        }
        else if(pile1[pile1.length()-1] == "Start" &&
                pile2[0] == "Finish")
        {
            pile1.removeLast();
            pile2.removeFirst();
        }

    }
    if(!pile1.isEmpty() && !pile2.isEmpty())
        console += "erreur dans la pile: "+ tmpMSG + "\n" ;
    if(erreur)
        console += "erreur dans la pile: "+ tmpMSG + "\n" ;
    return console ;
}

bool AnalyseSyntaxique::isDeclaration(QStringList words)
{
    if( words.length() < 3 )
        return false ;

    for ( int i = 1 ; i < words.length() -1 ; i++ )
    {
         if(_isIdentificateur(words[i]) || words[i] == ",")
             continue;
         else
             return false ;
    }
    if(words[words.length()-1] != "%.")
        return false ;
    return true ;
}

bool AnalyseSyntaxique::isAffectation(QStringList words)
{
    qDebug() << "bool AnalyseSyntaxique::isAffectation(QStringList words): " << words;
    //protiction contre overflow index
    //if( words.length() != 4 )
    //    return false ;

    if(!_isIdentificateur(words[1]))
        return false ;

    //Set i : 23 %.
    if (words[0] == "Set" && words[3] == "%.")
        if(_isFloat(words[2]) || _isInt(words[2]) )
            return true ;

    //Get j from i %.
    if (words[0] == "Get" && words[2] == "from" && words[4] == "%.")
        if(_isIdentificateur(words[3]) && _isIdentificateur(words[1]))
            return true ;
    return false ; // return false de tout facon
}

bool AnalyseSyntaxique::isAffichage(QStringList words)
{
    //qDebug() << "bool AnalyseSyntaxique::isAffichage(QStringList words): " << words;
    if( words[0] == "Snl_Put" )
    {
        if(  _isIdentificateur(words[1]) && words[2] == "%.")
            return true ;


        if( words[1] == "\"" && words[words.length()-2] == "\"" && words[words.length()-1] == "%.")
            return true ;

    }

    return false ;
}

bool AnalyseSyntaxique::isCondition(QString word)
{
    QStringList tmp ;
    if(word.contains("<") )
        tmp = word.split("<");

    if(word.contains(">") )
        tmp = word.split(">");

    if(word.contains("=") )
        tmp = word.split("=");

    if(tmp.length() != 2 )
        return false;

    if( _isIdentificateur(tmp[0]) && _isIdentificateur(tmp[1]) )
        return true;

    return false ;
}
