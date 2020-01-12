#include "analysesemantique.h"
AnalyseSemantique::AnalyseSemantique()
{

}


QString AnalyseSemantique::analyse(QStringList lines)
{
    console.clear();
    ram.clear();
    for ( int i = 0 ; i < lines.length() ; i++ )
    {
        QString line = lines[i] ;
        QStringList words = line.split(" ");
        QString word = words[0] ;

        if( word.isEmpty())
            continue ;
        if( word.contains("//."))
            continue ; // exception pour line commentaire

        if(word == "Int_Number" || word == "Real_Number")
            if(Declaration(words)) continue ;

        if(word == "Give" || word == "Affect")
            if(Affectation(words)) continue ;

        if(word == "ShowVal")
            if(Affichage(words)) continue ;

        if( word == "Else")
            continue ;

        if( word == "Start_Program" || word == "End_Program"
                || word == "Start" || word == "Finish" || word == "ShowMes" )
            continue ;
        if( words[0] == "If" && words[1] == "--" && words[3] == "--" && Condition(words[2]))
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

        console += line +" erreur dans la line " + QString::number(i+1) + "\n";


    }


    return console;
}

bool AnalyseSemantique::Declaration(QStringList words)
{
    for ( int i = 2 ; i < words.length() -1 ; i++ )
    {
        if(words[i] == ",")
            continue ;

        variable tmp ;
        if(words[0] == "Int_Number" )
            tmp.type = "int" ;
        else if(words[0] == "Real_Number" )
            tmp.type = "float" ;
        else return false ; // machi int machi float donc erreur
        // pour evite double declaration
        if(findThisIdentificateur(words[i]) > -1)
            return false ;

        tmp.identificateur = words[i] ;
        ram << tmp ;
    }

    return true ;
}


bool AnalyseSemantique::Affectation(QStringList words)
{
    if(words[0] == "Give")
    {
        for (int i = 0 ; i < ram.length() ; i++ )
        {
            //qDebug() << ram[i].type +" "+ ram[i].identificateur +" "+ ram[i].value ;
            if(ram[i].identificateur == words[1])
            {
                if(ram[i].type == "int" && _isInt(words[3]))
                {
                    ram[i].value = words[3] ;
                    return true ;
                }
                else if(ram[i].type == "float" && _isFloat(words[3]))
                {
                    ram[i].value = words[3] ;
                    return true ;
                }
                else return false ;
            }
        }
        console += words[1] + " not found\n";
        return false ;
    }
    if(words[0] == "Affect")
    {
        int x,y ;
        x = findThisIdentificateur(words[1]);
        y = findThisIdentificateur(words[3]);

        if( x < 0 || y < 0)
            return false ;
        if(ram[x].type != ram[y].type )
            return false ;
        ram[y].value = ram[x].value;
        return true ;
    }
    return false ; // to avoid waring msg
}


bool AnalyseSemantique::Affichage(QStringList words)
{
    int index = findThisIdentificateur(words[2]);
    if(index < 0)
        return false ;
    return true ;
}

bool AnalyseSemantique::Condition(QString word)
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

    int x = findThisIdentificateur(tmp[0]) ;
    int y = findThisIdentificateur(tmp[1]) ;
    if( x < 0 || y < 0 )
        return false ;
    if(ram[x].value.isEmpty())
        return false ;
    if(ram[y].value.isEmpty())
        return false ;


    return true ;
}

bool AnalyseSemantique::Condition2(QString word)
{
    QStringList tmp ;
    if(word.contains("<") )
        tmp = word.split("<");

    if(word.contains(">") )
        tmp = word.split(">");

    if(word.contains("=") )
        tmp = word.split("=");


    int x = findThisIdentificateur(tmp[0]) ;
    int y = findThisIdentificateur(tmp[1]) ;
    if( x < 0 || y < 0 )
        return false ;
    double a = ram[x].value.toDouble();
    double b = ram[y].value.toDouble() ;
    qDebug() << "a " << a << " b " << b ;
    if(word.contains("<") )
        return a < b;

    if(word.contains(">") )
        return a > b;

    if(word.contains("="))
        return a = b;
}
int AnalyseSemantique::findThisIdentificateur(QString name)
{
    int index = -1 ;
    for (int i = 0 ; i < ram.length() ; i++)
    {
        if(ram[i].identificateur == name)
        {
            index = i ;
            break;
        }
    }
    return index ;
}

QString AnalyseSemantique::execute(QStringList lines)
{
    console.clear();
    ram.clear();
    bool skipElse = false ;
    int ifCount = 0 ;
    for ( int i = 0 ; i < lines.length() ; i++ )
    {
        QString line = lines[i] ;
        QStringList words = line.split(" ");
        QString word = words[0] ;

        if( word.isEmpty())
            continue ;
        if( word.contains("//."))
            continue ; // exception pour line commentaire

        if(word == "Int_Number" || word == "Real_Number")
            if(Declaration(words)) continue ;

        if(word == "Give" || word == "Affect")
            if(Affectation(words)) continue ;


        if( word == "Else")
            continue ;

        if( word == "Start_Program" || word == "End_Program"
                || word == "Start" || word == "Finish"  )
            continue ;
        if( words[0] == "If" && words[1] == "--" && words[3] == "--" && Condition2(words[2]))
        {
            qDebug() << "fuuuuuuck " ;
            bool firstTime = true ;
            for ( int j = i + 1 ; j < lines.length() ; j++ )
            {
                if(lines[j] != "Else" && firstTime == true)
                    break ;
                firstTime = false ;
                if(lines[j] == "Finish")
                {
                    lines[j] = "" ;
                    break ;
                }
                qDebug() << " lines[j] " <<  lines[j] ;
                lines[j] = "" ;

            }
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

            continue;
        }


        if(word == "ShowVal")
        {
            //qDebug() << ram[findThisIdentificateur(words[2])].value ;
            console += ram[findThisIdentificateur(words[2])].value +"\n" ;
        }
        if( word == "ShowMes")
        {
            //qDebug() << line.replace("ShowMes : ","").replace("\"","") ;
            console += line.replace("ShowMes : ","").replace("\"","") +"\n" ;
        }
    }
    return console;
}
