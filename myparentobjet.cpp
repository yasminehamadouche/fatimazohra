#include "myparentobject.h"

MyParentObject::MyParentObject()
{

}

bool MyParentObject::_isInt(QString str)
{
    bool ok ;
    int dec = str.toInt(&ok, 10) ;
    dec = dec;
    //Q_UNUSED(dec) ;
    return ok ;
}

bool MyParentObject::_isFloat(QString str)
{
    if(str.contains("."))
    {
        QStringList tmp = str.split(".");
        if(tmp.length() != 2 )
            return false ;
        if(_isInt(tmp[0]) && _isInt(tmp[1]))
        {
            return true ;
        }
    }
    return false ;
}

bool MyParentObject::_isIdentificateur(QString str)
{
     if(!str[0].isLetter())
        return false ;
     for ( int i = 1 ; i < str.length() ; i++ )
     {
         if(!str[i].isLetterOrNumber())
             if(str[i] != "_")
                 return false ;
     }
     return true ;
}

void MyParentObject::_settext(QString str)
{
    console += str + "\n" ;
}
