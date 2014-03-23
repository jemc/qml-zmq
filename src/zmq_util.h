
#ifndef QML_ZMQ_UTIL_H
#define QML_ZMQ_UTIL_H

#include <QtQml>


class ZMQ_Util : public QObject
{
    Q_OBJECT
    
public:
    static QObject* qmlAttachedProperties(QObject* object);
};


#define LATIN1_CHARS \
" !\"#$&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

class ZMQ_UtilAttached : public QObject
{
    Q_OBJECT
    
public:
    ZMQ_UtilAttached(QObject* attached)
    { m_attached = attached; };
    
private:
    QObject* m_attached = NULL;
    
public slots:
    QString convertLatin1ToData(const QString& str)
    { return QString::fromLatin1(str.toLatin1().toPercentEncoding(LATIN1_CHARS)); }
    
    QString convertDataToLatin1(const QString& str)
    { return QString::fromLatin1(QByteArray::fromPercentEncoding(str.toLatin1())); }
};


QML_DECLARE_TYPEINFO(ZMQ_Util, QML_HAS_ATTACHED_PROPERTIES)

#endif
